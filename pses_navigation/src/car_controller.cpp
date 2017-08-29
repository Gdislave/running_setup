/*
 * car_controller.cpp
 *
 *      Authors: Sebastian Ehmes
 *				 Nicolas Acero
 */

#include <ros/ros.h>
#include <pses_basis/Command.h>
#include <geometry_msgs/Twist.h>

/* The following variables are car specific and used to calculate steering levels
 * from steering angles provided by the planner.
 */
std::vector<double> ctrlData;
double angleArray[101]={25.0, 24.6, 24.2, 23.8, 23.4, 23.0, 22.6, 22.2, 21.8, 21.4,
						21.0, 20.6, 20.2, 19.8, 19.4, 19.0, 18.6, 18.2, 17.8, 17.4,
						17.0, 16.5, 16.0, 15.5, 15.0, 14.5, 14.0, 13.5, 13.0, 12.5,
						12.0, 11.7, 11.4, 11.1, 10.8, 10.5, 10.2, 9.9, 9.6, 9.3,
						9.0, 8.7, 8.4, 8.1, 7.8, 7.5, 7.2, 6.9, 6.6, 6.3,
						6.0, 5.2, 4.4, 3.6, 2.8, 2.0, 1.2, 0.4, -0.4, -1.2,
						-2.0, -2.3, -2.6, -2.9, -3.2, -3.5, -3.8, -4.1, -4.4, -4.7,
						-5.0, -5.4, -5.8, -6.2, -6.6, -7.0, -7.4, -7.8, -8.2, -8.6,
						-9.0, -9.4, -9.8, -10.2, -10.6, -11.0, -11.4, -11.8, -12.2, -12.6,
						-13.0, -13.5, -14.0, -14.5, -15.0, -15.5, -16.0, -16.5, -17.0, -17.5, -18.0};


/* Determines the appropriate steering level to a given steering angle, using
 * the lookup table from above.
 */
int angleToSteering(double alpha){
    if(alpha>=angleArray[0]){
      return -50;
    }else if(alpha<=angleArray[100]){
      return 50;
    }else{
      for(int i = 1; i <= 100; i++){
        if(alpha < angleArray[i-1] && alpha >= angleArray[i]){
          return i-50;
        }
      }
    }
}
/* Gets a set of commands (motion planner) from the ctrlData container
 * and determines steering levels und motor levels.
 */

void setMotorAndSteering(pses_basis::Command& cmd){
	if(ctrlData.size()>1){

		if(ctrlData[0]>0){
			if(ctrlData[0] < 0.15) {
	    	cmd.motor_level = 2;
			}
			else if(ctrlData[0] < 0.28) {
				cmd.motor_level = 4;
			}
			else cmd.motor_level = 6;
	  }
		else if(ctrlData[0] < 0){
			if(ctrlData[0] > -0.15)
				cmd.motor_level = -6;
			else cmd.motor_level = -8;
	  }
		else {
			cmd.motor_level = 0;
		}
	  cmd.steering_level = angleToSteering(ctrlData[1]*180/3.1415);
	}else{
		cmd.motor_level = 0;
		cmd.steering_level = 0;
	}

}

// The callback for the planner data, used to steer the car
void plannerCommands(const geometry_msgs::Twist::ConstPtr& plannerCMD, ros::Publisher* command_pub){
  ctrlData.clear();
  ctrlData.push_back(plannerCMD->linear.x);
  ctrlData.push_back(plannerCMD->angular.z);
  pses_basis::Command cmd;
  setMotorAndSteering(cmd);
  command_pub->publish(cmd);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "car_controller");

  	ros::NodeHandle n;
	// Publishes an interpretation of the planner commands,
	ros::Publisher command_pub = n.advertise<pses_basis::Command>("pses_basis/command", 50);
	// Here we subscribe to the planner to receive steering commands,
	// when the planner has found a solution for a certain goal.
	ros::Subscriber planner = n.subscribe<geometry_msgs::Twist>("cmd_vel", 10, std::bind(plannerCommands, std::placeholders::_1, &command_pub));

	ros::spin();

	return 0;
}
