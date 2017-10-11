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



enum State{
  READY,
  ANOTHERSTATE
};
adhoc_customize::Car2Car subscribe_CarMessageObject;
/*
void Car2Car_Callback(const adhoc_customize::Car2Car::ConstPtr& Car2CarMsgR_ptr, std::unordered_map<std::string, adhoc_customize::Car2Car>* pose_map)
{

  //(*pose_map)[Car2CarMsgR_ptr->Teilnehmername] = Car2CarMsgR_ptr;
  pose_map->insert(Car2CarMsgR_ptr->Teilnehmername,Car2CarMsgR_ptr);
  //(*pose_map)[tfl_pos_ptr->src_robot].position = tfl_pos_ptr->position.pose.position;
  //std::cout << tfl_pos_ptr->position.pose.orientation.x << ": " << tfl_pos_ptr->position.pose.orientation.y<< " : " << tfl_pos_ptr->position.pose.orientation.z<< " : " << tfl_pos_ptr->position.pose.orientation.w << std::endl;

  ROS_INFO("Updated Map Content: ");
  for (auto& x: *pose_map) {

      //std::cout << x.first << ": " << x.second.position.x << ": " << x.second.position.y << ": " << x.second.position.z << ": " << std::endl;
    }

}
*/


//Subscriber Callback
void car2car_standard_callback(const adhoc_customize::Car2Car::ConstPtr& Car2CarMsgR_rcvd,
                               adhoc_customize::Car2Car Car2CarMsgR_own)
  {

      //does not work....
      ROS_INFO("I've heard something.");
      //why no copy consctructore
      //(*Car2CarMsgR_own).Teilnehmername = Car2CarMsgR_rcvd->Teilnehmername;
      //(*Car2CarMsgR_own).Nachrichtentyp = Car2CarMsgR_rcvd->Nachrichtentyp;
      //ROS_INFO("Updated Content: %s", (*Car2CarMsgR_own).Teilnehmername.c_str() );
      //ROS_INFO("State of member: %s", (*Car2CarMsgR_own).Nachrichtentyp.c_str() );
  }

void car2car_simple_callback(const adhoc_customize::Car2Car::ConstPtr& Car2CarMsgR_rcvd)
  {

      ROS_INFO("I've heard something.");
      subscribe_CarMessageObject = *Car2CarMsgR_rcvd;

}



