#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ros/topic.h"
/*
#include "adhoc_customize/include.h"
#include "adhoc_communication/functions.h"
#include "adhoc_communication/SendString.h"
#include "adhoc_communication/RecvString.h"
*/

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  //adhoc_communication::RecvString

  ros::init(argc, argv, "adhoc_listener");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("traffic_light_position", 1000, chatterCallback);

  ros::spin();

  return 0;
}
