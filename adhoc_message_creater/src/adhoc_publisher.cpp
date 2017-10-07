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

adhoc_customize::Car2Car CarMessageObject;


int main (int argc, char **argv){
	
  ros::init(argc, argv, "adhoc_message_creater");
  ros::NodeHandle nodehandler;
  //ros::Publisher confPub = nodehandler.advertise<std_msgs::String>("t_filename", 1000, false);

  std::string dst_car = "pses-car6";

  ros::Rate loop_rate(1);


  ros::Duration(5,0).sleep();


  //Settings CarMessageTest attributes:
  CarMessageObject.Teilnehmertyp0Fahrzeug1RSU = 0;
  CarMessageObject.Teilnehmername = std::string("pses-car2");
  CarMessageObject.Nachrichtentyp = std::string("ready");
  CarMessageObject.PositionX = 5;
  CarMessageObject.PositionX = 10;


  while(ros::ok()){

    
  //adhoc_communication::sendMessage(rectangle, FRAME_DATA_TYPE_RECTANGLE, dst_car, "mambo_jambo");
  //adhoc_communication::sendMessage(current_pos, FRAME_DATA_TYPE_POSITION, dst_car, "traffic_light_position");
  adhoc_communication::sendMessage(CarMessageObject, FRAME_DATA_TYPE_CAR2CAR, dst_car, "Car2Car");


  //ros::spinOnce();
  loop_rate.sleep();
  };
  return 1;

}
