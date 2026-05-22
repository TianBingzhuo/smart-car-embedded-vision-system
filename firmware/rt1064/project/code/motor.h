#ifndef _motor_h
#define _motor_h

#include "zf_common_headfile.h"
//总钻风
#define OX  (50/ 3000.0)  //标度变换

#define MOTOR4_DIR               (C9 )
#define MOTOR4_PWM               (PWM2_MODULE1_CHA_C8)

#define MOTOR3_DIR               (C7 )
#define MOTOR3_PWM               (PWM2_MODULE0_CHA_C6)

#define MOTOR2_DIR               (D2 )
#define MOTOR2_PWM               (PWM2_MODULE3_CHB_D3)

#define MOTOR1_DIR               (C10 )
#define MOTOR1_PWM               (PWM2_MODULE2_CHB_C11)


void motor_init(void);
void motor_set_speed1(int16 speed);
void motor_set_speed2(int16 speed);
void motor_set_speed3(int16 speed);
void motor_set_speed4(int16 speed);
int pid1(double target_speed, double current_speed);
int pid2(double target_speed, double current_speed);
int pid3(double target_speed, double current_speed);
int pid4(double target_speed, double current_speed);
int xianfu(int16 value,int16 max_val);
int jueduizhi(int a);
void car_go_straight(int16 speed);
void car_l_translation(int16 speed);
void car_r_translation(int16 speed);
void car_translation(int16 speed);
void car_diagonal(int num,int16 speed);
void car_turnround(int dir,int16 speed);
void car_stop(void);
void move_transfrom(double vx,double vy,double vz);
void car_move_dist(void);
void car_mve_test_part(void);

extern int16 temp_encoder_counts1,temp_target_counts1;
extern int16 error1,control_speed1, last_error1,l_last_error1;

extern int16 temp_encoder_counts2,temp_target_counts2;
extern int16 error2,control_speed2, last_error2,l_last_error2;

extern int16 temp_encoder_counts3,temp_target_counts3;
extern int16 error3,control_speed3, last_error3,l_last_error3;

extern int16 temp_encoder_counts4,temp_target_counts4;
extern int16 error4,control_speed4, last_error4,l_last_error4;
extern int16 target_speed1,target_speed2,target_speed3,target_speed4;


extern double dist_c_error;
extern double dist_l_error;
extern double t_dist;
extern double dist_kp;
extern double dist_kd;
extern int dist_pwm_out;
extern int set_go_straight_speed;


//总钻风
typedef struct {
  float nowspeed;     // pulse表示nowspeed
  int expectspeed;    // speed表示expectspeed
  int motor_duty;     //电机占空比
  float Length;       //走过路程
  int Circle_OUT_th;
  int MinSpeed;             //最低速度
  int MaxSpeed;             //最高速度
  float expect_True_speed;  //实际期望速度
  int straight_speed;       //直道速度
} SpeedDatatypedef;

#endif