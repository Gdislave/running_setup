#include "ros/ros.h"
#include "adhoc_customize/include.h"
#include "adhoc_communication/functions.h"


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

adhoc_customize::Rectangle rectangle;

int main (int argc, char **argv){
	
  ros::init(argc, argv, "adhoc_message_creater");
  ros::NodeHandle nodehandler;
  //ros::Publisher confPub = nodehandler.advertise<std_msgs::String>("t_filename", 1000, false);

  rectangle.length = 10;
  rectangle.width  = 20;


  ROS_INFO("length [%d]; width: [%d]", rectangle.length , rectangle.width);
  ros::Rate loop_rate(1);

  int i = 0;
  int loop = 30;



  ros::Duration(5,0).sleep();
  while(ros::ok() && i<loop){
    i++;

  adhoc_communication::sendMessage(rectangle, FRAME_DATA_TYPE_RECTANGLE, "pses-car6", "mambo-jambo");

  ros::spinOnce();
  };


}
