#ifndef INCLUDE_ENCODER_FIRMWARE_H_
#define INCLUDE_ENCODER_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>

namespace firmware {
    class Encoder {
        private:
            mraa::Gpio gpio1; // the pins the encoder is attached to
            mraa::Gpio gpio2; // the pins the encoder is attached to
            int state; // the state the encoder is currently in (0-3).
            int stateTicks[4]; // keeps track of the number of times each state is read
            bool running;
            std::thread runner;
            void poll();
        public:
            Encoder(int inputPin1, int inputPin2);
<<<<<<< HEAD
            int getNumTicks();
=======
            double getDistance();
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03
            void resetNumTicks();
            void startPolling();
            void stopPolling();
    };


}
#endif // INCLUDE_ENCODER_FIRMWARE_