int main (int argc, char **argv){
	
  ros::init(argc, argv, "adhoc_message_creater");
  ros::NodeHandle nodehandler;
  tf::TransformListener positionListener;
  //ros::Publisher confPub = nodehandler.advertise<std_msgs::String>("t_filename", 1000, false);
  bool rescueScenario = false,alreadyPublished = false;
  uintmax_t timer = 0;
  std::string dst_car = "";
  //std::unordered_map<std::string, geometry_msgs::Pose> pose_map;
  //std::unordered_map<std::string, adhoc_customize::Car2Car> adhoc_members;
  adhoc_customize::Car2Car publish_CarMessageObject;
  //adhoc_customize::Car2Car subscribe_CarMessageObject;
  std::string Teilnehmertyp = "";//Fahrzeug,Ampel,Geschwindigkeitsbegrenzung


  ros::Rate loop_rate(1);


  ros::Duration(5,0).sleep();


  //Settings CarMessageTest start attributes:
   if(Teilnehmertyp == "Fahrzeug")
  {

      publish_CarMessageObject.Teilnehmertyp0Fahrzeug1RSU = 0;
      publish_CarMessageObject.Teilnehmername = std::string("pses-car2");
      publish_CarMessageObject.Nachrichtentyp = std::string("ready");
      publish_CarMessageObject.PositionX = 5;
      publish_CarMessageObject.PositionY = 10;

  };

   if(Teilnehmertyp == "Geschwindigkeitsbegrenzung")
  {

      publish_CarMessageObject.Teilnehmertyp0Fahrzeug1RSU = 1;
      publish_CarMessageObject.Teilnehmername = std::string("RSU1");
      publish_CarMessageObject.Nachrichtentyp = std::string("limit");
      publish_CarMessageObject.PositionX = 12;
      publish_CarMessageObject.PositionY = -2;
      publish_CarMessageObject.limit_tag = "HollywoodHills";
      publish_CarMessageObject.speedLimit = 0.6;

  };

   if(Teilnehmertyp == "Ampel")//Braucht Verbesserung
  {

      publish_CarMessageObject.Teilnehmertyp0Fahrzeug1RSU = 1;
      publish_CarMessageObject.Teilnehmername = std::string("Ampel");
      publish_CarMessageObject.Nachrichtentyp = std::string("Ampel");
      publish_CarMessageObject.PositionX = 12;
      publish_CarMessageObject.PositionY = -2;

  };

  tf::StampedTransform transformContainer;
  scenario_handler::adhoc_reaction reactionObject;
  ros::Publisher adhoc_to_cruise_publisher =
  nodehandler.advertise<scenario_handler::adhoc_reaction>("adhoc_publisherToCruiser",5);

  //ros::Subscriber Car2Car_subscriber = nodehandler.subscribe<adhoc_customize::Car2Car>("Car2Car", 10, boost::bind(car2car_standard_callback, _1, &subscribe_CarMessageObject));
  //ros::Subscriber tfp_sub = nh.subscribe<adhoc_communication::MmRobotPosition>("traffic_light_position", 10, std::bind(traffic_light_positionCallback, std::placeholders::_1, &pose_map));

  //usual binding seems not to work for objects using customize
  ros::Subscriber Car2Car_subscriber = nodehandler.subscribe<adhoc_customize::Car2Car>("Car2Car", 3, car2car_simple_callback) ;



  while(ros::ok()){

  if(Teilnehmertyp == "Fahrzeug")//nur relevant, wenn die Position verändert wird
  {

      //Routine to get current position, and update the object
      positionListener.waitForTransform("/map", "/base_footprint", ros::Time(0), ros::Duration(10.0));
      //tf::StampedTransform transformContainer;
      try
       {
         positionListener.lookupTransform("/map", "/base_footprint",
                                   ros::Time(0), transformContainer);
         publish_CarMessageObject.PositionX = transformContainer.getOrigin().x();
         publish_CarMessageObject.PositionY = transformContainer.getOrigin().y();
         //seems to get the right information
         //ROS_INFO("Current position: (%f, %f, )\n", xPos,yPos);
       }
       catch(tf::TransformException &ex)
       {
         ROS_ERROR("%s", ex.what());
         ros::Duration(1.0).sleep();
       }

  }
  //Szenario1 das eigene Fahrzeug ist defekt
  //geht bestimmt auch über define...
  if((timer>100)&&rescueScenario&& !(alreadyPublished))
  {
    publish_CarMessageObject.Nachrichtentyp = "SOS";

    reactionObject.message_type = std::string("stopMyself");
    adhoc_to_cruise_publisher.publish(reactionObject);
    alreadyPublished = true;
    ROS_INFO("Just send a message to stop the machine");

  };
  ++timer;

  //Szenario1 Das folgende Fahrzeug eilt zur Hilfe
  if((subscribe_CarMessageObject.Nachrichtentyp == "SOS") && !(alreadyPublished))
  {
    publish_CarMessageObject.Nachrichtentyp = "Ambulance";

    reactionObject.message_type = std::string("Ambulance");
    reactionObject.position.x = subscribe_CarMessageObject.PositionX;
    reactionObject.position.y = subscribe_CarMessageObject.PositionY;
    reactionObject.source = subscribe_CarMessageObject.Teilnehmername;
    dst_car = subscribe_CarMessageObject.Teilnehmername;

    adhoc_to_cruise_publisher.publish(reactionObject);
    alreadyPublished = true;
    ROS_INFO("Just send a message to rescue another vehicle");

  };



  //adhoc_communication::sendMessage(rectangle, FRAME_DATA_TYPE_RECTANGLE, dst_car, "mambo_jambo");
  //adhoc_communication::sendMessage(current_pos, FRAME_DATA_TYPE_POSITION, dst_car, "traffic_light_position");
  adhoc_communication::sendMessage(publish_CarMessageObject, FRAME_DATA_TYPE_CAR2CAR, dst_car, "Car2Car");


  loop_rate.sleep();
  ros::spinOnce();
  
  };
  return 1;

}
