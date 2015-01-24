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
    int filled=10;
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
	  //floodFill(inFrame,Point(col,row),Scalar(0, 0, 255.0));
	}
	else if((g > (r * 1.4))/*&&(g < (r * 3))*/&&(g > (b * 1.0))/*&&(g < (b * 2.2))*/){
	  *p = 0;
	  *(p+1) = 255;
	  *(p+2) = 0;
	  //floodFill(inFrame,Point(col,row),Scalar(0, 255, 0));
	}
	else{
	  *p = 0;
	  *(p+1) = 0;
	  *(p+2) = 0;
	  if(filled>0){
	    floodFill(inFrame,Point(col,row),Scalar(0, 0, 0));
	    filled-=1;
	  }
	}
	//printf("red:%x\n",*(p+2));
	p+=3;
	i++; 
      }
    }
    outFrame=inFrame;
  }

  void Cam::findCubes(Mat& inFrame){
    Mat_<Vec3b> greenFrame(240, 320, Vec3b(255,255,255));
    Mat_<Vec3b> redFrame(240, 320, Vec3b(255,255,255));
    //inFrame=redFrame;
    for(int row = 0; row < inFrame.rows; ++row) {
      uchar* inFramePos = inFrame.ptr(row);
      uchar* redFramePos = redFrame.ptr(row);  
      uchar* greenFramePos = greenFrame.ptr(row);    
      for(int col = 0; col < inFrame.cols; col++) {
	if(*(inFramePos+1)==255){
	  *(greenFramePos)=0;
	  *(greenFramePos+1)=0;
	  *(greenFramePos+2)=0;

	}
	else if(*(inFramePos+2)==255){
	  *(redFramePos)=0;
  	  *(redFramePos+1)=0;
	  *(redFramePos+2)=0;

	}
	inFramePos+=3;
	redFramePos+=3;
	greenFramePos+=3;
      }
    }
    SimpleBlobDetector::Params params;
    params.minDistBetweenBlobs = -3.0;
    params.minArea = 40.0;
    params.filterByArea= true;
    SimpleBlobDetector blob_detector(params);
    vector<cv::KeyPoint> keypoints;
    blocks.clear();
    double frameHeight=inFrame.rows;
    double frameWidth=inFrame.cols;
    int i=0;
    std::cout<<"finding keypoints"<<std::endl;
    //Red Blocks

    blob_detector.detect(redFrame, keypoints);
    for (int i=0; i<keypoints.size()&&i<10; i++){
      double X = keypoints[i].pt.x; 
      double Y = keypoints[i].pt.y;
      double radius = keypoints[i].size;
      Y+=radius;
      for(int x=std::max(0.0,X-1);x<std::min(X+2.0,frameWidth);x++){
	for(int y=std::max(0.0,Y-1);y<std::min(Y+2.0,frameWidth);y++){
	    inFrame.at<Vec3b>(Point(x,y))=Vec3b(255,0,0);
	  }
      }
      printf("red: x:%f, y:%f\n",X,Y);
      double forwardDistance=height * tan(90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight);
      double leftDistance=forwardDistance * tan(30-60*X/frameWidth);
      Color color(red);
      std::complex<double> position(forwardDistance,leftDistance);
      //std::cout<<"making block"<<std::endl;
      
      Block block(position,color);
      blocks.push_back(&block);
      //std::cout<<"made block"<<std::endl;
    }

    //Green Blocks
    
    blob_detector.detect(greenFrame, keypoints);
    for (int i=0; i<keypoints.size()&&i<10; i++){
      double X = keypoints[i].pt.x; 
      double Y = keypoints[i].pt.y;
      double radius = keypoints[i].size;
      Y+=radius;
      for(int x=std::max(0.0,X-1);x<std::min(X+2,frameWidth);x++){
	for(int y=std::max(0.0,Y-1);y<std::min(Y+2,frameWidth);y++){
	      inFrame.at<Vec3b>(Point(x,y))=Vec3b(255,0,0);
	  }
      }
      printf("green:x:%f, y:%f\n",X,Y);
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
	//namedWindow( "Display window", WINDOW_AUTOSIZE );
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
	assert(outVid1.isOpened());
	//std::cout << "windowing" << std::endl;
	//imshow( "Display window", testOut);
	std::cout << running << std::endl;
	while (running) {
	    Mat frame;
	    cap >> frame;
	    outVid << frame;
	    processFrame(frame, out);
	    addMat(oldOut,out);
	    findCubes(oldOut);
	    outVid1 << oldOut;
	    //imshow( "Display window", oldOut);
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
