#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <stdio.h>

int main(int argc, char **argv)
{
ros::init(argc, argv,"send_vel");

ros::NodeHandle node_obj;

ros::Publisher float_pub;

float_pub = node_obj.advertise<std_msgs::Float64>("/vel_float",10);

std_msgs::Float64 msg_float;

ros::Rate loop_rate(20);

int count_simple = 0;

while (ros::ok()){

    // msg_float.data = sin(count_simple/20.0);
    msg_float.data = 0;
    // ROS_INFO("%d",msg.data);
    float_pub.publish(msg_float);
    ros::spinOnce();
    loop_rate.sleep();
    ++count_simple;
}
return 0;
}