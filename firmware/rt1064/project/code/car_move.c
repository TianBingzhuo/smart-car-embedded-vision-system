#include "zf_common_headfile.h"
#include "motor.h"
#include "location.h"
#include "encoder.h"
#include "tracking.h"
#include "camera.h"
#include "steer.h"
#include "ips200.h"
#include "Fuzzy.h"
#include "key.h"
#include "smotor.h"
#include "wifi.h"
#include "beep.h"
#include "u660.h"
#include "car_move.h"
#include "wirles_usart.h"
#include "wir_usart.h"
#include "math.h"
#include "box_logic_code.h"


/******************
函数功能:小车运动设定距离函数(y轴)
入口参数:int16
返回值:void
备注：无
******************/
int enter_circular_flag=0,dir_cir_flag=0;//进入圆环标志位，左右圆环标志位

double dist_c_s_error;
double dist_l_s_error;
double dist_s_kp=80.546;
//double dist_ki=0;
double dist_s_kd=60.836;
//double dist_error_cum=0;
int dist_s_pwm_out=0;
int set_go_s_straight_speed;
double move_s_dist_c=0;

int move_spe_dist_finish_flag=0;

void car_move_spe_dist(double dist)							//走直线，正速度向前走，负速度相反
{

	
	dist_c_s_error=(float)(dist - move_s_dist_c);//当前误差

	dist_s_pwm_out=dist_s_kp*dist_c_s_error+dist_s_kd*(dist_c_s_error-dist_l_s_error);//当前误差-上次误差
	dist_s_pwm_out=xianfu(dist_s_pwm_out,500);					//限幅
	if(dist_s_pwm_out<20&&dist_s_pwm_out>=0){dist_s_pwm_out=15;}
	if(dist_s_pwm_out>=-20&&dist_s_pwm_out<0){dist_s_pwm_out=-15;}
	dist_l_s_error=dist_c_s_error;
	
	set_go_s_straight_speed=(int)dist_s_pwm_out;
	car_go_straight(set_go_s_straight_speed);
	
	move_s_dist_c+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
	if((dist-move_s_dist_c)<=0.03&&(dist-move_s_dist_c)>=-0.03){move_spe_dist_finish_flag=1;}//返回完成标志位
}

/******************
函数功能:小车运动设定距离函数(x轴)
入口参数:int16
返回值:void
备注：无
******************/
double dist_c_s_error_x;
double dist_l_s_error_x;
double dist_s_kp_x=80.546;
//double dist_ki=0;
double dist_s_kd_x=60.836;
//double dist_error_cum=0;
int dist_s_pwm_out_x=0;
int set_go_s_straight_speed_x;
double move_s_dist_c_x=0;

int move_spe_dist_finish_flag_x=0;

void car_move_spe_dist_x(double dist)							
{

	
	dist_c_s_error_x=(float)(dist - move_s_dist_c_x);//当前误差

	dist_s_pwm_out_x=dist_s_kp_x*dist_c_s_error_x+dist_s_kd_x*(dist_c_s_error_x-dist_l_s_error_x);//当前误差-上次误差
	dist_s_pwm_out_x=xianfu(dist_s_pwm_out_x,500);					//限幅
	if(dist_s_pwm_out_x<20&&dist_s_pwm_out_x>=0){dist_s_pwm_out_x=15;}
	if(dist_s_pwm_out_x>=-20&&dist_s_pwm_out_x<0){dist_s_pwm_out_x=-15;}
	dist_l_s_error_x=dist_c_s_error_x;
	
	set_go_s_straight_speed_x=(int)dist_s_pwm_out_x;
	car_translation(set_go_s_straight_speed_x);
	
	move_s_dist_c_x+=(wheel_distance1+wheel_distance4)/2.0;
	if((dist-move_s_dist_c_x)<=0.03&&(dist-move_s_dist_c_x)>=-0.03){move_spe_dist_finish_flag_x=1;}//返回完成标志位
}
/******************
函数功能:圆环循迹
入口参数:int16
返回值:void
******************/

double kx=0;//0.836
double ky=1.248;//2.036
double kz=4.956;//2.536
int set_cir_y_speed;

extern int16 set_speed;


void tr_circal_move(int16_t x_error,int16_t z_error)
{
	//判断左右圆环
	if(dir_cir_flag==1){set_cir_y_speed=35;}
	if(dir_cir_flag==2){set_cir_y_speed=-30;}
	
	double target_speed_x;//x方向目标速度	
	x_error-=20;
	if(x_error>=0)
	{
		if(x_error>10){target_speed_x=8;}
		if(x_error>5&&x_error<=10){target_speed_x=5;}
		if(x_error>2&&x_error<=5){target_speed_x=3;}
		if(x_error<=2){target_speed_x=0;}
	}
	if(x_error<0)
	{
		if(x_error<-10){target_speed_x=-8;}
		if(x_error<-5&&x_error>=-10){target_speed_x=-5;}
		if(x_error<-2&&x_error>=-5){target_speed_x=-3;}
		if(x_error>=-2){target_speed_x=0;}
	}
	
	double target_speed_y=ky*set_cir_y_speed;//y方向目标速度
	double target_speed_z=kz*z_error;//z方向目标速度
	target_speed_y=xianfu(target_speed_y,50);
	target_speed_z=xianfu(target_speed_z,50);
	move_transfrom(0,target_speed_y,target_speed_z);//转换为每个电机的速度
	
	target_speed1=targetb+target_speed_x;
	target_speed2=targetc+target_speed_x;
	target_speed3=targeta+target_speed_x;								//更新目标速度
	target_speed4=targetd+target_speed_x;
}

