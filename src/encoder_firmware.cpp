// Contains the firmware Encoder class to interface with the motor encoders.
           

#include <cassert>
#include <cmath>

#include "../include/encoder_firmware.h"

 
namespace firmware
{
    // This is the class to interact with the motor encoders
    // Sample usage:
    //      firmware::Encoder encoder = firmware::Encoder(inputPin1, inputPin2)
    //      while(running) {
    //          encoder.poll(); // Constantly poll the hardware
    //      }
    //      int numTicks = encoder.getNumTicks(); // Get the number of ticks polled
    //      encoder.reset(); // Reset the tick count to 0
    //
    class Encoder {
        private:
            mraa::Gpio gpio[2]; // the pins the encoder is attached to
            int state; // the state the encoder is currently in (0-3).
            int stateTicks[4]; // keeps track of the number of times each state is read
    };


    Encoder::Encoder(int inputPin1, int inputPin2) {
        gpio[0] = mraa::Gpio(inputPin1);
        gpio[1] = mraa::Gpio(inputPin2);
        state = gpio[0].read()<<1 + gpio[1].read();
        stateTicks = {0, 0, 0, 0};
    }

    // Read the value on the pins at that time and stores the state.
    void Encoder::poll() {
        int newState = gpio[0].read()<<1 + gpio[1].read();;
        assert(newState<4 && newState>=0);
        if(newState != state) {
            state = newState;
            stateTicks[state] = newState;
        }
    }

    // Get the number of ticks registered since the last time the counter was reset.
    //
    // Returns the max of the stateTicks array because it is possible we skipped a state
    // by mistake somewhere.
    int Encoder::getNumTicks() {
        return std::max(stateTicks);
    }

    // Reset the number of ticks to 0.
    void Encoder::resetNumTicks() {
        stateTicks = {0, 0, 0, 0};
    }

}  

