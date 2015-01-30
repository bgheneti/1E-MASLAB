#include <sys/time.h>
#include <cmath>
#include <math.h>
#include <iostream>

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
    //usleep(10);
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
    if (diff*34000/2 > 300){
      return -1;
    }
    else{
      return diff*34000/2;
    }
  }
  
  double UltrasonicSensor::getHighestProbDistance(int n){
    double distance = 0; //running sum/averagee of distances found
    double curr = 0; //current distance measurement
    int numReadings = 0; //number of successful hits found
    for (int i = 0; i < n; i++){
      curr = getDistance();
      if (curr != -1){
        numReadings++;
        distance += curr;
      }
    }
    if (numReadings==0){
      return -1;
    }
    return distance / numReadings;
    
  }
  
  double UltrasonicSensor::getStdDev(int n, double hpDist){
    if (hpDist == -1){ //in the case that an invalid measurement was taken
      return -1; //skip calculation
    }
    
    return 1.5;//sqrt(n*1.0);
  }
  
}