/******************
函数功能:面对卡片循迹
入口参数:int16
返回值:void
******************/

double op_kx=5.836;//0.836
double op_ky=1.248;//2.036
double op_kz=4.956;//2.536

int set_op_y_speed;

extern int16 set_speed;

void op_find_card_move(int16_t x_error,int16_t z_error,int temp_op_turn_dir)
{
	//判断左右圆环
	if(temp_op_turn_dir==1){set_op_y_speed=50;}
	if(temp_op_turn_dir==2){set_op_y_speed=-50;}
	x_error-=20;
	double target_speed_x;//x方向目标速度	
	if(x_error>=0)
	{
		if(x_error>10){target_speed_x=8;}
		if(x_error>5&&x_error<=10){target_speed_x=5;}
		if(x_error>2&&x_error<=5){target_speed_x=3;}
		if(x_error<=2){target_speed_x=0;}
	}
	if(x_error<0)
	{
		if(x_error<-10){target_speed_x=-8;}
		if(x_error<-5&&x_error>=-10){target_speed_x=-5;}
		if(x_error<-2&&x_error>=-5){target_speed_x=-3;}
		if(x_error>=-2){target_speed_x=0;}
	}
	
	double target_speed_y=ky*set_op_y_speed;//y方向目标速度
	double target_speed_z=kz*z_error;//z方向目标速度
	target_speed_y=xianfu(target_speed_y,50);
	target_speed_z=xianfu(target_speed_z,50);
	move_transfrom(0,target_speed_y,target_speed_z);//转换为每个电机的速度
	//if(x_error>)
//	target_speed1=targetb;
//	target_speed2=targetc;
//	target_speed3=targeta;								//更新目标速度
//	target_speed4=targetd;
	target_speed1=targetb+target_speed_x;
	target_speed2=targetc+target_speed_x;
	target_speed3=targeta+target_speed_x;								//更新目标速度
	target_speed4=targetd+target_speed_x;
}
/******************
函数功能:小车运动在圆环的逻辑代码函数
入口参数:int16
返回值:void
备注：无
******************/
uint8_t f_cir_step_flag=0;
int catch_card_flag=0;			//拾取卡片标志位
int put_card_flag=0;            //放下卡片标志位
int somtor_c_finish_flag=0;     //舵机拾取成功标志位
int somtor_p_finish_flag=0;     //舵机放下成功标志位
int cir_tramove_start_flag=0;   //小车循迹开始标志位
								//
int cir_move_x_error=0;         //x方向error
int cir_move_z_error=0;         //y方向error
								//
int out_circal_flag=0;          //出圆环标志位

int temp_str_angleZ;            //暂时储存偏航角变量

double temp_cir_disc;

uint8_t have_card_flag;			//检测到卡片标志位

//int u660_init_flag=1;

extern int card_out_flag_left,card_out_flag_right;
extern int turn_flag;
int temp_stop_counts=0;
void op_go_for_card_gam(void);

