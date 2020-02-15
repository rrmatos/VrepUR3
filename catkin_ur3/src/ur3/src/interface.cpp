// Quem olhar é gay 
// Se continuar lendo a mãe é puta

#include "ros/ros.h"
#include <boost/thread/thread.hpp>
#include "sensor_msgs/JointState.h"
#include "std_msgs/Float64MultiArray.h"
#include "read_data.h"
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
	ros::Publisher vel_pub = node.advertise<std_msgs::Float64MultiArray>("ref_vel",10);
	ros::Publisher startSimulation = node.advertise<std_msgs::Bool>("startSimulation",1);
	ros::Publisher stopSimulation = node.advertise<std_msgs::Bool>("stopSimulation",1);
	std_msgs::Bool startSim;
	startSim.data = true; 
	std_msgs::Bool stopSim;
	stopSim.data = true; 
	
	ros::Rate loop_rate1(20);
	while (ros::ok()){
		startSimulation.publish(startSim);
		x++;
		if(x > 10){break;}
		ros::spinOnce();
		loop_rate1.sleep();
		}
	///////////////////////////////////////////////////////////////////////////////////
	ros::Subscriber sub_joint_state = node.subscribe("arm2D", 10, jointState_Callback);
    ///////////////////////////////////////////////////////////////////////////////////////
	ros::Rate loop_rate(20);
	//Declaração das estruturas de dados para as publicações
	std_msgs::Float64MultiArray vel_arm; 
	vel_arm.data.resize(2);

	sensor_msgs::JointState arm;
	arm.header.frame_id = " ";
	arm.name.resize(2);
	arm.position.resize(2);
	arm.velocity.resize(2);
	arm.effort.resize(2); 
	arm.name[0] = "Joint_1";
	arm.name[1] = "Joint_2";
    
	float** data_input = read_data(); 
	int count_simple = 0;
	float w_mais1 = 0, w_menos1 = 0, alpha1 = 0;
	float w_mais2 = 0, w_menos2 = 0, alpha2 = 0;
	float tm1 = 0, tm2 = 0, I1 = 0.235, I2 = 0.24;

	//////////////////////////////////////////////////////////
	
	printf("UR3 is ready!\n");
	
    while (ros::ok()){

		arm.position[1] = joint2[0];
		arm.velocity[1] = joint2[1];
		
		w_mais2 = arm.velocity[1];
		alpha2 = (w_mais2 - w_menos2)/0.05;
		w_menos2 = w_mais2;

		tm2 = I2*alpha2 - joint2[2];
		arm.effort[1] = tm2;


		arm.position[0] = joint1[0];
		arm.velocity[0] = joint1[1];
		
		w_mais1 = arm.velocity[0];
		alpha1 = (w_mais1 - w_menos1)/0.05;
		w_menos1 = w_mais1;

		tm1 = I1*alpha1 - joint1[2];
		arm.effort[0] = tm1;

		vel_arm.data[0] = data_input[count_simple][0];//2*sin(count_simple/10.0);
		vel_arm.data[1] = data_input[count_simple][1];//-3*sin(count_simple/10.0);
		
		arm.header.stamp = ros::Time::now();

		arm_pub.publish(arm);
		vel_pub.publish(vel_arm);
		if(count_simple >= 2000){
			stopSimulation.publish(stopSim);
			break;
		}
		count_simple++;
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
} 

