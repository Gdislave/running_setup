#include "std_msgs/String.h"
#include <math.h>
#include "sstream"
#include <iostream>
#include <utility>
#include <ros/ros.h>
#include <pses_basis/Command.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <functional>

//Subscriber Callbacks
void cmd_velCallback(const geometry_msgs::Twist::ConstPtr& cmd_vel_msg,
     geometry_msgs::Twist *cmd_vel){
            *cmd_vel = *cmd_vel_msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "segfault_nav_ctrl");
  ros::NodeHandle nstack_ctrl;
  ros::Rate loop_rate(40);
  //Declaration and Initialization of variables
  pses_basis::Command cmd;
  geometry_msgs::Twist cmd_vel;
  /*
        Maximal values for steering and motor level
        according to vehicle specs; bias term for errors in steering
  */
  const float max_motor_level = 20.f;
  const float min_motor_level = -20.f;
  float speed_level = 0.f;
  double PI = 3.14159;
  double velCmd = 0;
  double yawCmd = 0;
  float BoundCheck = 0;

  //Publisher for cmd.steering_level and cmd.motor_level
  ros::Publisher commands_pub =
  nstack_ctrl.advertise<pses_basis::Command>("pses_basis/command", 10);

  //Subscriber for cmd_vel output of navigation stack
  ros::Subscriber cmd_vel_sub =
  nstack_ctrl.subscribe<geometry_msgs::Twist>
   ("cmd_vel", 10, std::bind(cmd_velCallback, std::placeholders::_1, &cmd_vel));

   while(ros::ok())
    {
        //Gets linear and angular control velocities from navigation stack
        velCmd = (double)cmd_vel.linear.x;
        yawCmd = (double)cmd_vel.angular.z;

        /*
              Approx. max. vel.: 2 metres/second @ motor_level: 20
              thus metric velocity values linear with motor Level
              at a slope of 10 motor_level per 1 metre/second.
        */
        speed_level = std::ceil(velCmd * 10.f);
        cmd.motor_level =
        (speed_level > max_motor_level) ? (max_motor_level) : (speed_level);

        cmd.motor_level =
        (speed_level < min_motor_level) ? (min_motor_level) : (speed_level);

        /*
              Check if min. motor_level to overcome friction is reached;
              otherwise set motor_level to the necessary value.
              Also checks if maxima of motor level are exceeded.
        */

        
      	if((cmd.motor_level < 7) && (cmd.motor_level > 0)){
      	    	cmd.motor_level = 6;
        }
      	if((cmd.motor_level > -12) && (cmd.motor_level < 0)){
      	    	cmd.motor_level = -12;
        }

        //Converts steering angle to degrees
        yawCmd = yawCmd * (180.f/PI);

        BoundCheck = std::ceil(yawCmd * 2.0);
        if(BoundCheck > 50){
        	cmd.steering_level = 50;
        }else if(BoundCheck < -50){
        	cmd.steering_level = -50;
        }else{
        	cmd.steering_level = BoundCheck;
        }
        //Publish motor_level and steering_level
        commands_pub.publish(cmd);
        ros::spinOnce();
        loop_rate.sleep();
    }
  ros::spin();
  return 0;
}
