#ifndef INCLUDE_CAM_VISION_H_
#define INCLUDE_CAM_VISION_H_

#include <stdio.h>
#include "block.h"

namespace vision{
    class Cam{
        private:
	    const double height;
	    const double hViewAngle=40;
	    const double vViewAngle=60;
	    int running;
            std::thread runner;
	    void processFrame(Mat& inFrame);
	    void poll();
 	    Block blocks[];	    
        public:	    
	    void startPoll();
	    void stopPoll();
	    Block[] getBlocks();
    }
}
#endif
