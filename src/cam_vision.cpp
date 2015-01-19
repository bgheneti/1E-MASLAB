#include "block.cpp"
#include "../include/cam_vision.h"
#include <cassert>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;

namespace vision{

    void processFrame(Mat& inFrame) {
	Mat downFrame;
	int i=0;
	for(int row = 0; row < inFrame.rows; ++row) {
	    uchar* p = inFrame.ptr(row);
	    for(int col = 0; col < inFrame.cols; col++) {
	        //points to each pixel value in turn assuming a CV_8UC3 color image 
		float b =  (*p);
		float g = (*(p+1));
	        float r = (*(p+2));
		if((r > (b * 1.45))&&(r > (g * 1.6))){
		    *p = 0;
		    *(p+1) = 0;
		    *(p+2) =  255;
		}
		else if((g > (r * 1.4))&&(g > (b * 1.05))){
		    *p = 0;
		    *(p+1) = 255;
		    *(p+2) = 0;
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
    }

    void Cam::poll(VideoCapture cap){
	Mat Frame;
	while(running){
	    cap>>frame;
	    processFrame(frame);
	}
    }

    void Cam::startPoll(){
	running=1;
	VideoCapture cap(1);
	assert(cap.isOpened());
	
	std::thread thr(&Cam::poll, this);
	std::swap(thr, runner);
    }

    void Cam::stopPoll(){
	running=0;
	runner.join();
    }

    Block[] Cam::getBlocks(){
	return blocks[];
    }
}
