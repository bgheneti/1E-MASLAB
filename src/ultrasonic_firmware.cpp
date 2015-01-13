#include <sys/time.h>

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
    //clock_t start, stop;
    struct timeval start, stop;
    
    //send pulse
    trigger.write(1);
    usleep(10);
    trigger.write(0);
    
    //record starting time
    //start = clock();
    //stop = clock();
    gettimeofday(&start, NULL);
    gettimeofday(&stop, NULL);
    int secs = stop.tv_sec - start.tv_sec;
    int usecs = stop.tv_usec - start.tv_usec;
    double diff = double(secs)+0.000001*usecs;
    
    //listen for echo
    while(echo.read()==0 && diff < 0.1){
      gettimeofday(&stop, NULL);
      secs = stop.tv_sec - start.tv_sec;
      usecs = stop.tv_usec - start.tv_usec;
      diff = double(secs)+0.000001*usecs;
    }
    //switch over to the correct variables
    //start will actually be the start of the response
    //and stop will be its end
    start = stop;
    gettimeofday(&stop, NULL);
    secs = stop.tv_sec - start.tv_sec;
    usecs = stop.tv_usec - start.tv_usec;
    diff = double(secs)+0.000001*usecs;
    
    while(echo.read() == 1 && diff < 0.1){
      gettimeofday(&stop, NULL);
      secs = stop.tv_sec - start.tv_sec;
      usecs = stop.tv_usec - start.tv_usec;
      diff = double(secs)+0.000001*usecs;
    }
    
    //calculate distance
    return diff*34000/2;
  }
}
