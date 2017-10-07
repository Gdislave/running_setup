#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include "std_msgs/String.h"
#include "scenario_handler/adhoc_reaction.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_listener.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

//Subscriber Callbacks
void adhoc_cmd_callback(const scenario_handler::adhoc_reaction::ConstPtr &adhoc_reaction_msg,
     scenario_handler::adhoc_reaction *adhoc_reaction)
  {
            *adhoc_reaction = *adhoc_reaction_msg;
            adhoc_reaction->message_type = adhoc_reaction_msg->message_type;
            adhoc_reaction->messagenumber = adhoc_reaction_msg->messagenumber;
            adhoc_reaction->position.x = adhoc_reaction_msg->position.x;
            adhoc_reaction->position.y = adhoc_reaction_msg->position.y;
            adhoc_reaction->position.z = adhoc_reaction_msg->position.z;
  }

void switchGoal(move_base_msgs::MoveBaseGoal &goal,int &counter ,double array_of_goals[9][7]){
    //Return from function if 4th goal has already been reached
    if(counter > 6)
    {counter = 3;};
    //Command the action server to stop tracking the former goal.
    //ac.stopTrackingGoal();
    //Write the new goal to the goal variable.
    //goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x    = array_of_goals[counter][0];
    goal.target_pose.pose.position.y    = array_of_goals[counter][1];
    goal.target_pose.pose.orientation.z = array_of_goals[counter][5];
    goal.target_pose.pose.orientation.w = array_of_goals[counter][6];
    ROS_INFO("just set %d goal", counter);
    counter++;
}

void setInitialPosition(geometry_msgs::PoseWithCovarianceStamped &pose_obj){
  pose_obj.header.stamp = ros::Time::now();
  pose_obj.header.frame_id = "map";
  pose_obj.pose.pose.position.x = -0.1;
  pose_obj.pose.pose.position.y = 8.449;
  pose_obj.pose.pose.position.z = 0.0;
  pose_obj.pose.pose.orientation.x = 0.0;
  pose_obj.pose.pose.orientation.y = 0.0;
  pose_obj.pose.pose.orientation.z = 0.0;
  pose_obj.pose.pose.orientation.w = 1.0;
}


