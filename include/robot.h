#ifndef INCLUDE_ROBOT_H
#define INCLUDE_ROBOT_H

#include "pickup.h"
#include "dropoff.h"
#include "drive_ctrl.h"
#include "color.h"
#include "angle_localizer.h"
#include "cam_vision.h"
#include "map.h"

class Robot {

	private:
		drive::DriveTrain& driveCtrl;
		control::Pickup& pickup;
		control::Dropoff& dropoffLeft;
		control::Dropoff& dropoffRight;
		map::Map& map;
		map::AngleLocalizer& angleLocalizer;
		utils::Color mode;
		vision::Cam& cam;
		double driveStraightNormal(double distance);
		double turnNormal(double heading);

	public:
		// In addition to constructing the object, it also localizes
		Robot(drive::DriveTrain& driveCtrl, control::Pickup& pickup, control::Dropoff& dropoffLeft,
		      control::Dropoff& dropoffRight, map::Map& map, map::AngleLocalizer& angleLocalizer, 
		      vision::Cam& cam, utils::Color mode);
		double driveToNearestStack(); 
		void pickupStack();

};


#endif
