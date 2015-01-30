#include "../include/robot.h"
#include <math.h>

Robot::Robot(drive::DriveTrain& driveCtrl, control::Pickup& pickup, control::Dropoff& dropoffLeft,
      control::Dropoff& dropoffRight, vision::Cam& cam, utils::Color mode) : driveCtrl(driveCtrl), pickup(pickup), 
					                                     dropoffLeft(dropoffLeft),
					                                     dropoffRight(dropoffRight), 
					                                     cam(cam) {
	//  Activate dropoff container servos
	dropoffLeft.resetStack();
	dropoffRight.resetStack();

}

double Robot::driveStraightNormal(double distance) {
	double actualDistance = driveCtrl.straightForDistance(distance, .15, 5.0);

}

double Robot::turnNormal(double heading) {
        std::cout<<"turnNormal heading:"<<heading<<std::endl;
        double tempHeading=heading;
        tempHeading=fmod(tempHeading,360.0);
	if(tempHeading>180){
	  tempHeading-=180;
	}
	else if(tempHeading<-180){
	  tempHeading+=180;
	}
	std::cout<<"turnNormal tempHeading:"<<tempHeading<<std::endl;
	if(tempHeading>90){
	  tempHeading-=90;
	}
	else if(tempHeading<-90){
	  tempHeading+=90;
	}
	double actualHeading = driveCtrl.turnForDegrees(tempHeading, 2.0, 5.0);
}

void Robot::pickupStack(map::DrivingInstruction instruction) {
	turnNormal(instruction.heading);
	pickup.start();
	driveStraightNormal(instruction.distance);	
	for(int i=0; i<100; i++) {
	      if(pickup.getNumPickedUpThisTime()>=3) {
		     break;
	        }
      	     usleep(10000000);
	}	
        pickup.stop();
 }



