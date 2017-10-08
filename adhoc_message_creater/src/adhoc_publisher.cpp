#include "ros/ros.h"
#include "adhoc_customize/include.h"
#include "adhoc_communication/functions.h"
#include "adhoc_communication/MmRobotPosition.h"
#include <tf/transform_listener.h>
#include <unordered_map>


//SOLLTE SPÄTER IN EINE NODE ZUSAMMENGEFASST WERDEN
#include "scenario_handler/adhoc_reaction.h"



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

enum State{
  READY,
  ANOTHERSTATE
};

void Car2Car_Callback(const adhoc_customize::Car2Car::ConstPtr& tfl_pos_ptr, std::unordered_map<std::string, adhoc_customize::Car2Car>* pose_map)
{
  //(*pose_map)[tfl_pos_ptr->src_robot].orientation = tfl_pos_ptr->position.pose.orientation;
  //(*pose_map)[tfl_pos_ptr->src_robot].position = tfl_pos_ptr->position.pose.position;
  //std::cout << tfl_pos_ptr->position.pose.orientation.x << ": " << tfl_pos_ptr->position.pose.orientation.y<< " : " << tfl_pos_ptr->position.pose.orientation.z<< " : " << tfl_pos_ptr->position.pose.orientation.w << std::endl;

  ROS_INFO("Updated Map Content: ");
  for (auto& x: *pose_map) {

      //std::cout << x.first << ": " << x.second.position.x << ": " << x.second.position.y << ": " << x.second.position.z << ": " << std::endl;
    }

}





int main (int argc, char **argv){
	
  ros::init(argc, argv, "adhoc_message_creater");
  ros::NodeHandle nodehandler;
  tf::TransformListener positionListener;
  //ros::Publisher confPub = nodehandler.advertise<std_msgs::String>("t_filename", 1000, false);
  bool rescueScenario = false,alreadyPublished = false;
  uintmax_t timer = 0;
  std::string dst_car = "pses-car6";
  std::unordered_map<std::string, geometry_msgs::Pose> pose_map;
  std::unordered_map<std::string, adhoc_customize::Car2Car> adhoc_members;

  ros::Rate loop_rate(1);


  ros::Duration(5,0).sleep();


  //Settings CarMessageTest start attributes:
  CarMessageObject.Teilnehmertyp0Fahrzeug1RSU = 0;
  CarMessageObject.Teilnehmername = std::string("pses-car2");
  CarMessageObject.Nachrichtentyp = std::string("ready");
  CarMessageObject.PositionX = 5;
  CarMessageObject.PositionY = 10;
  tf::StampedTransform transformContainer;
  scenario_handler::adhoc_reaction reactionObject;
  ros::Publisher adhoc_to_cruise_publisher =
  nodehandler.advertise<scenario_handler::adhoc_reaction>("adhoc_publisherToCruiser",5);

  while(ros::ok()){


  //Routine to get current position, and update the object
  positionListener.waitForTransform("/map", "/base_footprint", ros::Time(0), ros::Duration(10.0));
  //tf::StampedTransform transformContainer;
  try
   {
     positionListener.lookupTransform("/map", "/base_footprint",
                               ros::Time(0), transformContainer);
     CarMessageObject.PositionX = transformContainer.getOrigin().x();
     CarMessageObject.PositionY = transformContainer.getOrigin().y();
     //seems to get the right information
     //ROS_INFO("Current position: (%f, %f, )\n", xPos,yPos);
   }
   catch(tf::TransformException &ex)
   {
     ROS_ERROR("%s", ex.what());
     ros::Duration(1.0).sleep();
   }

  //geht bestimmt auch über define...
  if((timer>100)&&rescueScenario&& !(alreadyPublished))
  {
    CarMessageObject.Nachrichtentyp = "SOS";

    reactionObject.message_type = std::string("stopMyself");
    //for(int miraclecounter = 0; miraclecounter < 20; ++miraclecounter )
    //{
      adhoc_to_cruise_publisher.publish(reactionObject);
    //}
    alreadyPublished = true;
    ROS_INFO("Just send a message to stop the machine");
  };
  ++timer;
  //adhoc_communication::sendMessage(rectangle, FRAME_DATA_TYPE_RECTANGLE, dst_car, "mambo_jambo");
  //adhoc_communication::sendMessage(current_pos, FRAME_DATA_TYPE_POSITION, dst_car, "traffic_light_position");
  adhoc_communication::sendMessage(CarMessageObject, FRAME_DATA_TYPE_CAR2CAR, "", "Car2Car");


  //ros::spinOnce();
  loop_rate.sleep();
  };
  return 1;

}