void carmove_in_circular(void)
{
	if(enter_circular_flag==1)//当进入标志位为一时
	{
		/*
			该段用于利用art，将小车对准卡片堆
			大概思路为，小车转过角度后利用art，不断靠近并且对卡片堆校准。利用小车的平移或者45度角斜平移
		*/
		/**************
		现在假设已经校准
		**************/
		//判断左右圆环
		if(ImageFlag.image_element_rings==1){dir_cir_flag=1;}//左圆环
		if(ImageFlag.image_element_rings==2){dir_cir_flag=2;}//右圆环
		//如果是左圆环
		if(dir_cir_flag==1)
		{
			if(f_cir_step_flag==0)
			{
				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cir_disc>0.115)
				{
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					f_cir_step_flag=1;
					temp_cir_disc=0;
				}
			}
			//开始转向，面朝卡片堆
			if(f_cir_step_flag==1)
			{
				turn_s_angle(temp_str_angleZ+90);
				if(turn_s_ang_finish_flag==1)
				{
					/*
					这里检测是否有卡片堆
					*/
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					car_stop();
					turn_s_ang_finish_flag=0;
					f_cir_step_flag=2;
				}
			}
			
			if(f_cir_step_flag==2)
			{
				temp_stop_counts++;
				temp_str_angleZ=anglez_trsl_in_cir(angleZ);
				temp_op_anglez=anglez_trsl_in_cir(angleZ);
				if(temp_stop_counts>=10)
				{
					temp_stop_counts=10;
					if(out_card_flag==0){f_cir_step_flag=3;temp_stop_counts=0;}
					else if(out_card_flag==1){f_cir_step_flag=11;temp_stop_counts=0;}
				}
//				if(out_card_flag==1&&have_card_flag==0){f_cir_step_flag=11;}
//				if(out_card_flag==0&&have_card_flag==1){f_cir_step_flag=3;}
			}
			if(f_cir_step_flag==3)
			{
				op_go_for_card_gam();//该函数里面使用的角度为temp_op_anglez，而不是temp_str_angleZ，所以这里容易出bug
				/*这里要切换为第二个art来识别*/
				if(out_card_flag==1&&smotor_catch_finish_flag==1){f_cir_step_flag=4;smotor_catch_finish_flag=0;}
			}
			if(f_cir_step_flag==4)
			{
				art_catch_card_convert_speed(temp_op_anglez,50,0);
				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cir_disc>0.04)
				{
					car_stop();
					f_cir_step_flag=5;
					temp_cir_disc=0;
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
				}
			}
			if(f_cir_step_flag==5)
			{
				turn_s_angle(temp_str_angleZ-90);
				if(turn_s_ang_finish_flag==1)
				{
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					turn_s_ang_finish_flag=0;
					f_cir_step_flag=6;
				}
			}
			if(f_cir_step_flag==6)
			{
				art_catch_card_convert_speed(temp_op_anglez,50,0);
				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cir_disc>0.04)
				{
					car_stop();
					f_cir_step_flag=7;
					temp_cir_disc=0;
				}
			}
			if(f_cir_step_flag==7)
			{
				if(border_line_left>50){card_out_flag_left=1;}//判断是否该出圆环
				cir_move_x_error=(border_line_left+border_line_right)/2;  //计算误差
				cir_move_z_error=border_line_left-border_line_right;      //计算误差
				tr_circal_move(cir_move_x_error,-cir_move_z_error);//小车循迹圆环函数
				if(card_out_flag_left==1)//当出环标志位为1时
				{
					
					car_stop();//小车停止为下一步出环做准备
					f_cir_step_flag=8;
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
				}
			}
			if(f_cir_step_flag==8)
			{
				turn_s_angle(temp_str_angleZ+110);//转向跑道为正常循迹做准备
				if(turn_s_ang_finish_flag==1){turn_s_ang_finish_flag=0;f_cir_step_flag=9;move_s_dist_c=0;}//当转向完成时
			}
//			if(f_cir_step_flag==9)
//			{
//				art_catch_card_convert_speed(temp_op_anglez,50,0);
//				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
//				if(temp_cir_disc>0.04)
//				{
//					car_stop();
//					f_cir_step_flag=10;
//					temp_cir_disc=0;
//				}
//			}
			//正常循迹开始，相关圆环循迹变量与标志位回归初始化
			if(f_cir_step_flag==9)
			{					
				ImageFlag.image_element_rings_flag=8;
				temp_str_angleZ=0;
				temp_op_anglez=0;
				turn_s_ang_finish_flag=0;
				enter_circular_flag=0;
				dir_cir_flag=0;
				card_out_flag_left=0;
				move_spe_dist_finish_flag=0;
				turn_flag=0;
				set_speed=CAR_SPEED;//由于之前修改过循迹的速度，所以要重新设定速度
				move_s_dist_c=0;
				f_cir_step_flag=0;
				car_move_mode=0;
				have_card_flag=0;
			}
			if(f_cir_step_flag==11)
			{
				turn_s_angle(temp_str_angleZ-90);
				if(turn_s_ang_finish_flag==1){turn_s_ang_finish_flag=0;f_cir_step_flag=9;move_s_dist_c=0;}//当转向完成时
			}
		}
		/*
		相关原理类似，只有正负号修改，与相关变量修改（dir_cir_flag==2，card_out_flag_right，border_line_right）
		*/
		if(dir_cir_flag==2)
		{
			if(f_cir_step_flag==0)
			{
				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cir_disc>0.115)
				{
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					f_cir_step_flag=1;
					temp_cir_disc=0;
				}
			}
			//开始转向，面朝卡片堆
			if(f_cir_step_flag==1)
			{
				turn_s_angle(temp_str_angleZ-90);
				if(turn_s_ang_finish_flag==1)
				{
					/*
					这里检测是否有卡片堆
					*/
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					car_stop();
					turn_s_ang_finish_flag=0;
					f_cir_step_flag=2;
				}
			}
			
			if(f_cir_step_flag==2)
			{
				temp_stop_counts++;
				temp_str_angleZ=anglez_trsl_in_cir(angleZ);
				temp_op_anglez=anglez_trsl_in_cir(angleZ);
				if(temp_stop_counts>=10)
				{
					temp_stop_counts=10;
					if(out_card_flag==0){f_cir_step_flag=3;temp_stop_counts=0;}
					else if(out_card_flag==1){f_cir_step_flag=11;temp_stop_counts=0;}
				}
//				if(out_card_flag==1&&have_card_flag==0){f_cir_step_flag=11;}
//				if(out_card_flag==0&&have_card_flag==1){f_cir_step_flag=3;}
			}
			if(f_cir_step_flag==3)
			{
				op_go_for_card_gam();//该函数里面使用的角度为temp_op_anglez，而不是temp_str_angleZ，所以这里容易出bug
				/*这里要切换为第二个art来识别*/
				if(out_card_flag==1&&smotor_catch_finish_flag==1){f_cir_step_flag=4;smotor_catch_finish_flag=0;}
			}
			if(f_cir_step_flag==4)
			{
				art_catch_card_convert_speed(temp_op_anglez,50,0);
				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cir_disc>0.04)
				{
					car_stop();
					f_cir_step_flag=5;
					temp_cir_disc=0;
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
				}
			}
			if(f_cir_step_flag==5)
			{
				turn_s_angle(temp_str_angleZ+90);
				if(turn_s_ang_finish_flag==1)
				{
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					turn_s_ang_finish_flag=0;
					f_cir_step_flag=6;
				}
			}
			if(f_cir_step_flag==6)
			{
				art_catch_card_convert_speed(temp_op_anglez,50,0);
				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cir_disc>0.04)
				{
					car_stop();
					f_cir_step_flag=7;
					temp_cir_disc=0;
				}
			}
			if(f_cir_step_flag==7)
			{
				if(border_line_right>50){card_out_flag_right=1;}//判断是否该出圆环
				cir_move_x_error=(border_line_left+border_line_right)/2;  //计算误差
				cir_move_z_error=border_line_left-border_line_right;      //计算误差
				tr_circal_move(cir_move_x_error,-cir_move_z_error);//小车循迹圆环函数
				if(card_out_flag_right==1)//当出环标志位为1时
				{
					
					car_stop();//小车停止为下一步出环做准备
					f_cir_step_flag=8;
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
				}
			}
			if(f_cir_step_flag==8)
			{
				turn_s_angle(temp_str_angleZ-110);//转向跑道为正常循迹做准备
				if(turn_s_ang_finish_flag==1){turn_s_ang_finish_flag=0;f_cir_step_flag=9;move_s_dist_c=0;}//当转向完成时
			}
