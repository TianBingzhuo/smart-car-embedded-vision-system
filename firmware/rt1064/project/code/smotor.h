#ifndef	smotor_H_
#define	smotor_H_

#include "zf_common_headfile.h"
#include "math.h"

#define SERVO_MOTOR_PWM1                (PWM4_MODULE2_CHA_C30)                      // 定义主板上舵机对应引脚 舵机一
#define SERVO_MOTOR_PWM2                (PWM1_MODULE3_CHA_D0)                       // 定义主板上舵机对应引脚 舵机二
#define SERVO_MOTOR_PWM3                (PWM4_MODULE3_CHA_C31)                      // 定义主板上舵机对应引脚 舵机三
#define SERVO_MOTOR_PWM4                (PWM1_MODULE3_CHB_D1)                      // 定义主板上舵机对应引脚 电磁铁

#define SERVO_MOTOR_FREQ                (200)                                       // 定义主板上舵机频率  请务必注意范围 50-300
#define SERVO_MOVE_SPEED				(120)										// 定义主板上舵机运动速度

// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))    //舵机角度设置为0 - 180，90度为中值
#define SERVO_MOTOR_DUTY_360(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/180.0))    //舵机角度设置为0 - 360，180度为中值
#define SERVO_MOTOR_DUTY_stone(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(4.0*x))    //舵机角度设置为0 - 360，180度为中值

void servo_slow_ctrl(uint16 _servo1_angle, uint16 _servo2_angle, float _step_count);
void smotor_move_back(void);
void magnet_ctr(int mode);
void smotor_move_catch(void);
void smotor_move_put(void);
void smotor_init(void);
void smotor_catch_card(void);
void smotor_put_card(void);
void smotor_part(void);
void servo_slow_ctrl_360(uint16 _servo1_angle,float _step_count);

extern uint16 servo1_duty, servo2_duty;
extern uint8_t smotor_catch_finish_flag;
extern uint8_t smotor_put_finish_flag;

#endif
