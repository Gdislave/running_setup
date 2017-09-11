#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ros/topic.h"
//#include "adhoc_customize/include.h"
#include "adhoc_communication/functions.h"
#include "adhoc_communication/SendString.h"
#include "adhoc_communication/RecvString.h"
#include "adhoc_communication/MmRobotPosition.h"
#include <functional>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <unordered_map>
#include <tf/transform_listener.h>
#include <iostream>
#include "scenario_handler/adhoc_reaction.h"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

//Subscriber Callbacks
/*
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}
*/

//Subscriber Callbacks
void traffic_light_positionCallback(const adhoc_communication::MmRobotPosition::ConstPtr& tfl_pos_ptr, std::unordered_map<std::string, geometry_msgs::Pose>* pose_map)
{
	(*pose_map)[tfl_pos_ptr->src_robot].orientation = tfl_pos_ptr->position.pose.orientation;
	(*pose_map)[tfl_pos_ptr->src_robot].position = tfl_pos_ptr->position.pose.position;
	//std::cout << tfl_pos_ptr->position.pose.orientation.x << ": " << tfl_pos_ptr->position.pose.orientation.y<< " : " << tfl_pos_ptr->position.pose.orientation.z<< " : " << tfl_pos_ptr->position.pose.orientation.w << std::endl;

	ROS_INFO("Updated Map Content: ");
	for (auto& x: *pose_map) {
    	
    	std::cout << x.first << ": " << x.second.position.x << ": " << x.second.position.y << ": " << x.second.position.z << ": " << std::endl;
  	}

}

/*
void traffic_light_positionCallback(const adhoc_communication::MmRobotPosition::ConstPtr& tfl_pos_ptr)
{
	ROS_INFO("Old fashioned way via arrow operator: Position-> x: [%f], y: [%f], z: [%f]", 
		tfl_pos_ptr->position.pose.position.x, 
		tfl_pos_ptr->position.pose.position.y, 
		tfl_pos_ptr->position.pose.position.z);
	
	wie bekomme ich einen Pointer in der callback-Funktion übergeben?
	quaternion[0] = tfl_pos_ptr->position.pose.position.x;
	quaternion[1] = tfl_pos_ptr->position.pose.position.y;
	quaternion[2] = tfl_pos_ptr->position.pose.position.z;
	
	//ROS_INFO("Position via asterisk,dereference *,  I heard: [%s]", tfl_pos_obj.position.pose.position.x);
}
*/
bool rdy_for_pub(int &a, int &b){
  return true;
}

int main(int argc, char **argv)
{

  //adhoc_communication::RecvString

  ros::init(argc, argv, "adhoc_listener");
  ros::NodeHandle nh;
  adhoc_communication::MmRobotPosition tfl_pos_obj;
  scenario_handler::adhoc_reaction test_object;
  ////ros::Publisher commands_pub = nh.advertise<std_msgs::String>("adhoc_message_commands", 10);
  ros::Publisher adhoc_rct_pub = nh.advertise<scenario_handler::adhoc_reaction>("adhoc_rct_message",10);
  //geometry_msgs::Quaternion quaternion;
  //int quaternion[4] = {0,0,0,0};
  std::unordered_map<std::string, geometry_msgs::Pose> pose_map;
  //init values for delta calculation
  double time_a = ros::Time::now().toSec(), time_b, delta;
  delta = time_b = time_a;
  //time_a = ;

  MoveBaseClient ac("move_base", true);
  while(!ac.waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  ros::Subscriber tfp_sub = nh.subscribe<adhoc_communication::MmRobotPosition>("traffic_light_position", 10, std::bind(traffic_light_positionCallback, std::placeholders::_1, &pose_map));
  ros::Rate loop_rate(10);
  ////std_msgs::String msg_adhoc_command;
  std::stringstream ss;
  ss << "pses_car2";
  ////msg_adhoc_command.data = ss.str();
  test_object.source = ss.str();
  test_object.message_type = "SOS";
  test_object.position.x = 12.5;
  test_object.position.y = -3.5;
  test_object.position.z = 0;
  test_object.messagenumber = 0;


   while(ros::ok()) {

   	//for (auto& x: pose_map) {
    //std::cout << x.first << ": " << x.second.position.x << std::endl;
  	//}

    if((time_b - time_a) > 1){
      ROS_INFO("a second has passed");
      time_a = ros::Time::now().toSec();
      ////commands_pub.publish(msg_adhoc_command);
      ////ROS_INFO("just published the resue message for car : %s", msg_adhoc_command.data.c_str());
      adhoc_rct_pub.publish(test_object);
    }
    time_b = ros::Time::now().toSec();

    //
    /*
     * THIS IS JUST COMMENTED BECAUSE OF MESSAGE TESTING
     *
  	if ( pose_map.find("pses-car2") == pose_map.end())
      {std::cout << "not found" << std::endl;}
  	else {
  		goal.target_pose.header.frame_id = "map";
  		goal.target_pose.pose = pose_map["pses-car2"];
  		ac.sendGoal(goal);
  		ROS_INFO("Sending goal");
		ac.waitForResult();

    }
    */

  //	
	
/*
	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the base moved 1 meter forward");
	else
		ROS_INFO("The base failed to move forward 1 meter for some reason");
*/

  	ros::spinOnce();
  	loop_rate.sleep();

   }
  //ros::Subscriber tfp_sub = nh.subscribe<adhoc_communication::MmRobotPosition>("traffic_light_position", 50, traffic_light_positionCallback);
  //ROS_INFO("TRY_OUT Position-> x: [%f]", tfl_pos_ptr->position.pose.position.x);

/*
  goal.target_pose.header.frame_id = "base_footprint";//"was base_link 
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = 10.0;
  goal.target_pose.pose.position.y = 1.f;
  goal.target_pose.pose.position.z = 0.f;
  goal.target_pose.pose.orientation.x = 0.f;
  goal.target_pose.pose.orientation.y = 0.f;
  goal.target_pose.pose.orientation.z = 0.f;
  goal.target_pose.pose.orientation.w = 0.f;


  ROS_INFO("Sending goal");
	ac.sendGoal(goal);

	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the base moved 1 meter forward");
	else
		ROS_INFO("The base failed to move forward 1 meter for some reason");
*/

  return 0;
}



//ros::Subscriber cmd_vel_sub = nstack_ctrl.subscribe<geometry_msgs::Twist>("cmd_vel", 10, std::bind(cmd_velCallback, std::placeholders::_1, &cmd_vel));