//			if(f_cir_step_flag==9)
//			{
//				art_catch_card_convert_speed(temp_op_anglez,50,0);
//				temp_cir_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
//				if(temp_cir_disc>0.04)
//				{
//					car_stop();
//					f_cir_step_flag=10;
//					temp_cir_disc=0;
//				}
//			}
			//正常循迹开始，相关圆环循迹变量与标志位回归初始化
			if(f_cir_step_flag==9)
			{					
				ImageFlag.image_element_rings_flag=8;
				temp_str_angleZ=0;
				temp_op_anglez=0;
				turn_s_ang_finish_flag=0;
				enter_circular_flag=0;
				dir_cir_flag=0;
				card_out_flag_right=0;
				move_spe_dist_finish_flag=0;
				turn_flag=0;
				set_speed=CAR_SPEED;//由于之前修改过循迹的速度，所以要重新设定速度
				move_s_dist_c=0;
				f_cir_step_flag=0;
				car_move_mode=0;
				have_card_flag=0;
			}
			if(f_cir_step_flag==11)
			{
				turn_s_angle(temp_str_angleZ+90);
				if(turn_s_ang_finish_flag==1){turn_s_ang_finish_flag=0;f_cir_step_flag=9;move_s_dist_c=0;}//当转向完成时
			}
		}	
	}
}
/******************
函数功能:小车运动在十字的逻辑代码函数
入口参数:int16
返回值:void
备注：无
******************/
uint8_t f_cross_step_flag=0;
//以下的变量为临时设置的，等与洪飞同步

uint8_t enter_cross_flag;
uint8_t dir_cross_flag;



//以后为临时变量
double temp_cross_disc;

extern int turn_flag;

