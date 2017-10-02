#include "ros/ros.h"
#include "adhoc_customize/include.h"
#include "adhoc_communication/functions.h"
#include "adhoc_communication/MmRobotPosition.h"


//#include "ros/topic.h"
//#include "adhoc_customize/include.h"
#include "adhoc_customize/defines.h"
//#include "adhoc_communication/functions.h"
//#include "adhoc_communication/SendString.h"


//#include "adhoc_communication/RecvString.h"
//#include <adhoc_tests/FilenameService.h>
//#include "adhoc_communication/MmRobotPosition.h"
//#include "nav_msgs/Odometry.h"
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sys/time.h>
//#include <tf/transform_listener.h>

//#include "ros/ros.h"
//#include "ros/topic.h"
//#include "adhoc_customize/include.h"
//#include "adhoc_communication/SendString.h"
//#include "adhoc_communication/RecvString.h"
//#include <adhoc_tests/FilenameService.h>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sys/time.h>

adhoc_communication::MmRobotPosition current_pos;
adhoc_customize::Rectangle rectangle;

int main (int argc, char **argv){
	
  ros::init(argc, argv, "adhoc_message_creater");
  ros::NodeHandle nodehandler;
  //ros::Publisher confPub = nodehandler.advertise<std_msgs::String>("t_filename", 1000, false);

  rectangle.length = 10;
  rectangle.width  = 20;
  std::string dst_car = "pses-car6";


  ROS_INFO("length [%d]; width: [%d]", rectangle.length , rectangle.width);
  ros::Rate loop_rate(1);

  int i = 0;
  int loop = 30;



  ros::Duration(5,0).sleep();
  current_pos.position.pose.position.x = 0;
  current_pos.position.pose.position.y = 0;
  current_pos.position.pose.position.z = 0;

  current_pos.position.header.stamp = ros::Time::now();
  current_pos.position.header.frame_id = "base_footprint";

  current_pos.position.pose.orientation.x = 0.0;
  current_pos.position.pose.orientation.y = 0.0;
  current_pos.position.pose.orientation.z = 0.0;
  current_pos.position.pose.orientation.w = 1.0;

  while(ros::ok() && i<loop){
    i++;

  adhoc_communication::sendMessage(rectangle, FRAME_DATA_TYPE_RECTANGLE, dst_car, "mambo_jambo");
  adhoc_communication::sendMessage(current_pos, FRAME_DATA_TYPE_POSITION, dst_car, "traffic_light_position");

  //ros::spinOnce();
  loop_rate.sleep();
  };
  return 1;

}
