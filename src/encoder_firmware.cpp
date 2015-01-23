// Contains the firmware Encoder class to interface with the motor encoders.
           

#include <cassert>
#include <cmath>
#include <thread>
<<<<<<< HEAD
=======
#include <iostream>
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03

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
        for(int i=0; i<4; i++) {stateTicks[i]=0;}
        running = false;
    }

    // Read the value on the pins repeatedly and stores the state.
    void Encoder::poll() {
        while(running) {
            int newState = (gpio1.read()<<1) + gpio2.read();;
            assert(newState<4 && newState>=0);
            if(newState != state) {
                state = newState;
                stateTicks[state]++;
            }
        }
    }

    // Spawn a new thread to continuously poll the encoder
    void Encoder::startPolling() {
        running = true;
<<<<<<< HEAD
	std::thread thr(&Encoder::poll, this);
=======
        std::thread thr(&Encoder::poll, this);
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03
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
        int maxTicks=0;
        for(int i=0; i<4; i++) {
            if(stateTicks[i] > maxTicks) {
                maxTicks = stateTicks[i];
            }
        }
        // 480 is ticks per rotation
        // 0.0492125 is radius of wheel in meters
        return ((double) maxTicks) / 480.0 * 0.0492125;
    }

    // Reset the number of ticks to 0.
    void Encoder::resetNumTicks() {
        for(int i=0; i<4; i++) {stateTicks[i]=0;}
    }

}  