void carmove_in_cross(void)
{
	if(enter_cross_flag==1)//当进入标志位为一时
	{
		//判断左右十字(ImageFlag.cross_flag这个是临时的，要改)
		if(ImageFlag.cross_flag==3){dir_cross_flag=1;}//左十字
		if(ImageFlag.cross_flag==4){dir_cross_flag=2;}//右十字
		//如果是左十字
		if(dir_cross_flag==1)
		{
			if(f_cross_step_flag==0)
			{
				car_stop();
				temp_str_angleZ=anglez_trsl_in_cir(angleZ);
				temp_op_anglez=anglez_trsl_in_cir(angleZ);
				f_cross_step_flag=1;
			}
			if(f_cross_step_flag==1)
			{
				art_catch_card_convert_speed(temp_op_anglez,80,0);
				temp_cross_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cross_disc>0.03)
				{
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					f_cross_step_flag=2;
					temp_cross_disc=0;
				}
			}
			//开始转向，面朝卡片堆
			if(f_cross_step_flag==2)
			{
				turn_s_angle(anglez_trsl_in_cir(temp_str_angleZ+90));
				if(turn_s_ang_finish_flag==1)
				{
					/*
					这里检测是否有卡片堆
					*/
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					turn_s_ang_finish_flag=0;
					f_cross_step_flag=3;
				}
			}
			
			if(f_cross_step_flag==3)
			{
				temp_stop_counts++;
				if(temp_stop_counts>=8)
				{
					temp_stop_counts=8;
					if(out_card_flag==0){f_cross_step_flag=4;temp_stop_counts=0;}
					else if(out_card_flag==1){f_cross_step_flag=11;temp_stop_counts=0;}
				}
			}
			if(f_cross_step_flag==4)
			{
				op_go_for_card_gam();//该函数里面使用的角度为temp_op_anglez，而不是temp_str_angleZ，所以这里容易出bug
				/*这里要切换为第二个art来识别*/
				if(out_card_flag==1&&smotor_catch_finish_flag==1){f_cross_step_flag=5;smotor_catch_finish_flag=0;}
			}
			if(f_cross_step_flag==5)
			{
				turn_s_angle(temp_str_angleZ+180);
				if(turn_s_ang_finish_flag==1)
				{
					/*
					这里检测是否有卡片堆
					*/
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					turn_s_ang_finish_flag=0;
					f_cross_step_flag=6;
				}
			}
			if(f_cross_step_flag==6)
			{
				if(border_line_left>50&&border_line_right<20){card_out_flag_left=1;}//判断是否该出圆环
				cir_move_x_error=(border_line_left+border_line_right)/2;  //计算误差
				cir_move_z_error=border_line_left-border_line_right;      //计算误差
				op_find_card_move(cir_move_x_error,-cir_move_z_error,1);//小车循迹圆环函数
				if(card_out_flag_left==1)//当出环标志位为1时
				{
					
					car_stop();//小车停止为下一步出环做准备
					f_cross_step_flag=7;
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
				}
			}
			if(f_cross_step_flag==7)
			{
				turn_s_angle(temp_str_angleZ+90);//转向跑道为正常循迹做准备
				if(turn_s_ang_finish_flag==1){turn_s_ang_finish_flag=0;f_cross_step_flag=8;}//当转向完成时
			}
			//正常循迹开始，相关圆环循迹变量与标志位回归初始化
			if(f_cross_step_flag==8)
			{					
				ImageFlag.cross_flag=5;//这里要换成有关十字的标志位
				temp_str_angleZ=0;
				temp_op_anglez=0;
				turn_s_ang_finish_flag=0;
				enter_cross_flag=0;
				dir_cross_flag=0;
				card_out_flag_left=0;
				move_spe_dist_finish_flag=0;
				turn_flag=0;//这里要改成十字相关代码
				set_speed=CAR_SPEED;//由于之前修改过循迹的速度，所以要重新设定速度
				move_s_dist_c=0;
				f_cross_step_flag=0;
				car_move_mode=0;
				have_card_flag=0;
			}
			if(f_cross_step_flag==11)
			{
				art_catch_card_convert_speed(temp_op_anglez,-50,0);
				temp_cross_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
//				if(temp_cross_disc<-0.04)
//				{
//					car_stop();
//					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
//					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					f_cross_step_flag=12;
//					temp_cross_disc=0;
//				}
			}
			if(f_cross_step_flag==12)
			{
				turn_s_angle(temp_str_angleZ-90);
				if(turn_s_ang_finish_flag==1)
				{
					turn_s_ang_finish_flag=0;
					f_cross_step_flag=8;
				}//当转向完成时
			}
		}
		/*
		相关原理类似，只有正负号修改，与相关变量修改（dir_cir_flag==2，card_out_flag_right，border_line_right）
		*/
		if(dir_cross_flag==2)
		{
			if(f_cross_step_flag==0)
			{
				car_stop();
				temp_str_angleZ=anglez_trsl_in_cir(angleZ);
				temp_op_anglez=anglez_trsl_in_cir(angleZ);
				f_cross_step_flag=1;
			}
			if(f_cross_step_flag==1)
			{
				art_catch_card_convert_speed(temp_op_anglez,80,0);
				temp_cross_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
				if(temp_cross_disc>0.03)
				{
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					f_cross_step_flag=2;
					temp_cross_disc=0;
				}
			}
			//开始转向，面朝卡片堆
			if(f_cross_step_flag==2)
			{
				turn_s_angle(temp_str_angleZ-90);
				if(turn_s_ang_finish_flag==1)
				{
					/*
					这里检测是否有卡片堆
					*/
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					turn_s_ang_finish_flag=0;
					f_cross_step_flag=3;
				}
			}
			
			if(f_cross_step_flag==3)
			{
				temp_stop_counts++;
				if(temp_stop_counts>=8)
				{
					temp_stop_counts=8;
					if(out_card_flag==0){f_cross_step_flag=4;temp_stop_counts=0;}
					else if(out_card_flag==1){f_cross_step_flag=11;temp_stop_counts=0;}
				}
			}
			if(f_cross_step_flag==4)
			{
				op_go_for_card_gam();//该函数里面使用的角度为temp_op_anglez，而不是temp_str_angleZ，所以这里容易出bug
				/*这里要切换为第二个art来识别*/
				if(out_card_flag==1&&smotor_catch_finish_flag==1){f_cross_step_flag=5;smotor_catch_finish_flag=0;}
			}
			if(f_cross_step_flag==5)
			{
				turn_s_angle(temp_str_angleZ+180);
				if(turn_s_ang_finish_flag==1)
				{
					/*
					这里检测是否有卡片堆
					*/
					car_stop();
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					turn_s_ang_finish_flag=0;
					f_cross_step_flag=6;
				}
			}
			if(f_cross_step_flag==6)
			{
				if(border_line_right>50&&border_line_left<20){card_out_flag_right=1;}//判断是否该出圆环
				cir_move_x_error=(border_line_left+border_line_right)/2;  //计算误差
				cir_move_z_error=border_line_left-border_line_right;      //计算误差
				op_find_card_move(cir_move_x_error,-cir_move_z_error,1);//小车循迹圆环函数
				if(card_out_flag_right==1)//当出环标志位为1时
				{
					
					car_stop();//小车停止为下一步出环做准备
					f_cross_step_flag=7;
					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
					temp_op_anglez=anglez_trsl_in_cir(angleZ);
				}
			}
			if(f_cross_step_flag==7)
			{
				turn_s_angle(temp_str_angleZ-90);//转向跑道为正常循迹做准备
				if(turn_s_ang_finish_flag==1){turn_s_ang_finish_flag=0;f_cross_step_flag=8;}//当转向完成时
			}
			//正常循迹开始，相关圆环循迹变量与标志位回归初始化
			if(f_cross_step_flag==8)
			{					
				ImageFlag.cross_flag=5;//这里要换成有关十字的标志位
				temp_str_angleZ=0;
				temp_op_anglez=0;
				turn_s_ang_finish_flag=0;
				enter_cross_flag=0;
				dir_cross_flag=0;
				card_out_flag_right=0;
				move_spe_dist_finish_flag=0;
				turn_flag=0;//这里要改成十字相关代码
				set_speed=CAR_SPEED;//由于之前修改过循迹的速度，所以要重新设定速度
				move_s_dist_c=0;
				f_cross_step_flag=0;
				car_move_mode=0;
				have_card_flag=0;
			}
			if(f_cross_step_flag==11)
			{
//				art_catch_card_convert_speed(temp_op_anglez,-50,0);
//				temp_cross_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
//				if(temp_cross_disc<-0.04)
//				{
//					car_stop();
//					temp_str_angleZ=anglez_trsl_in_cir(angleZ);
//					temp_op_anglez=anglez_trsl_in_cir(angleZ);
					f_cross_step_flag=12;
//					temp_cross_disc=0;
//				}
			}
			if(f_cross_step_flag==12)
			{
				turn_s_angle(temp_str_angleZ+90);
				if(turn_s_ang_finish_flag==1)
				{
					turn_s_ang_finish_flag=0;
					f_cross_step_flag=8;
				}//当转向完成时
			}
		}
	}
}
/**************************************************************
OPEN ART 逻辑代码部分
**************************************************************/
uint8_t f_c_step_flag=0;
uint8_t use_art_num=0;
int temp_op_anglez;
uint8_t op_smotor_catch_flag=0;
uint8_t op_smotor_put_flag=0;

