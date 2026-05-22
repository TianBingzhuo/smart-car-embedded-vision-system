#include "motor.h"
#include "filter.h"
#include "location.h"
#include "tracking.h"
#include "u660.h"
#include "encoder.h"
#include "car_move.h"
extern int set_speed;

void motor_init(void)//
{
	gpio_init(MOTOR1_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR1_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
    
    gpio_init(MOTOR2_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR2_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(MOTOR3_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR3_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(MOTOR4_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR4_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
	
}

void motor_set_speed4(int16 speed)														//50大约是2m/s
{
	if(speed>=0)
	{
		gpio_set_level(MOTOR4_DIR, GPIO_HIGH);                                         // DIR输出高电平
		pwm_set_duty(MOTOR4_PWM, speed);                   // 计算占空比 
	}
	else if(speed<0)
	{
		gpio_set_level(MOTOR4_DIR, GPIO_LOW);                                         // DIR输出低电平
		pwm_set_duty(MOTOR4_PWM, -speed);                   // 计算占空比 
	}
}
void motor_set_speed3(int16 speed)														//50大约是2m/s
{
	if(speed>=0)
	{
		gpio_set_level(MOTOR3_DIR, GPIO_HIGH);                                         // DIR输出高电平
		pwm_set_duty(MOTOR3_PWM, speed);                   // 计算占空比 
	}
	else if(speed<0)
	{
		gpio_set_level(MOTOR3_DIR, GPIO_LOW);                                         // DIR输出低电平
		pwm_set_duty(MOTOR3_PWM, -speed);                   // 计算占空比 
	}
}
void motor_set_speed2(int16 speed)														//50大约是2m/s
{
	if(speed>=0)
	{
		gpio_set_level(MOTOR2_DIR, GPIO_HIGH);                                         // DIR输出高电平
		pwm_set_duty(MOTOR2_PWM, speed);                   // 计算占空比 
	}
	else if(speed<0)
	{
		gpio_set_level(MOTOR2_DIR, GPIO_LOW);                                         // DIR输出低电平
		pwm_set_duty(MOTOR2_PWM, -speed);                   // 计算占空比 
	}
}
void motor_set_speed1(int16 speed)														//50大约是2m/s
{
	if(speed>=0)
	{
		gpio_set_level(MOTOR1_DIR, GPIO_HIGH);                                         // DIR输出高电平
		pwm_set_duty(MOTOR1_PWM, speed);                   // 计算占空比 
	}
	else if(speed<0)
	{
		gpio_set_level(MOTOR1_DIR, GPIO_LOW);                                         // DIR输出低电平
		pwm_set_duty(MOTOR1_PWM, -speed);                   // 计算占空比 
	}
}

/******************
函数功能:取绝对值
入口参数:int
返回值:int
******************/
int jueduizhi(int a)						//取绝对值
{
	int temp;
	if(a<0)  temp=-a;
	else temp=a;
	return temp;
}

int xianfu(int16 value,int16 max_val)			//限幅函数
{
	int temp;
	if(value>max_val) temp = max_val;
	else if(value<-max_val) temp = -max_val;
	else temp = value;
	return temp;
}

/******************
函数功能:增量式PID速度
入口参数:int
返回值:int
******************/
float pid1_speed_kp1=12.4;
float pid1_speed_ki1=0.5;
float pid1_speed_kd1=0.6;
int16 temp_encoder_counts1=0,temp_target_counts1=0;
int16 last_error1=0,l_last_error1=0;
int16 error1;
int16 target_speed1=0,target_speed2=0,target_speed3=0,target_speed4=0;

int pid1(double target_speed, double current_speed)
{
	static int16 control_speed1;

	temp_target_counts1=target_speed;
	temp_encoder_counts1=current_speed;
	error1=temp_target_counts1-temp_encoder_counts1;
	control_speed1+=(int16)(pid1_speed_kp1*(error1-last_error1) + pid1_speed_ki1*error1 + pid1_speed_kd1*(error1-2*l_last_error1+l_last_error1));

	l_last_error1=last_error1;
	last_error1=error1;

	
	return control_speed1; //返回速度控制值
}

float pid1_speed_kp2=12.4;
float pid1_speed_ki2=0.5;
float pid1_speed_kd2=0.6;
int16 temp_encoder_counts2=0,temp_target_counts2=0;
int16 last_error2=0,l_last_error2=0;
int16 error2;
int pid2(double target_speed, double current_speed)
{
	static int16 control_speed2;

	temp_target_counts2=target_speed;
	temp_encoder_counts2=current_speed;
	error2=temp_target_counts2-temp_encoder_counts2;
	control_speed2+=(int16)(pid1_speed_kp2*(error2-last_error2) + pid1_speed_ki2*error2 + pid1_speed_kd2*(error2-2*l_last_error2+l_last_error2));

	l_last_error2=last_error2;
	last_error2=error2;

	
	return control_speed2; //返回速度控制值
}

float pid1_speed_kp3=12.4;
float pid1_speed_ki3=0.5;
float pid1_speed_kd3=0.6;
int16 temp_encoder_counts3=0,temp_target_counts3=0;
int16 last_error3=0,l_last_error3=0;
int16 error3;
int pid3(double target_speed, double current_speed)
{
	static int16 control_speed3;
	temp_target_counts3=target_speed;
	temp_encoder_counts3=current_speed;
	error3=temp_target_counts3-temp_encoder_counts3;
	control_speed3+=(int16)(pid1_speed_kp3*(error3-last_error3) + pid1_speed_ki3*error3 + pid1_speed_kd3*(error3-2*l_last_error3+l_last_error3));
	l_last_error3=last_error3;
	last_error3=error3;

	
	return control_speed3; //返回速度控制值
}

float pid1_speed_kp4=12.4;
float pid1_speed_ki4=0.5;
float pid1_speed_kd4=0.6;
int16 temp_encoder_counts4=0,temp_target_counts4=0;
int16 last_error4=0,l_last_error4=0;
int16 error4;
int pid4(double target_speed, double current_speed)
{
	static int16 control_speed4;
	temp_target_counts4=target_speed;
	temp_encoder_counts4=current_speed;
	error4=temp_target_counts4-temp_encoder_counts4;
	control_speed4+=(int16)(pid1_speed_kp4*(error4-last_error4) + pid1_speed_ki4*error4 + pid1_speed_kd4*(error4-2*l_last_error4+l_last_error4));
	l_last_error4=last_error4;
	last_error4=error4;
	
	return control_speed4; //返回速度控制值
}

void car_go_straight(int16 speed)							//走直线，正速度向前走，负速度相反
{
	target_speed1=speed;
	target_speed2=speed;
	target_speed3=speed;
	target_speed4=speed;
}

void car_l_translation(int16 speed)							//左平移
{
	target_speed1=-speed;
	target_speed2=speed;
	target_speed3=speed;
	target_speed4=-speed;
}

void car_r_translation(int16 speed)							//右平移
{
	target_speed1=speed;
	target_speed2=-speed;
	target_speed3=-speed;
	target_speed4=speed;
}
void car_translation(int16 speed)							//正数为右平移，负数左平移
{
	target_speed1=speed;
	target_speed2=-speed;
	target_speed3=-speed;
	target_speed4=speed;
}
void car_diagonal(int num,int16 speed)						//num:1为左上角，2为右上角，3为左下角，4为右下角
{
	if(num==1)
	{
		target_speed1=0;
		target_speed2=speed;
		target_speed3=speed;
		target_speed4=0;
	}
	else if(num==2)
	{
		target_speed1=speed;
		target_speed2=0;
		target_speed3=0;
		target_speed4=speed;
	}
	else if(num==3)
	{
		target_speed1=-speed;
		target_speed2=0;
		target_speed3=0;
		target_speed4=-speed;
	}
	else if(num==4)
	{
		target_speed1=0;
		target_speed2=-speed;
		target_speed3=-speed;
		target_speed4=0;
	}
}

void car_turnround(int dir,int16 speed)						//0为顺时针，1为逆时针
{
	if(dir==0)
	{
		target_speed1=speed;
		target_speed2=-speed;
		target_speed3=speed;
		target_speed4=-speed;
	}
	else if(dir==1)
	{
		target_speed1=-speed;
		target_speed2=speed;
		target_speed3=-speed;
		target_speed4=speed;
	}
}
	
void car_stop(void)											//停车
{
	target_speed1=0;
	target_speed2=0;
	target_speed3=0;
	target_speed4=0;
	set_speed=0;
}
/******************
函数功能:小车运动设定距离函数(与小车复位函数配套使用)
入口参数:int16
返回值:void
备注：无
******************/

double dist_c_error;
double dist_l_error;
double t_dist=0;
double dist_kp=28.546;
double dist_kd=12.836;
int dist_pwm_out=0;
int set_go_straight_speed;
void car_move_dist(void)							//走直线，正速度向前走，负速度相反
{
	
	dist_c_error=(float)(t_dist - str_carmove_dist);//当前误差

	dist_pwm_out=dist_kp*dist_c_error+dist_kd*(dist_c_error-dist_l_error);//当前误差-上次误差
	dist_pwm_out=xianfu(dist_pwm_out,500);					//限幅
	dist_l_error=dist_c_error;
	
	set_go_straight_speed=(int)dist_pwm_out;
	car_go_straight(set_go_straight_speed);
	
	str_carmove_dist+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
}

extern int flag;							//测试小车各个运动模式

void car_mve_test_part(void)
{
//	if(flag==0)
//	{
////		storage_carmove();
//		car_go_straight(100);		
//	}
//	if(flag==0)
//	{
//		back_carmove();
//	}
//	if(flag==2)
//	{
//		car_go_straight(-set_speed);
//	}
//	if(flag==0)
//	{
//		car_l_translation(set_speed);
//	}
	if(flag==0)
	{
		car_r_translation(set_speed);
	}
//	if(flag==5)
//	{
//		car_diagonal(1,set_speed);
//	}
//	if(flag==6)
//	{
//		car_diagonal(2,set_speed);
//	}
//	if(flag==7)
//	{
//		car_diagonal(3,set_speed);
//	}
//	if(flag==8)
//	{
//		car_diagonal(4,set_speed);
//	}
//	if(flag==9)
//	{
//		car_turnround(0,250);
//	}
//	if(flag==10)
//	{
//		car_turnround(1,set_speed);
//	}
	if(flag==1)
	{
		car_stop();
	}
}















