#include "../include/cam_vision.h"
#include <cassert>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "cmath"
#include <complex>   

using namespace cv;
using namespace vision;
namespace vision{

  void Cam::processFrame(Mat& inFrame, Mat& outFrame) {
    resize(inFrame, inFrame, Size(), 0.5,0.5,INTER_NEAREST);
    int i=0;
    for(int row = 0; row < inFrame.rows; ++row) {
      uchar* p = inFrame.ptr(row);
      for(int col = 0; col < inFrame.cols; col++) {
	//points to each pixel value in turn assuming a CV_8UC3 color image 
	double b =  (*p);
	double g = (*(p+1));
	double r = (*(p+2));
	if((r > (b * 1.5))/*&&(r < (b * 5))*/&&(r > (g * 1.5))/*&&(r < (g * 5))*/){
	  *p = 0;
	  *(p+1) = 0;
	  *(p+2) =  255;
	  floodFill(inFrame,Point(col,row),Scalar(0, 0, 255.0));
	}
	else if((g > (r * 1.4))/*&&(g < (r * 3))*/&&(g > (b * 1.0))/*&&(g < (b * 2.2))*/){
	  *p = 0;
	  *(p+1) = 255;
	  *(p+2) = 0;
	  floodFill(inFrame,Point(col,row),Scalar(0, 255, 0));
	}
	else{
	  *p = 0;
	  *(p+1) = 0;
	  *(p+2) = 0;
	  //floodFill(inFrame,Point(col,row),Scalar(0, 0, 0));
	}
	//printf("red:%x\n",*(p+2));
	p+=3;
	i++; 
      }
    }
    outFrame=inFrame;
  }

  void Cam::findCubes(Mat& inFrame){
    Mat_<Vec3b> greenFrame(240, 320, Vec3b(0,0,0));
    Mat_<Vec3b> redFrame(240, 320, Vec3b(0,0,0));
    //inFrame=redFrame;
    for(int row = 0; row < inFrame.rows; ++row) {
      uchar* inFramePos = inFrame.ptr(row);
      uchar* redFramePos = redFrame.ptr(row);  
      uchar* greenFramePos = greenFrame.ptr(row);    
      for(int col = 0; col < inFrame.cols; col++) {
	if(*(inFramePos+1)==255){
	  *(greenFramePos+1)=255;
	}
	else if(*(inFramePos+2)==255){
	  *(redFramePos+2)=255;
	}
	inFramePos+=3;
	redFramePos+=3;
	greenFramePos+=3;
      }
    }
    std::cout << inFrame.depth() << ", " << inFrame.channels() << std::endl;
    SimpleBlobDetector::Params params;
    params.minDistBetweenBlobs = 5.0;
    params.minArea = 10.0;
    params.filterByArea= true;
    SimpleBlobDetector blob_detector(params);
    std::cout<<"created detector"<<std::endl;
    vector<cv::KeyPoint> keypoints;
    std::cout<<"declared keypoints"<<std::endl;
    blocks.clear();
    double frameHeight=inFrame.rows;
    double frameWidth=inFrame.cols;
    int i=0;
    blob_detector.detect(redFrame, keypoints);
    for (int i=0; i<keypoints.size()&&i<10; i++){
      double X = keypoints[i].pt.x; 
      double Y = keypoints[i].pt.y;
      //printf("red: %d, x:%f, y:%f\n",i,X,Y);
      double forwardDistance=height * tan(90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight);
      double leftDistance=forwardDistance * tan(30-60*X/frameWidth);
      Color color(red);
      std::complex<double> position(forwardDistance,leftDistance);
      //std::cout<<"making block"<<std::endl;
      
      Block block(position,color);
      blocks.push_back(&block);
      //std::cout<<"made block"<<std::endl;
    }    
    blob_detector.detect(greenFrame, keypoints);
    for (int i=0; i<keypoints.size()&&i<10; i++){
      double X = keypoints[i].pt.x; 
      double Y = keypoints[i].pt.y;
      //printf("green: %d, x:%f, y:%f\n",i,X,Y);
      double forwardDistance=height * tan(90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight);
      double leftDistance=forwardDistance*tan(30-60*X/frameWidth);
      Color color(red);
      std::complex<double> position(forwardDistance,leftDistance);
      //delete blocks[block];
      //std::cout<<"making block"<<std::endl;
      Block block(position,color);
      blocks.push_back(&block);
      //std::cout<<"made block"<<std::endl;
    }
    //std::cout<<"made all blocks"<<std::endl;    
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
	//std::cout << "processing Frame" << std::endl;
	processFrame(testFrame, testOut);
	Size outSize = Size(testOut.cols, testOut.rows);
	std::cout << "outviding" << std::endl;
	VideoWriter outVid("test.avi", CV_FOURCC('M','P','4','2'),10,outSize,true);
	VideoWriter outVid1("test1.avi", CV_FOURCC('M','P','4','2'),10,outSize,true);
        Mat out;
	Mat oldOut=testOut;
	assert(outVid.isOpened());
	//std::cout << "windowing" << std::endl;
	imshow( "Display window", testOut);
	std::cout << running << std::endl;
	while (running) {
	    Mat frame;
	    cap >> frame;
	    outVid << frame;
	    processFrame(frame, out);
	    addMat(oldOut,out);
	    outVid1 << oldOut;
	    findCubes(oldOut);
	    imshow( "Display window", oldOut);
	    oldOut=out;
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

    std::vector<Block*> Cam::getBlocks(){
	return blocks;
    }

}
