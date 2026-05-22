#ifndef U660_H_
#define U660_H_

#include "zf_common_headfile.h"

void u660_init(void);
void xyz_normalization(float gx, float gy, float gz, float ax, float ay, float az);
void translation_getValues(void);
void gyroOffset_init(void) ;
float fast_sqrt(float x);
void angle_final_val(void);
void angle_pd1(int t_angle,int u_angle);
void turn_s_angle(int angle);
int anglez_trsl_in_cir(int anglez);

extern float imu660ra_acc_t_x,imu660ra_acc_t_y,imu660ra_acc_t_z;
extern float imu660ra_gyro_t_x,imu660ra_gyro_t_y,imu660ra_gyro_t_z;
extern float angleX,angleY,angleZ;           //四元数解算出的欧拉角
extern int target_angle,turn_s_ang_finish_flag;
extern float angle_kp,angle_kd;
extern float angle_c_error,angle_l_error;
extern float c_pwm_out;


#endif 
