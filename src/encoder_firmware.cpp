// Contains the firmware Encoder class to interface with the motor encoders.
           

#include <cassert>
#include <cmath>
#include <thread>
#include <iostream>

#include "../include/encoder_firmware.h"

 
namespace firmware
{
    // This is the class to interact with the motor encoders
    // Sample usage:
    //      firmware::Encoder encoder = firmware::Encoder(inputPin1, inputPin2);
    //      encoder.startPolling();
    //      int intermediateNumTicks = encoder.getNumTicks(); 
    //      encoder.stopPolling(); 
    //      int numTicks = encoder.getNumTicks(); 
    //      encoder.reset(); // Reset the tick count to 0
    //

    Encoder::Encoder(int inputPin1, int inputPin2) : gpio1(inputPin1), gpio2(inputPin2) {
        state = gpio1.read()<<1 + gpio2.read();
        ticks = 0;
        running = false;
    }

    // Read the value on the pins repeatedly and stores the state.
    void Encoder::poll() {
        while(running) {
            int newState = (gpio1.read()<<1) + gpio2.read();;
            assert(newState<4 && newState>=0);
	    if(newState == 2 || newState == 3)  {
	      newState ^= 1;  //toggles LSB, converts 2<-->3
	    }
	    ticks += newState - state;
            state = newState;
        }
    }

    // Spawn a new thread to continuously poll the encoder
    void Encoder::startPolling() {
        running = true;
        std::thread thr(&Encoder::poll, this);
        std::swap(thr, runner);
    }

    // Get the poll thread to return and stop
    void Encoder::stopPolling() {
        running = false;
	runner.join();
    }

    // Get the number of ticks registered since the last time the counter was reset.
    //
    // Returns the max of the stateTicks array because it is possible we skipped a state
    // by mistake somewhere.
    double Encoder::getDistance() {
                // 480 is ticks per rotation
        // 0.0492125 is radius of wheel in meters
	// 7.0104 is the constant scaling factor from tuning
        return ((double) ticks) / 480.0 * 0.0492125 * 7.0104 / 4;
    }

    // Reset the number of ticks to 0.
    void Encoder::resetNumTicks() {
            }

}  