void op_dist_judge(void);

/******************
函数功能:速度环与角度环双重闭环
现在这个函数还存在较大问题，需要等到新的目标检测之后在做验证
******************/
void art_catch_card_convert_speed(int temp_angle,int speed,int mode)
{
	int temp_turn_aroud_speed;
	int temp_target_speed1,temp_target_speed2,temp_target_speed3,temp_target_speed4;
	angle_pd1(temp_angle,anglez_trsl_in_cir(angleZ));

	//调节y轴时
	if(mode==0)
	{
			temp_target_speed1=speed-c_pwm_out*1.35;
			temp_target_speed2=speed+c_pwm_out*1.35;
			temp_target_speed3=speed-c_pwm_out*1.35;
			temp_target_speed4=speed+c_pwm_out*1.35;
	}
	//调节x轴时
	if(mode==1)
	{
		temp_target_speed1=speed-c_pwm_out ;
		temp_target_speed2=-speed+c_pwm_out;
		temp_target_speed3=-speed-c_pwm_out;
		temp_target_speed4=speed+c_pwm_out ;
	}
	target_speed1=temp_target_speed1;
	target_speed2=temp_target_speed2;
	target_speed3=temp_target_speed3;
	target_speed4=temp_target_speed4;

}
/******************
函数功能:寻找卡片,小车挡位调节代码
并且利用660保持车头不偏航（可加可不加吧，感觉平移的时候不会歪太多，等待验证吧）
******************/
//可能还要加个位置环，来保持车头始终朝向卡片
double op_x_dist,op_y_dist;
int op_cali_speed;
int op_turn_dir=0;
uint8_t cali_catch_card_step=0;
int test_box_num=0;
void op_go_for_card_gam(void)
{	
	if(cali_catch_card_step==0)
	{
		if(op_x_er>=0)
		{
			if(op_x_er>50){op_cali_speed=30;}
			if(op_x_er<=50&&op_x_er>40){op_cali_speed=20;}
			if(op_x_er<=40&&op_x_er>30){op_cali_speed=10;}
			if(op_x_er<=30){op_cali_speed=0;}
		}
		if(op_x_er<0)
		{
			if(op_x_er<-50){op_cali_speed=-30;}
			if(op_x_er>=-50&&op_x_er<-40){op_cali_speed=-20;}
			if(op_x_er>=-40&&op_x_er<-30){op_cali_speed=-10;}
			if(op_x_er>=-30){op_cali_speed=0;}
		}
		//car_translation(op_cali_speed);//平移
		//op_x_dist+=(wheel_distance1+wheel_distance4)/2.0;
		art_catch_card_convert_speed(temp_op_anglez,op_cali_speed,1);//角度换与速度环双重闭环
		if(op_x_er>=-30&&op_x_er<=30){cali_catch_card_step=1;}//校准成功标志位
	}
	if(cali_catch_card_step==1)
	{
		if(op_y_er>=0)
		{
			if(op_y_er>30){op_cali_speed=35;}
			if(op_y_er<=30&&op_y_er>20){op_cali_speed=25;}
			if(op_y_er<=20&&op_y_er>5){op_cali_speed=15;}
			if(op_y_er<=3){op_cali_speed=0;}
		}
		if(op_y_er<0)
		{
			if(op_y_er<-30){op_cali_speed=-35;}
			if(op_y_er>=-30&&op_y_er<-20){op_cali_speed=-25;}
			if(op_y_er>=-20&&op_y_er<-5){op_cali_speed=-15;}
			if(op_y_er>=-3){op_cali_speed=0;}
		}
		if(op_y_er>=-3&&op_y_er<=3){cali_catch_card_step=2;}
		//op_y_dist+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
		art_catch_card_convert_speed(temp_op_anglez,-op_cali_speed,0);
		//car_go_straight(-op_cali_speed);
	}
	if(cali_catch_card_step==2)
	{
		if(op_x_er>=0)
		{
			if(op_x_er>30){op_cali_speed=25;}
			if(op_x_er<=30&&op_x_er>10){op_cali_speed=15;}
			if(op_x_er<=10&&op_x_er>5){op_cali_speed=10;}
			if(op_x_er<=5){op_cali_speed=0;}
		}
		if(op_x_er<0)
		{
			if(op_x_er<-30){op_cali_speed=-25;}
			if(op_x_er>=-30&&op_x_er<-10){op_cali_speed=-15;}
			if(op_x_er>=-10&&op_x_er<-5){op_cali_speed=-10;}
			if(op_x_er>=-5){op_cali_speed=0;}
		}
		//op_x_dist+=(wheel_distance1+wheel_distance4)/2.0;
		//car_translation(op_cali_speed);
		art_catch_card_convert_speed(temp_op_anglez,op_cali_speed,1);//角度换与速度环双重闭环
		if(op_x_er>=-5&&op_x_er<=5){cali_catch_card_step=3;}//校准成功标志位
	}
	if(cali_catch_card_step==3)
	{
		car_stop();
		smotor_catch_card();
		test_box_num++;
		cali_catch_card_step=0;
		op_x_er=300;
		op_y_er=300;
	}
}
/******************
函数功能:判断什么时候开始捡卡片模式
入口参数:void
返回值:void
******************/
void op_dist_judge(void)
{	
	//if(out_card_flag==0&&jueduizhi(op_y_er)<45){car_move_mode=2;have_card_flag=1;}
	if(jueduizhi(op_y_er)<25)
	{
		if(car_move_mode!=1&&car_move_mode!=3)
		{
			car_move_mode=2;
		}
		
		have_card_flag=1;
	}
	else{have_card_flag=0;}
	//if(out_card_flag==1||jueduizhi(op_y_er)>=50){car_move_mode=0;have_card_flag=0;}
}
/******************
函数功能:小车沿赛道寻找并且拾取卡片，最后返回赛道逻辑代码
入口参数:void
返回值:void
存在问题：
还是偶尔会出现转到指定角度后，一直不把标志位置一
捡完卡片后，一直循环在4中，但是out_card_flag一直是一
******************/
double temp_op_disc=0;

