#include "../include/cam_vision.h"
#include <cassert>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "cmath"
#include <complex>
#include <queue>  
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


  std::vector<int> vision::Cam::expandPoint(int x, int y, Mat& inFrame){
    std::vector<int> vals(3,0);
    vals[0]= 0;
    vals[1]=-1;
    vals[2]=-1;
    std::queue<vector<int> > points;
    std::vector<int> point;
    point.push_back(x);
    point.push_back(y);
    points.push(point);
    while(points.size()>0){
      std::vector<int> currentPoint=points.front();
      points.pop();
      int x=currentPoint[0];
      int y=currentPoint[1];
      //std::cout<<x<<","<<y<<std::endl;
      vals[0]+=1;
      if(vals[2]<currentPoint[1]){
	vals[1]=currentPoint[0];
	vals[2]=currentPoint[1];
      }
      for(int i=std::max(x-1,0);i<std::min(x+2,inFrame.cols);i++){
	for(int j=std::max(y-1,0);j<std::min(y+2,inFrame.rows);j++){
	  if((i!=x) && (j!=y) && (inFrame.at<Vec3b>(j,i)[0]==255)){
	    inFrame.at<Vec3b>(j,i)[0]=100;
	    std::vector<int> newPoint;
	    newPoint.push_back(i);
	    newPoint.push_back(j);
	    points.push(newPoint);
	  }
	}
      }
      
    }
    return vals;
  }
  //recursively expands a points to determine the size of and lowest point on the coloration in inFrame
  vector<int> vision::Cam::findPoint(Mat& inFrame){
    //std::cout<<"find"<<std::endl;
    std::vector<int> point(2,-1);
    int width=inFrame.cols;
    int height=inFrame.rows;
    for(int i=0; i<width;i++){
      for(int j=0;j<height;j++){
	if(inFrame.at<Vec3b>(j,i)[0]==255){
	  //std::cout<<"found"<<std::endl;
	  inFrame.at<Vec3b>(j,i)=Vec3b(0,255,0);
	  std::vector<int>val=expandPoint(i,j,inFrame);
	  //std::cout<<"setting found"<<std::endl;
	  if((val[0]>10) && (val[2]>point[1])){
	    point[0]=val[1];
	    point[1]=val[2];
	  }
	}
      }
    }
    std::cout<<point[0]<<":"<<point[1]<<std::endl;
    return point;
  }


  //sets entries in vector blocks to the closest red block and the closest green block
  void Cam::findCubes(Mat& inFrame){
    std::cout<<"find cubes"<<std::endl;
    Mat_<Vec3b> greenFrame(240, 320, Vec3b(0,0,0));
    Mat_<Vec3b> redFrame(240, 320, Vec3b(0,0,0));
    //inFrame=redFrame;
    for(int row = 0; row < inFrame.rows; ++row) {
      uchar* inFramePos = inFrame.ptr(row);
      uchar* redFramePos = redFrame.ptr(row);  
      uchar* greenFramePos = greenFrame.ptr(row);    
      for(int col = 0; col < inFrame.cols; col++) {
	if(*(inFramePos+1)==255){
	  *(greenFramePos)=255;
	  *(greenFramePos+1)=255;
	  *(greenFramePos+2)=255;
	}
	else if(*(inFramePos+2)==255){
	  *(redFramePos)=255;
	  *(redFramePos+1)=255;
	  *(redFramePos+2)=255;
	}
	inFramePos+=3;
	redFramePos+=3;
	greenFramePos+=3;
      }
    }
    std::cout << inFrame.depth() << ", " << inFrame.channels() << std::endl;
    
    blocks.clear();
    double frameHeight=inFrame.rows;
    double frameWidth=inFrame.cols;
    int i=0;
   
    vector<int> point= findPoint(redFrame);
    if(point[0]!=-1){
      double X = point[0]; 
      double Y = point[1];
      //printf("red: x:%f, y:%f\n",X,Y);
      
      for(int x=std::max(0.0,X-1);x<std::min(X+2,frameWidth);x++){
	for(int y=std::max(0.0,Y-1);y<std::min(Y+2,frameWidth);y++){
	  inFrame.at<Vec3b>(Point(x,y))=Vec3b(255,0,0);
	}
      }

      double forwardDistance=height * tan(90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight);
      double leftDistance=forwardDistance * tan(30-60*X/frameWidth);
      Color color(red);
      std::complex<double> position(forwardDistance,leftDistance);
      Block block(position,color);
      blocks.push_back(block);
      //std::cout<<"made block"<<std::endl;
    }

    point= findPoint(redFrame);
    if(point[0]!=-1){
      double X = point[0]; 
      double Y = point[1];
      //printf("green: %d, x:%f, y:%f\n",i,X,Y);

      for(int x=std::max(0.0,X-1);x<std::min(X+2,frameWidth);x++){
	for(int y=std::max(0.0,Y-1);y<std::min(Y+2,frameWidth);y++){
	  inFrame.at<Vec3b>(Point(x,y))=Vec3b(255,0,0);
	}
      }

      double forwardDistance=height * tan(90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight);
      double leftDistance=forwardDistance*tan(30-60*X/frameWidth);
      Color color(red);
      std::complex<double> position(forwardDistance,leftDistance);
      Block block(position,color);
      blocks.push_back(block);
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
    //std::cout << "windowing" << std::endl;
    //imshow( "Display window", testOut);
    std::cout << running << std::endl;
    while (running) {
      Mat frame;
      cap >> frame;
      outVid << frame;
      processFrame(frame, out);
      addMat(oldOut,out);
      outVid1 << oldOut;
      findCubes(oldOut);
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

  std::vector<Block> Cam::getBlocks(){
    return blocks;
  }

}
