#include "../include/cam_vision.h"
#include <cassert>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "cmath"

using namespace cv;
using namespace vision;
namespace vision{

  void Cam::processFrame(Mat& inFrame, Mat& outFrame) {

    int i=0;

    for(int row = 0; row < inFrame.rows; ++row) {
      uchar* p = inFrame.ptr(row);
      for(int col = 0; col < inFrame.cols; col++) {
	//points to each pixel value in turn assuming a CV_8UC3 color image 
	int b = (*p);
	int g = (*(p+1));
	int r = (*(p+2));
	if((r > (b * 1.45))&&(r > (g * 1.6))){
	  *p = 0;
	  *(p+1) = 0;
	  *(p+2) =  255;
	  floodFill(inFrame,Point(col,row),Scalar(0, 0, 255.0));
	}
	else if((g > (r * 1.3))&&(g > (b * 1.0))){
	  *p = 0;
	  *(p+1) = 255;
	  *(p+2) = 0;
	  floodFill(inFrame,Point(col,row),Scalar(0, 255, 0));
	}
	else{
	  *p = 0;
	  *(p+1) = 0;
	  *(p+2) = 0;
	}
	//printf("red:%x\n",*(p+2));
	p+=3;
	i++; 
      }
    }
    outFrame=inFrame;
  }

  void Cam::findCubes(Mat& inFrame){
    std::cout << inFrame.depth() << ", " << inFrame.channels() << std::endl;
    SimpleBlobDetector::Params params;
    params.minDistBetweenBlobs = 1.0;
    params.minArea = 5.0;
    SimpleBlobDetector blob_detector(params);
    vector<cv::KeyPoint> keypoints;
    blob_detector.detect(inFrame, keypoints);

    for (int i=0; i<keypoints.size(); i++){
      float X = keypoints[i].pt.x; 
      float Y = keypoints[i].pt.y;
      printf("%d, x:%f, y:%f\n",i,X,Y);
    }
  }
  
  void Cam::addMat(Mat& a, Mat& b){
    int i=0;
    for(int row = 0; row < a.rows; ++row) {
      uchar* p = a.ptr(row);
      uchar* q = b.ptr(row);
      for(int col = 0; col < a.cols; col++) {
	//points to each pixel value in turn assuming a CV_8UC3 color image 
	int g = *(p+1);
	int g1 = *(q+1);
	int r = *(p+2);
	int r1 = *(q+2);
	if(!((g==g1) && (r==r1))){
	  *(p+1) = 0;
	  *(p+2) = 0;
	}
	p+=3;
	q+=3;
	i++; 
      }
    }
  }

    void Cam::poll(){
        std::cout << 1 << std::endl;
	VideoCapture cap(0);
	assert(cap.isOpened());
	//	namedWindow( "Display window", WINDOW_AUTOSIZE );
	Mat testFrame;
	cap >> testFrame;
	Mat testOut;
	std::cout << "processing Frame" << std::endl;
	processFrame(testFrame, testOut);
	Size outSize = Size(testOut.cols, testOut.rows);
	std::cout << "outviding" << std::endl;
	VideoWriter outVid("test.avi", CV_FOURCC('M','P','4','2'),10,outSize,true);
	VideoWriter outVid1("test1.avi", CV_FOURCC('M','P','4','2'),10,outSize,true);
	Mat out;
	Mat oldOut=testOut;
	assert(outVid.isOpened());
	std::cout << "windowing" << std::endl;
	//	imshow( "Display window", testOut);
	std::cout << running << std::endl;
	while (running) {
	    Mat frame;
	    cap >> frame;
	    outVid << frame;
	    processFrame(frame, out);
	    addMat(oldOut,out);
	    outVid1 << oldOut;
	    findCubes(oldOut);
	    //	    imshow( "Display window", oldOut);
	    oldOut=out;
	    std::cout << 1 << std::endl;
	}
    }

    void Cam::startPoll(){
	running=1;
	std::thread thr(&Cam::poll, this);
	std::swap(thr, runner);
    }

    void Cam::stopPoll(){
	running=0;
	runner.join();
    }

    Block* Cam::getBlocks(){
	return blocks;
    }

}