void car_find_card(void)
{
	if(f_c_step_flag==0)
	{
		
		if(op_x_er>=0)
		{
			op_turn_dir=1;
		}
		else if(op_x_er<0)
		{
			op_turn_dir=2;
		}
		f_c_step_flag=1;
		temp_op_anglez=anglez_trsl_in_cir(angleZ);//储存当前角度
	}
	if(f_c_step_flag==1)
	{
		if(op_turn_dir==1)
		{
			turn_s_angle(temp_op_anglez-90);
			
		}
		if(op_turn_dir==2)
		{
			turn_s_angle(temp_op_anglez+90);
		}
		if(turn_s_ang_finish_flag==1)
		{
			temp_op_anglez=anglez_trsl_in_cir(angleZ);
			turn_s_ang_finish_flag=0;
			f_c_step_flag=2;
			op_x_er=300;
			op_y_er=300;
			move_s_dist_c=0;
			car_stop();
		}
	}
	/*在考虑是不是小车转过角度后药向后退一段距离在进行平移*/
	if(f_c_step_flag==2)
	{
		art_catch_card_convert_speed(temp_op_anglez,-50,0);
		temp_op_disc+=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
		if(temp_op_disc<-0.025)
		{
			car_stop();
			f_c_step_flag=3;
			temp_op_disc=0;
		}
	}
	if(f_c_step_flag==3)
	{
		cir_move_x_error=(border_line_left+border_line_right)/2;  //计算误差
		cir_move_z_error=border_line_left-border_line_right;      //计算误差
		op_find_card_move(cir_move_x_error,-cir_move_z_error,op_turn_dir);//小车循迹圆环函数
		if(jueduizhi(op_x_er)<15){car_stop();f_c_step_flag=4;}
	}
//	if(f_c_step_flag==3)
//	{
////		if(jueduizhi(op_x_er)>15)
////		{
//			if(op_turn_dir==1){art_catch_card_convert_speed(temp_op_anglez,-50,1);}
//			if(op_turn_dir==2){art_catch_card_convert_speed(temp_op_anglez,50,1);}
//			cir_move_x_error=(border_line_left+border_line_right)/2;  //计算误差
//			cir_move_z_error=border_line_left-border_line_right;      //计算误差
//			op_find_card_move(cir_move_x_error,cir_move_z_error,op_turn_dir);
////		}
//		//if(out_card_flag==0){car_stop();f_c_step_flag=4;}
//		if(jueduizhi(op_x_er)<15){car_stop();f_c_step_flag=4;}
//	}
	if(f_c_step_flag==4)
	{
		op_go_for_card_gam();//开始向卡片并且捡卡片
		if(out_card_flag==1&&smotor_catch_finish_flag==1)
		{
			smotor_catch_finish_flag=0;
			f_c_step_flag=5;
			temp_op_anglez=anglez_trsl_in_cir(angleZ);
		}
	}
	if(f_c_step_flag==5)
	{
		if(op_turn_dir==1)
		{
			turn_s_angle(temp_op_anglez+90);
		}
		if(op_turn_dir==2)
		{
			turn_s_angle(temp_op_anglez-90);
		}
		if(turn_s_ang_finish_flag==1)
		{
			turn_s_ang_finish_flag=0;
			f_c_step_flag=6;
			temp_op_anglez=anglez_trsl_in_cir(angleZ);
		}
	}
	if(f_c_step_flag==6)
	{
		f_c_step_flag=0;
		set_speed=CAR_SPEED;
		have_card_flag=0;
		out_card_flag=1;
		op_x_er=300;
		op_y_er=300;
		car_move_mode=0;
		op_turn_dir=0;
	}
}
/******************************
主函数部分
*******************************/
uint8_t car_move_mode=0;

