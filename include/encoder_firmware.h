#ifndef INCLUDE_ENCODER_FIRMWARE_H_
#define INCLUDE_ENCODER_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>

namespace firmware {
    class Encoder {
        private:
            mraa::Gpio& gpio1; // the pins the encoder is attached to
            mraa::Gpio& gpio2; // the pins the encoder is attached to
            int state; // the state the encoder is currently in (0-3).
            int counter;
	    //int ticks[4];
	    bool running;
            std::thread runner;
            void poll();
        public:
            Encoder(mraa::Gpio& input1, mraa::Gpio& input2);
            double getDistance();
            void resetNumTicks();
            void startPolling();
            void stopPolling();
    };


}
#endif // INCLUDE_ENCODER_FIRMWARE_
