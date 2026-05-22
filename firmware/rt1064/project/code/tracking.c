#include "tracking.h"
#include "motor.h"
#include "location.h"
#include "encoder.h"
#include "isr.h"
#include "u660.h"
#include "car_move.h"



double targeta,targetb,targetc,targetd;
/*
VABCD轮-> 麦轮A、B、C、D 的线速度，单位m/s。
Vx-> 机器人前后移动速度，前进为正，单位：m/s。
Vy-> 机器人左右移动速度，左移为正，单位：m/s。
Vz-> 机器人绕 O 点旋转速度，逆时针为正，单位：rad/s
W-> 轮距，机器人左右麦轮的距离，单位：m。
H-> 轴距，机器人前后麦轮的距离，单位：m。
*/

void move_transfrom(double vx,double vy,double vz)
{
	targeta=vx+vy-vz*0.8;			//对应motor3
	targetb=vx-vy-vz*0.8;			//对应motor1
	targetc=vx+vy+vz*0.8;			//对应motor2
	targetd=vx-vy+vz*0.8;			//对应motor4
}


 

/******************
函数功能:直行循迹，偏航纠正函数
入口参数:int16
返回值:void
******************/
extern int16 set_speed;

void tr_straight_move(int16_t v_error)
{
	v_error=(double)v_error/38*90;//-38~38映射-90~90

	double target_angle=(double)v_error/360*2*PI;//角度转弧度
	double target_speed_x=set_speed*cos(target_angle);//x方向目标速度	
	double target_speed_y=set_speed*sin(-target_angle);//y方向目标速度
	
	move_transfrom(target_speed_x,0.28*target_speed_y,0.75*target_speed_y);//转换为每个电机的速度
	//利用wifi调参
//	move_transfrom(target_speed_x,seekfree_assistant_parameter[0]*target_speed_y,seekfree_assistant_parameter[1]*target_speed_y);//转换为每个电机的速度
	
	target_speed1=targetb;
	target_speed2=targetc;
	target_speed3=targeta;								//更新目标速度
	target_speed4=targetd;
}
 
/******************
函数功能:转特定角度，偏航纠正函数
入口参数:void
返回值:void
******************/

extern int16 set_turnround_speed;

void tr_turnround_move(void)
{
	angle_pd1(target_angle,angleZ);
	set_turnround_speed=(int)c_pwm_out;
	if(set_turnround_speed>=0)
	{
		car_turnround(1,jueduizhi(set_turnround_speed));
	}
	if(set_turnround_speed<0)
	{
		car_turnround(0,jueduizhi(set_turnround_speed));
	}
	
}

/******************
函数功能:记录小车运动函数
入口参数:void
返回值:void
备注：该函数放在了停车线函数的后面所以，不再调用encoder_get_val();distance_cal();

******************/

float str_carmove_dist=0;
int str_carmove_angle=0;
int move_test_flag=0;

void storage_carmove(void)
{
	str_carmove_dist+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
	str_carmove_angle=angleZ;
}

/******************
函数功能:小车归位运动函数
入口参数:void
返回值:void
备注：无
******************/

void back_carmove(void)
{
	static int back_carmove_flag=0;
	if(back_carmove_flag==0)
	{
		car_move_dist();
		if(str_carmove_dist>=-0.15&&str_carmove_dist<=0.15)
		{
			back_carmove_flag=1;
		}
	}
	if(back_carmove_flag==1)
	{
		tr_turnround_move();
	}
}
