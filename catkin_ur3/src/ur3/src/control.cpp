#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "std_msgs/Float64MultiArray.h"
#include <stdio.h>

float vet_vel[6];

void vel_Callback(const sensor_msgs::JointState::ConstPtr &msg){
	vet_vel[0] = msg->velocity[0];
    vet_vel[1] = msg->velocity[1];
    vet_vel[2] = msg->velocity[2];
    vet_vel[3] = msg->velocity[3];
    vet_vel[4] = msg->velocity[4];
    vet_vel[5] = msg->velocity[5];
}


int main(int argc, char **argv){

vet_vel[0] = 0, vet_vel[1] = 0, vet_vel[2] = 0, vet_vel[3] = 0, vet_vel[4] = 0, vet_vel[5] = 0;

ros::init(argc, argv,"control");
ros::NodeHandle node_obj;
ros::Publisher float_pub;
ros::Subscriber float_sub;

float_pub = node_obj.advertise<std_msgs::Float64MultiArray>("/erro com ganho",10);
float_sub = node.subscribe("arm", 100, vel_Callback);

std_msgs::Float64MultiArray vel_control; 
vel_control.data.resize(6);

ros::Rate loop_rate(20);
int count_simple = 0;

// TODO
float kp = 1.0;
float ki = 0;
float kd = 0;

while (ros::ok()){

    
    msg_float.data[0] = vet_vel[0]*kp;
    msg_float.data[1] = vet_vel[1]*kp;
    msg_float.data[2] = vet_vel[2]*kp;
    msg_float.data[3] = vet_vel[3]*kp;
    msg_float.data[4] = vet_vel[4]*kp;
    msg_float.data[5] = vet_vel[5]*kp;
    
    float_pub.publish(msg_float);
    ros::spinOnce();
    loop_rate.sleep();
    ++count_simple;
}
return 0;
}