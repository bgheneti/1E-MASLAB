#include "../include/robot.h"
#include <math.h>

// In addition to constructing the object, it also localizes.
Robot::Robot(drive::DriveTrain& driveCtrl, control::Pickup& pickup, control::Dropoff& dropoffLeft,
      control::Dropoff& dropoffRight, map::Map& map, map::AngleLocalizer& angleLocalizer, 
      vision::Cam& cam, utils::Color mode) : driveCtrl(driveCtrl), pickup(pickup), dropoffLeft(dropoffLeft),
					     dropoffRight(dropoffRight), map(map), angleLocalizer(angleLocalizer),
					     cam(cam) {
	//  Activate dropoff container servos
	dropoffLeft.resetStack();
	dropoffRight.resetStack();

	// Localize
	angleLocalizer.setLocation(map.getLocation().x, map.getLocation().y);
	map.setHeading(angleLocalizer.getAngle(10));
	std::cout<<"angle localization results in an angle of "<<map.getHeading()<<std::endl;
}

double Robot::driveStraightNormal(double distance) {
	double actualDistance = driveCtrl.straightForDistance(distance, .15);
	map.setLocationStraight(actualDistance);

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
	  map.setHeadingRelative(driveCtrl.turnForDegrees(90,2.0));
	}
	else if(tempHeading<-90){
	  tempHeading+=90;
	  map.setHeadingRelative(driveCtrl.turnForDegrees(-90,2.0));
	}
	double actualHeading = driveCtrl.turnForDegrees(tempHeading, 2.0);
	map.setHeadingRelative(actualHeading);
}

double Robot::driveToNearestStack() {
        std::cout<<"drive to nearest stack"<<std::endl;
        std::vector<map::DrivingInstruction> instructions = map.getDrivingInstructions(
								map.getPathTo(map.getNearestStack()));
	if(instructions.size() < 1) {
		std::cout << "no driving instructions" << std::endl;
		return 0.0;
	}
	for(int i=0; i<instructions.size()-1; i++) {
                std::cout<<"trying to turn to heading:"<<instructions[i].heading<<std::endl;
		turnNormal(instructions[i].heading);
		std::cout<<"trying to drive for:"<<instructions[i].distance<<std::endl;
		driveStraightNormal(instructions[i].distance);
	}

	turnNormal(instructions[instructions.size()-1].heading);
	double lastDistance = instructions[instructions.size()-1].distance;
	if(lastDistance > 0.5) {
		driveStraightNormal(lastDistance - 0.5);
		lastDistance = 0.5;
	} 
  
	return lastDistance;
	

}

void Robot::pickupStack() {
      double expectedDistance = driveToNearestStack();
      std::cout<<"expectedDistance="<<expectedDistance<<std::endl;
      while(expectedDistance > 0.3) {
      	map::DrivingInstruction instruction = cam.getDirectionToBlock(expectedDistance, 0.0);
        expectedDistance = instruction.distance;
        if(instruction.distance < 0) {
		std::cout << "camera can't see block" << std::endl;
  	} 
        std::cout<<"Driving towards block: "<<instruction.heading<<" degrees, "<<instruction.distance<<" meters" << std::endl;
      
        turnNormal(instruction.heading);
	double distanceToTravel = expectedDistance <= 0.5 ? instruction.distance-.2 : 0.5;
	expectedDistance -= .5;
	driveStraightNormal(distanceToTravel);
	}
      pickup.start();
      driveStraightNormal(.5);
      for(int i=0; i<100; i++) {
	if(pickup.getNumPickedUpThisTime()>3) {
		break;
	}
      	usleep(10000000);
      pickup.stop();
     }


}
