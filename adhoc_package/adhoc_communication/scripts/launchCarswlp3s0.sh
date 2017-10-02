#!/bin/bash
sudo ifconfig wlp3s0 down 
 sudo iwconfig wlp3s0 mode ad-hoc
 sudo iwconfig wlp3s0 essid cars
 sudo iwconfig wlp3s0 ap fe:ed:de:ad:be:ef
sudo ifconfig wlp3s0 up
 rosrun adhoc_communication adhoc_communication_node
