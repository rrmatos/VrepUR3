
#include "ros/ros.h"
#include <boost/thread/thread.hpp>
#include "sensor_msgs/JointState.h"
#include "std_msgs/Bool.h"
#include <stdio.h>   
#include <string.h>  
#include <string> 
#include <stdlib.h>
#include <sstream>
#include <inttypes.h>
#include <unistd.h>

float joint1[3], joint2[3];
///////////////////////////////////////
void jointState_Callback(const sensor_msgs::JointState::ConstPtr &ur3){
	joint1[0] = ur3->position[0];
	joint1[1] = ur3->velocity[0];
	joint1[2] = ur3->effort[0];

	joint2[0] = ur3->position[1];
	joint2[1] = ur3->velocity[1];
	joint2[2] = ur3->effort[1];
	
}
///////////////////////////////////////
int main(int argc, char **argv){ 
	int x = 0;
	ros::init(argc, argv, "ur3");
	
	ros::NodeHandle node;
	//Declaração das publicões 
	ros::Publisher arm_pub = node.advertise<sensor_msgs::JointState>("arm",10);
	ros::Publisher startS = node.advertise<std_msgs::Bool>("startSimulation",1);
	ros::Publisher stopS = node.advertise<std_msgs::Bool>("stopSimulation",1);
	std_msgs::Bool startSim;
	startSim.data = true; 
	std_msgs::Bool stopSim;
	stopSim.data = true; 
	
	ros::Rate loop_rate1(10);
	while (ros::ok()){
		startS.publish(startSim);
		x++;
		if(x > 5){break;}
		ros::spinOnce();
		loop_rate1.sleep();
		}
	///////////////////////////////////////////////////////////////////////////////////
	ros::Subscriber sub_joint_state = node.subscribe("arm2D", 10, jointState_Callback);
    ///////////////////////////////////////////////////////////////////////////////////////
	ros::Rate loop_rate(20);
	//Declaração das estruturas de dados para as publicações
	sensor_msgs::JointState arm;
	arm.header.frame_id = " ";
	arm.name.resize(2);
	arm.position.resize(2);
	arm.velocity.resize(2);
	arm.effort.resize(2); 
	arm.name[0] = "Joint_1";
	arm.name[1] = "Joint_2";

	int count_simple = 0;
	//////////////////////////////////////////////////////////

	printf("UR3 is ready!\n");
	
    while (ros::ok()){
		arm.position[0] = joint1[0];
		arm.velocity[0] = joint1[1];
		arm.effort[0] = joint1[2];

		arm.position[1] = joint2[0];
		arm.velocity[1] = joint2[1];
		arm.effort[1] = joint2[2];
		
		arm.header.stamp = ros::Time::now();

		arm_pub.publish(arm);
		if(count_simple >= 500){
			stopS.publish(stopSim);
			break;
		}
		count_simple++;
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
} 

