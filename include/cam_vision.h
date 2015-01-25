#ifndef INCLUDE_CAM_VISION_H_
#define INCLUDE_CAM_VISION_H_

#include <stdio.h>
#include "block.h"
#include <thread>
#include "cmath"
#include "opencv2/opencv.hpp"

using namespace cv;

namespace vision{
    class Cam{
        private:
	    const double height=0.3556;
	    const double hViewAngle=40;
	    const double vViewAngle=60;
	    const double angleDown=25;
	    const double cameraDistance=0.0762;
	    int running;
            std::thread runner;
	    void processFrame(Mat& inFrame);
	    void poll();
	    void addMat(Mat& a, Mat& b);
	    void findCubes(Mat& inFrame);
	    void processFrame(Mat& inFrame, Mat& outFrame);
	    std::vector<double> expandPoint(int x, int y, Mat& inFrame);
	    std::vector<double> findPoint(Mat& inFrame);
	    std::vector<Block> blocks;
        public:	  
	    void startPoll();
	    void stopPoll();
	    std::vector<Block> getBlocks();
    };
}
#endif