int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh;
  ros::Rate loop_rate(40);
  scenario_handler::adhoc_reaction rct_obj;
  tf::TransformListener positionListener;
  double goalList[9][7] = {
    {-0.019, 8.449, 0, 0, 0, 0,  1.f},
    {11.0, 8.523,   0, 0, 0, 0,    1.000},
    {12.365, -0.937, 0, 0, 0, -0.696,  0.718},
    {14.535, -7.436,  0, 0, 0, 0.028, 1.000},//
    {26.915, -6.936, 0, 0, 0, 0.028,  1.000},
    {27.227, 0.855, 0, 0, 0, 0.735,  0.678},
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

  geometry_msgs::PoseWithCovarianceStamped initial_pose;
  ros::Publisher pose_pub =
  nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose", 10);



  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";//"was base_link
  ros::Subscriber reaction_sub = nh.
      subscribe<scenario_handler::adhoc_reaction>
      ("adhoc_rct_message", 10, std::bind(adhoc_cmd_callback, std::placeholders::_1, &rct_obj));

  //double weird_pub_counter = 0;


  ros::Duration(30,0).sleep();
  //eleganter über eine while(as long as the message im publishing is not initial pose ){keep on publishin}
  //need bool, is_published, and subscriber from intialpose and compare my pose with received.
  //initialpose needs some kind if time to be able to receive information. not really comprehensible
  setInitialPosition(initial_pose);
  pose_pub.publish(initial_pose);


//initialmode

  switchGoal(goal, goal_tracker, goalList);
  ac.sendGoal(goal);
  //pose_pub.publish(initial_pose);
  //if(weird_pub_counter <= 100){ pose_pub.publish(initial_pose); weird_pub_counter++;};
  bool parameterSemaphore = false, RSU = false, speedLimit = false, approach1 = false, approach2 = false;
  std::string tag("Road3to4");
  std::string tagToFlag("empty");
  float velocityParamter = 0, velocityBuffer = 0, speedLimitValue = 1;
  double xPos,yPos, distanceToTransmitter1, distanceToTransmitter2, rec1Px, rec1Py, rec2Px, rec2Py;
  int timedelta1 = 0, timedelta2=0, timedelta3=0, timedelta4=0;
  int distanceBuffer1 = 0, distanceBuffer2 = 0;


while(ros::ok()){


  //Testszenario: die RSU ist eine Geschwindigkeitsbegrenzun->speedLimit
  //Die Einheit befindet sich bei Position,
  // /*
  rec1Px = 12.5, rec1Py = -1;
  rec2Px = 15, rec2Py = -7.5;

  positionListener.waitForTransform("/map", "/base_footprint", ros::Time(0), ros::Duration(10.0));
  tf::StampedTransform transformTest;
  try
   {
     positionListener.lookupTransform("/map", "/base_footprint",
                               ros::Time(0), transformTest);
     xPos = transformTest.getOrigin().x();
     yPos = transformTest.getOrigin().y();
     //seems to get the right information
     //ROS_INFO("Current position: (%f, %f, )\n", xPos,yPos);
   }
   catch(tf::TransformException &ex)
   {
     ROS_ERROR("%s", ex.what());
     ros::Duration(1.0).sleep();
   }
  distanceToTransmitter1 = sqrt(pow((rec1Px - xPos),2)
                            + pow((rec1Py - yPos),2));
  if(distanceToTransmitter1 < 10)
        {if(!RSU)
             {RSU= true;
              ROS_INFO("Receiving messages from RSU");
              speedLimit = true;
              }
        }

  else  {if(RSU)
             {RSU=false;
              ROS_INFO("Do not Receive messages from RSU");
              speedLimit = false;
              }
        }



  //ROS_INFO("Distance to Transmitter: [%f]", distanceToTransmitter1);
  // */



  //Pseudo Prosa-Code
  //Aufpassen ob ein Signal von einer RSU kommt
  //Falls ja entscheiden was für eins -> Hier Geschwindigkeitsbegrenzung, Ampel
  //Position von RSU ermitteln und Distanz berechnen
  //Entscheiden ob man innerhalb des Radius der Beschränkung ist, schon reingekommen
  //Falls JA
  //Geschwindigkeit auf Reduktion setzen, weiterfahren
  //Falls Nein
  //Keine Änderung -> weiterfahren
  //Falls Nein aber Bool gesetzt -> bool auf false setzen


  //Code um Bools zu setzen, ob das Fahrzeug sich auf die RSUs bewegt oder weg davon.











  //RSU-Handling , approach of speed limiter
  /*
  if(RSU){
  //Falls ein Signal von einer RSU bekommen, eigene Position bestimmen. Gespeichert in positionListener
  positionListener.waitForTransform("/map", "/base_footprint", ros::Time(0), ros::Duration(10.0));
  tf::StampedTransform transform;
  try
   {
     positionListener.lookupTransform("/map", "/base_footprint", ros::Time(0), transform);
     xPos = transform.getOrigin().x();
     yPos = transform.getOrigin().y();
     //seems to get the right information
     //ROS_INFO("Current position: (%f, %f, )\n", xPos,yPos);
   }
  catch(tf::TransformException &ex)
   {
     ROS_ERROR("%s", ex.what());
     ros::Duration(1.0).sleep();
   }
  //Berechnen der Distanz zur RSU-Einheit
  distanceToTransmitter1 = sqrt(pow((rec1Px - xPos),2)
                            + pow((rec1Py - yPos),2));
  //ROS_INFO("Distance to Transmitter: [%f]", distanceToTransmitter1);


  if(speedLimit && (distanceToTransmitter1 < 1) && (!parameterSemaphore) && (tagToFlag == "emtpy"))
  {
     nh.getParam("/move_base/local_costmap/TebLocalPlannerROS/max_vel_x",velocityParamter);
     velocityBuffer = velocityParamter;
     nh.setParam("/move_base/local_costmap/TebLocalPlannerROS/max_vel_x",speedLimitValue);
     tagToFlag = tag;

     //nh.getParam("/move_base/local_costmap/TebLocalPlannerROS/max_vel_x",speedLimitValue);
     //ROS_INFO("set velocity auf %f", speedLimitValue);

        //vielleicht ein Akku
        parameterSemaphore = true;
  }


  if(speedLimit && !(distanceToTransmitter1 <2) && parameterSemaphore &&)
  {
     nh.setParam("/move_base/local_costmap/TebLocalPlannerROS/max_vel_x",velocityBuffer);
     velocityBuffer = 0;



     //nh.getParam("/move_base/local_costmap/TebLocalPlannerROS/max_vel_x",velocityBuffer);
     //ROS_INFO("set velocity auf %f", velocityBuffer);
     //velocityBuffer = 0;


     parameterSemaphore = false;
     //Akku delete
  }

  }//RSU Klammer
  */







  //ROS_INFO("Sending Goal [%d]!", goal_tracker);
  //Send goal to the action server and increment goal count by one.

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ac.sendGoal(goal);
    switchGoal(goal, goal_tracker, goalList);
  }
  //ac.waitForResult(); selfmade including check for new commands from publisher
  if(rct_obj.message_type == "SOS")
  {
    ac.cancelAllGoals();
    //Setting emergency goal
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = rct_obj.position.x;
    goal.target_pose.pose.position.y = rct_obj.position.y;
    ac.sendGoal(goal);
    ROS_INFO("received emergencycall from %s and heading its direction , every other command will be ignored :)", rct_obj.source.c_str());
    ac.waitForResult();
  }
  ros::spinOnce();

  /*
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved");
  else
    ROS_INFO("The base failed to move forward");
  */
  }//While(ROS::OK)Klammer
 //}
  return 0;
}

