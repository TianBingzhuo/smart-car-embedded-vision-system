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
#include <math.h>
#include "car_move.h"
//float imu660ra_acc_t_x,imu660ra_acc_t_y,imu660ra_acc_t_z;
//float imu660ra_gyro_t_x,imu660ra_gyro_t_y,imu660ra_gyro_t_z;
//float gyro_angle_x = 0,gyro_angle_y = 0,gyro_angle_z = 0;
//float g_x=-171,g_y=54,g_z=21;                    //陀螺仪矫正参数
//float a_x=0,a_y=0;                          //角度矫正参数
//float angle_x=0,angle_y=0,angle_z=0;        //由角速度计算的角速率(角度制)
//float angle_ax=0,angle_ay=0,angle_az=0;     //由加速度计算的加速度(弧度制)

float gyro_x,gyro_y,gyro_z;
float gyro_x_init,gyro_y_init,gyro_z_init;
float acc_x,acc_y,acc_z;
float angleX=0,angleY=0,angleZ=0;           //四元数解算出的欧拉角
int val_counts=0;

/**********
欧拉角计算
**********/
#define pi      3.14159265f
#define alpha           0.75f		//权重
#define Kp     12.8// 12.8f         //0.8f  //8.8
#define Ki      0.001//f      //0.001
#define halfT   0.6564f
#define dt   	0.005f
float  q0=1,q1=0,q2=0,q3=0;
float  exInt=0,eyInt=0,ezInt=0;
/*********
陀螺仪零飘初始化
*********/
void gyroOffset_init(void) 
{
    gyro_x_init= 0;
    gyro_y_init= 0;
    gyro_z_init= 0;
    for (uint16_t i = 0; i < 100; ++i) 
	{
       imu660ra_get_gyro();
       gyro_x_init+= imu660ra_gyro_x;
       gyro_y_init+= imu660ra_gyro_y;
       gyro_z_init+= imu660ra_gyro_z;
       system_delay_ms(10);
    }
    gyro_x_init /= 100;
    gyro_y_init /= 100;
    gyro_z_init /= 100;
}
/*********
转化角度
*********/
void translation_getValues(void)
{
    //转化角加速度，单位g/s
    acc_x = imu660ra_acc_transition(imu660ra_acc_x);
    acc_y = imu660ra_acc_transition(imu660ra_acc_y);
    acc_z = imu660ra_acc_transition(imu660ra_acc_z);

    //转化加速度，单位°/s
    gyro_x = imu660ra_gyro_transition(imu660ra_gyro_x-gyro_x_init)*pi/180;
    gyro_y = imu660ra_gyro_transition(imu660ra_gyro_y-gyro_y_init)*pi/180;
    gyro_z = imu660ra_gyro_transition(imu660ra_gyro_z-gyro_z_init)*pi/180;
	
}

/*********
将数据转化为角度
*********/
void xyz_normalization(float gx, float gy, float gz, float ax, float ay, float az)
{
	float  norm;
    float  vx, vy, vz;
    float  ex, ey, ez;
	
    norm = sqrt(ax*ax + ay*ay + az*az); //把加速度计的三维向量转成单维向量
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
        //  下面是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
        //  根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素
        //  所以这里的vx vy vz，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的
        //  重力单位向量。
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;

    ex = (ay*vz - az*vy) ;
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;

    exInt = exInt + ex * Ki*dt;
    eyInt = eyInt + ey * Ki*dt;
    ezInt = ezInt + ez * Ki*dt;

    gx = alpha*gx + (1-alpha)*(Kp*ex + exInt);
    gy = alpha*gy + (1-alpha)*(Kp*ey + eyInt);
    gz = alpha*gz + (1-alpha)*(Kp*ez + ezInt);//姿态更新，四元数微分求解
    //采用一阶解法更新四元数
    q0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT*dt;
    q1 = q1 + ( q0*gx + q2*gz - q3*gy) * halfT*dt;
    q2 = q2 + ( q0*gy - q1*gz + q3*gx) * halfT*dt;
    q3 = q3 + ( q0*gz + q1*gy - q2*gx) * halfT*dt;//一阶法

    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
//转化到欧拉角
	angleX = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.2957795f; // pitch
    angleY = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.2957795f; // roll
    angleZ = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.2957795f; // yaw
}

void angle_final_val(void)
{
	//采集陀螺仪数据
    imu660ra_get_acc(); 
    imu660ra_get_gyro();
	translation_getValues();
	xyz_normalization(gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z);
}

void u660_init(void)
{
	imu660ra_init();
	gyroOffset_init();
}

int target_angle=0;
//float angle_kp=4.524,angle_kd=1.528;
float angle_kp=2.524,angle_kd=0.528;
float angle_c_error=0,angle_l_error=0;
float c_pwm_out=0;

//660角度闭环
void angle_pd1(int t_angle,int u_angle)
{
	//这里主要利用了数学计算得出怎么样转的角度最小
	int temp_derta_angle1,temp_derta_angle2;
	if(u_angle>t_angle)
	{
		temp_derta_angle1=t_angle-u_angle;
		temp_derta_angle2=360+t_angle-u_angle;
	}
	else if(u_angle<t_angle)
	{
		temp_derta_angle1=t_angle-u_angle;
		temp_derta_angle2=-(360-t_angle+u_angle);
	}
	else if(u_angle==t_angle)
	{
		temp_derta_angle1=temp_derta_angle2=0;
	}
	
	if(abs(temp_derta_angle1)>=abs(temp_derta_angle2)){angle_c_error=(float)temp_derta_angle2;}//当前误差
	else if(abs(temp_derta_angle1)<abs(temp_derta_angle2)){angle_c_error=(float)temp_derta_angle1;}
	
    c_pwm_out=angle_kp*angle_c_error+angle_kd*(angle_c_error-angle_l_error);//当前误差-上次误差
	c_pwm_out=xianfu(c_pwm_out,80);					//限幅
    angle_l_error=angle_c_error;
}

extern int16 set_turnround_speed;
int turn_s_ang_finish_flag=0;//转向完成标志位
void turn_s_angle(int angle)
{
	if(angle>360){angle-=360;}
	angle_pd1(angle,anglez_trsl_in_cir(angleZ));
	set_turnround_speed=(int)c_pwm_out;
	if(set_turnround_speed>=0)
	{
		car_turnround(1,jueduizhi(set_turnround_speed));
	}
	if(set_turnround_speed<0)
	{
		car_turnround(0,jueduizhi(set_turnround_speed));
	}
	if((anglez_trsl_in_cir(angleZ)-anglez_trsl_in_cir(angle))<=2&&(anglez_trsl_in_cir(angleZ)-anglez_trsl_in_cir(angle))>=-2)
	{
		turn_s_ang_finish_flag=1;
	}
}
/*********
转化角度为0-360
*********/
int anglez_trsl_in_cir(int anglez)
{
	int trsl_angle;
	if(anglez<0)
	{
		trsl_angle=360+anglez;
		
	}
	else if(anglez>=0)
	{
		trsl_angle=anglez;
	}
	return trsl_angle;
	
}




