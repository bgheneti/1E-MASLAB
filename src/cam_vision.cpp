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
	else if((b > ( r * 3))){
	  *p = 255;
	  *(p+1) = 0;
	  *(p+2) = 0;
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


  std::vector<int> vision::Cam::expandPoint(int x, int y, Mat& inFrame, bool green){
    Vec3b color;
    Vec3b replacementColor;
    if(green==0){
      color =  Vec3b(0,0,255);
      replacementColor = Vec3b(0,0,127);
    }
    else if(green==1){
      color =  Vec3b(0,255,0);      
      replacementColor =  Vec3b(0,127,0);            
    }
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
	  if((i!=x) && (j!=y) && (inFrame.at<Vec3b>(j,i)==color)){
	    inFrame.at<Vec3b>(j,i)=replacementColor;
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

  std::vector<vector<int> > vision::Cam::findPoints(Mat& inFrame){
    //std::cout<<"find"<<std::endl;

    std::vector<int> redPoint(2,-1);
    std::vector<int> greenPoint(2,-1);
    int width=inFrame.cols;
    int wallHeightThreshhold=4;
    int height=inFrame.rows;
    bool foundAll=false;
    for(int i=width-1; i>-1;i--){
      for(int j=height-1;j>-1;j--){
	if((j<redPoint[1])&&(j<greenPoint[1])){
	  foundAll=true;
	  break;
	}
        else if(inFrame.at<Vec3b>(j,i)[1]==255){
	  //std::cout<<"found"<<std::endl;
	  inFrame.at<Vec3b>(j,i)=Vec3b(0,127,0);
	  std::vector<int>val=expandPoint(i,j,inFrame,1);
	  //std::cout<<"setting found"<<std::endl;
	  if((val[0]>10) && (val[2]>greenPoint[1])){
	    greenPoint[0]=val[1];
	    greenPoint[1]=val[2];
	  }
	}
	else if(inFrame.at<Vec3b>(j,i)[2]==255){
	  //std::cout<<"found"<<std::endl;
	  inFrame.at<Vec3b>(j,i)=Vec3b(0,0,127);
	  std::vector<int>val=expandPoint(i,j,inFrame,0);
	  //std::cout<<"setting found"<<std::endl;
	  if((val[0]>10) && (val[2]>redPoint[1])){
	    redPoint[0]=val[1];
	    redPoint[1]=val[2];
	  }
	}
	else if(inFrame.at<Vec3b>(j,i)[0]==255){
	  bool isWall=true;
	  for(int y=j;y>std::max(j-wallHeightThreshhold,-1);y--){
	    if(inFrame.at<Vec3b>(y,i)[0]==255){
	      inFrame.at<Vec3b>(y,i)=Vec3b(240,240,0);    
	    }
	    else{
	      isWall=false;
	      break;
	    }
	  }
	  std::cout<<isWall<<std::endl;
	  if(isWall){
	    for(int y=j-5;y>-1;y--){
	      inFrame.at<Vec3b>(y,i)=Vec3b(0,0,0);
	    }
	  }
	}
      }
      if(foundAll){
	break;
      }
    }
    std::cout<<"green:"<<greenPoint[0]<<","<<greenPoint[1]<<std::endl;
    std::vector<vector<int> > points;
    points.push_back(redPoint);
    points.push_back(greenPoint);
    return points;
  }


  //sets entries in vector blocks to the closest red block and the closest green block
  void Cam::findCubes(Mat& inFrame){
    //std::cout<<"find cubes"<<std::endl;

    //std::cout << inFrame.depth() << ", " << inFrame.channels() << std::endl;
    
    blocks.clear();
    double frameHeight=inFrame.rows;
    double frameWidth=inFrame.cols;
    int i=0;
   
    vector<vector<int> > points = findPoints(inFrame);
    Vector<int> redPoint=points[0];
    vector<int> greenPoint=points[1];
    if(redPoint[0]!=-1){
      double X = redPoint[0]; 
      double Y = redPoint[1];
      printf("red: x:%f, y:%f\n",X,Y);
      
      for(int x=std::max(0.0,X-3);x<std::min(X+4,frameWidth);x++){
	for(int y=std::max(0.0,Y-3);y<std::min(Y+4,frameHeight);y++){
	  inFrame.at<Vec3b>(Point(x,y))=Vec3b(255,255,255);
	}
      }

      double angle= 60 * ( (X-frameWidth/2) / frameWidth);
      double forwardDistance = (height * tan( (4 * atan(1) / 180) * (90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight))+cameraDistance);
      double distance = forwardDistance/cos( (4 * atan(1) / 180) * angle);
      Color color(red);
      vector<double> position;
      position.push_back(angle);
      position.push_back(distance);
      Block block(position,color);
      blocks.push_back(block);
      std::cout<<"made block"<<std::endl;
    }
    if(greenPoint[0]!=-1){
      double X = greenPoint[0]; 
      double Y = greenPoint[1];
      printf("green: %d, x:%f, y:%f\n",i,X,Y);

      for(int x=std::max(0.0,X-3);x<std::min(X+4,frameWidth);x++){
	for(int y=std::max(0.0,Y-3);y<std::min(Y+4,frameHeight);y++){
	  inFrame.at<Vec3b>(Point(x,y))=Vec3b(255,255,255);
	}
      }
      double angle= 60 * ( (X-frameWidth/2) / frameWidth);
      double forwardDistance=  (height * tan( (4 * atan(1) / 180) * (90-hViewAngle/2-angleDown+hViewAngle * (frameHeight-Y)/frameHeight))+cameraDistance);
      double distance= forwardDistance/ cos( (4 * atan(1) / 180) * angle);
      Color color(green);
      vector<double> position;
      position.push_back(angle);
      position.push_back(distance);
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
    //std::cout << "outviding" << std::endl;
    VideoWriter outVid("testEd.avi", CV_FOURCC('M','P','4','2'),10,outSize,true);
    //VideoWriter outVid1("test1.avi", CV_FOURCC('M','P','4','2'),10,outSize,true);
    Mat out;
    Mat oldOut=testOut;
    assert(outVid.isOpened());
    //std::cout << "windowing" << std::endl;
    //imshow( "Display window", testOut);
    std::cout << running << std::endl;
    while (running) {
      Mat frame;
      cap >> frame;
      processFrame(frame, out);
      findCubes(out);
      //imshow( "Display window", oldOut);
      outVid << out;
    }
  }


  void Cam::startPoll(){
    running=1;
    std::thread thr(&Cam::poll, this);
    std::swap(thr, runner);
    runner.detach();
  }

  void Cam::stopPoll(){
    running=0;
  }

  std::vector<Block> Cam::getBlocks(){
    return blocks;
  }

}