extern SystemDatatypdef SystemData;
extern float tt_distance;
extern int turn_flag;

void car_move_part(void)
{
	//利用wifi无线控制
//		set_speed=seekfree_assistant_parameter[2];
//		if(seekfree_assistant_parameter[7]==10)			//控制电机转动或者停止
//		{
//			tr_straight_move(vision_error);
//		}
//		if(seekfree_assistant_parameter[7]==5)
//		{
//			car_stop();
//		}
		//有线控制
//		tr_straight_move(vision_error);
//		tt_distance=(wheel_distance1+wheel_distance2+wheel_distance3+wheel_distance4)/4.0;
		/******************
		整体循迹
		******************/		
		//拾取零散卡片循迹
// 		op_go_for_card();
//		if(have_card_flag==1){op_go_for_card_gam();}
//		op_go_for_card_gam();
		//直道拾取放下卡片分类
//		extern uint8_t card_sorting_in_straight_step;
//		if(card_sorting_in_straight_step!=5)
//		{
//			card_sorting_in_straight();
//		}
//		if(card_sorting_in_straight_step==5)
//		{
//			put_card_in_str();
//		}
		//十字，圆环拾取放下卡片分类
		extern uint8_t card_sorting_cir_cro_step;
		if(card_sorting_cir_cro_step!=5)
		{
			card_sorting_cir_cro();
		}
		if(card_sorting_cir_cro_step==5)
		{
			put_card_in_cir_cro();
		}
		


		//普通循迹
//		if(car_move_mode==0)
//		{
//			if(SystemData.Stop!=1)
//			{
//				tr_straight_move(vision_error);
//			}
//			if(SystemData.Stop==1)
//			{
//				distance_cal();
//				if(tt_distance<3.5)
//				{
//					tr_straight_move(vision_error);
//				}
//				if(tt_distance>=3.5)
//				{
//					tt_distance=3.5;
//					car_stop();
//				}
//			}
//		}
//		//圆环循迹
//		if(car_move_mode==1)
//		{
//			carmove_in_circular();
//		}
//		//拾取卡片
//		if(car_move_mode==2)
//		{
//			car_find_card();
//		}
//		//十字循迹
//		if(car_move_mode==3)
//		{
//			carmove_in_cross();
//		}
}