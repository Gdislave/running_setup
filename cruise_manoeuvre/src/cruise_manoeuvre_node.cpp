#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

void switchGoal(move_base_msgs::MoveBaseGoal &goal,int &counter ,double array[9][7]){
    //Return from function if 4th goal has already been reached
    if(counter > 6)
    {counter = 3;};
    //Command the action server to stop tracking the former goal.
    //ac.stopTrackingGoal();
    //Write the new goal to the goal variable.
    //goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x    = array[counter][0];
    goal.target_pose.pose.position.y    = array[counter][1];
    goal.target_pose.pose.orientation.z = array[counter][5];
    goal.target_pose.pose.orientation.w = array[counter][6];
    ROS_INFO("just set %d goal", counter);
    counter++;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  double goalList[9][7] = {
    {-0.019, 8.449, 0, 0, 0, 0,  1.f},
    {11.0, 8.523,   0, 0, 0, 0,    1.000},
    {12.365, -0.937, 0, 0, 0, -0.696,  0.718},
    {13.935, -7.436,  0, 0, 0, 0.028, 1.000},//
    {26.915, -6.936, 0, 0, 0, 0.028,  1.000},
    {27.627, 1.355, 0, 0, 0, 0.735,  0.678},
    {14.0, 0.491, 0, 0, 0, 1.000,  -0.015},//
    {12.689, 5.691, 0, 0, 0, 0.722,  0.692},
    {1.066, 8.531, 0, 0, 0, 1.000,  -0.015},
  };

  int goal_tracker = 0;
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";//"was base_link

  while(ros::ok()){


  switchGoal(goal, goal_tracker, goalList);

  //ROS_INFO("Sending Goal [%d]!", goal_tracker);
  //Send goal to the action server and increment goal count by one.
  ac.sendGoal(goal);
  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved");
  else
    ROS_INFO("The base failed to move forward");

  }

  ros::spin();
  return 0;
}
