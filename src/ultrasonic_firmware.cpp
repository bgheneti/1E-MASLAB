#include <time.h>

#include "../include/ultrasonic_firmware.h"

namespace firmware{
  
  //Constructor
  UltrasonicSensor::UltrasonicSensor(int triggerPin, int echoPin):trigger(triggerPin), echo(echoPin){
    trigger.dir(mraa::DIR_OUT);
    echo.dir(mraa::DIR_IN);
  }
  
  //returns distance in centimeters
  double UltrasonicSensor::getDistance(){
    //variables for counting clock ticks
    clock_t start, stop;
    
    //send pulse
    trigger.write(1);
    usleep(10);
    trigger.write(0);
    
    //record starting time
    start = clock();
    stop = clock();
    
    //listen for echo
    while(echo.read()==0 && (stop - start)/CLOCKS_PER_SEC < 0.1){
      stop = clock();
    }
    //switch over to the correct variables
    //start will actually be the start of the response
    //and stop will be its end
    start = stop;
    stop = clock();
    while(echo.read() == 1 && (stop - start)/CLOCKS_PER_SEC < 0.1){
      stop = clock();
    }
    
    //calculate distance
    return ((double)(stop-start))/CLOCKS_PER_SEC*34000/2;
  }
}
