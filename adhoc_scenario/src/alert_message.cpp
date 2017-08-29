#include "ros/ros.h"
#include "ros/topic.h"
#include "adhoc_customize/include.h"
#include "adhoc_customize/defines.h"
#include "adhoc_communication/functions.h"
#include "adhoc_communication/SendString.h"
#include "adhoc_communication/RecvString.h"
#include <adhoc_tests/FilenameService.h>
#include "adhoc_communication/MmRobotPosition.h"
#include "nav_msgs/Odometry.h"
#include <iostream> 
#include <fstream>
#include <string> 
#include <sys/time.h>
#include <tf/transform_listener.h>


//adhoc_customize::Rectangle rectangle;
adhoc_communication::MmRobotPosition current_pos;
//const nav_msgs::Odometry::ConstPtr& msg;
//nav_msgs::Odometry odom;
//nav_msgs::Odometry::
//Callback functions
/*

void odom_callback(const nav_msgs::Odometry::ConstPtr& odom_msg, nav_msgs::Odometry* odom){
        *odom = *odom_msg;
}

*/

int main (int argc, char **argv){
	
	ros::init(argc, argv, "alert_message");
  ros::NodeHandle nh;

  tf::TransformListener listener;

	// get Parameters and print INFO
	int loop;
	std::string dst_car, src_car;
	dst_car = "pses-car6";
	src_car = "pses-car2";
	loop = 10;
	/*nh.getParam("/adhoc_scenario_params/dst_car", dst_car);
	nh.getParam("/adhoc_scenario_params/loop", loop);
	nh.getParam("/adhoc_scenario_params/src_car", src_car); */
	ROS_INFO("loop [%d]; Dest: [%s] ; Source: [%s]", loop, dst_car.c_str(), src_car.c_str());
	current_pos.src_robot = src_car; 
	
  ros::Rate loop_rate(10);


	int i = 0;

  tf::StampedTransform transform;
  geometry_msgs::Quaternion quaternion;

	while(ros::ok() && i<loop){
		i++;

    try
        {
    listener.lookupTransform("/map","/base_footprint",ros::Time(0), transform);
    ROS_INFO("Got a transform! x = %f, y = %f, z = %f",transform.getOrigin().x(),transform.getOrigin().y(), transform.getOrigin().z());
    current_pos.position.pose.position.x = transform.getOrigin().x();
    current_pos.position.pose.position.y = transform.getOrigin().y();
    current_pos.position.pose.position.z = transform.getOrigin().z();
    tf::quaternionTFToMsg(transform.getRotation(), quaternion);
    current_pos.position.pose.orientation = quaternion;
    adhoc_communication::sendMessage(current_pos, FRAME_DATA_TYPE_POSITION, dst_car, "traffic_light_position");

    }
    catch (tf::TransformException ex)
    {
          ROS_ERROR("Nope! %s", ex.what());
    }
		// send Rectangle
    //rectangle.length = i;
    //rectangle.width = i;
    //current_pos.position.pose.position.x = odom.pose.pose.position.x;
    //adhoc_communication::sendMessage(rectangle, FRAME_DATA_TYPE_RECTANGLE, dst_car, "t_rectangle");
   // ROS_INFO("position [%d]", odom.pose.pose.position.x);

    loop_rate.sleep();
	}	
	return 1;
}
