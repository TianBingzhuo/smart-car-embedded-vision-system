#include "smotor.h"
#include "u660.h"
#include "car_move.h"
#include "box_logic_code.h"
uint16 servo1_duty = 0, servo2_duty = 0,servo3_duty=0;

#define	SERVO_MOTOR_PWM1_DUTY		(48)
#define	SERVO_MOTOR_PWM2_DUTY		(135)
#define	SERVO_MOTOR_PWM3_DUTY		(0)
#define	SERVO_MOTOR_PWM4_DUTY		(0)

void smotor_init(void)
{
	pwm_init(SERVO_MOTOR_PWM1, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_PWM1_DUTY));    
    pwm_init(SERVO_MOTOR_PWM2, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_PWM2_DUTY)); 
	pwm_init(SERVO_MOTOR_PWM3, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY_360(SERVO_MOTOR_PWM3_DUTY)); //0  55  112  172
	pwm_init(SERVO_MOTOR_PWM4, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY_stone(SERVO_MOTOR_PWM4_DUTY));
	
	servo1_duty = SERVO_MOTOR_PWM1_DUTY;servo2_duty = SERVO_MOTOR_PWM2_DUTY,servo3_duty=SERVO_MOTOR_PWM3_DUTY;	
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     舵机连续控制函数
// 参数说明     _servo1_angle               舵机1的目标角度
// 参数说明     _servo2_angle               舵机2的目标角度
// 返回参数     _step_count                 舵机连续控制间隔次数
// 使用示例     servo_slow_ctrl(90, 90, 100);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void servo_slow_ctrl(uint16 _servo1_angle, uint16 _servo2_angle, float _step_count)
{
    float servo1_start = (float)servo1_duty, servo2_start = (float)servo2_duty;
    float servo1_step = (float)(_servo1_angle - servo1_duty)/_step_count, servo2_step = (float)(_servo2_angle - servo2_duty)/_step_count;
    while(1)
    {
        system_delay_ms(2);
        if(fabsf(servo1_start - (float)_servo1_angle) >= servo1_step)servo1_start += servo1_step;
        else servo1_start = _servo1_angle;
        pwm_set_duty(SERVO_MOTOR_PWM1, (uint32)SERVO_MOTOR_DUTY((uint16)servo1_start));
        
        if(fabsf(servo2_start - (float)_servo2_angle) >= servo2_step)servo2_start += servo2_step;
        else servo2_start = _servo2_angle;
        pwm_set_duty(SERVO_MOTOR_PWM2, (uint32)SERVO_MOTOR_DUTY((uint16)servo2_start));
        
        if(fabsf(servo1_start - (float)_servo1_angle) < 1 && fabsf(servo2_start - (float)_servo2_angle) < 1)
        {
            servo1_duty = (uint16)_servo1_angle;
            servo2_duty = (uint16)_servo2_angle;
            return;
        }
    }
    
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     舵机连续控制函数
// 参数说明     _servo1_angle               舵机1的目标角度
// 返回参数     _step_count                 舵机连续控制间隔次数
// 使用示例     servo_slow_ctrl(90, 100);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void servo_slow_ctrl_360(uint16 _servo1_angle,float _step_count)
{
    float servo1_start = (float)servo3_duty;
    float servo1_step = (float)(_servo1_angle - servo3_duty)/_step_count;
    while(1)
    {
        system_delay_ms(1);
        if(fabsf(servo1_start - (float)_servo1_angle) >= servo1_step)servo1_start += servo1_step;
        else servo1_start = _servo1_angle;
        pwm_set_duty(SERVO_MOTOR_PWM3, (uint32)SERVO_MOTOR_DUTY_360((uint16)servo1_start));
        
        if(fabsf(servo1_start - (float)_servo1_angle) < 1 )
        {
            servo3_duty = (uint16)_servo1_angle;
            return;
        }
    }
    
}

//吸铁石控制函数	1吸取 0放下
void magnet_ctr(int mode)
{
//	if(mode==0){gpio_set_level(D1,0);pwm_set_duty(SERVO_MOTOR_PWM4, (uint32)SERVO_MOTOR_DUTY_stone(0));}
//	if(mode==1){gpio_set_level(D1,1);pwm_set_duty(SERVO_MOTOR_PWM4, (uint32)SERVO_MOTOR_DUTY_stone(1));}
	if(mode==0){pwm_set_duty(SERVO_MOTOR_PWM4, (uint32)SERVO_MOTOR_DUTY_stone(0));}
	if(mode==1){pwm_set_duty(SERVO_MOTOR_PWM4, (uint32)SERVO_MOTOR_DUTY_stone(1));}
}
//舵机回到待定位置
void smotor_move_back(void)
{
	servo_slow_ctrl(48, 135, SERVO_MOVE_SPEED);
}
//舵机拾取卡片
void smotor_move_catch(void)
{
	magnet_ctr(1);
	servo_slow_ctrl(120, 135, SERVO_MOVE_SPEED);
}
//舵机放下卡片
void smotor_move_put(void)
{
//	servo_slow_ctrl(80, 135, SERVO_MOVE_SPEED);
	//servo_slow_ctrl(80, 135, SERVO_MOVE_SPEED);
	//servo_slow_ctrl(80, 135, SERVO_MOVE_SPEED);
	servo_slow_ctrl(80, 135, SERVO_MOVE_SPEED);
	servo_slow_ctrl(80, 25, SERVO_MOVE_SPEED);
	servo_slow_ctrl(48, 25, SERVO_MOVE_SPEED);
	magnet_ctr(0);
}
uint8_t smotor_catch_finish_flag=0;
uint8_t smotor_put_finish_flag=0;
//舵机拾取卡片并放置
void smotor_catch_card(void)
{
	smotor_move_catch();
	smotor_move_put();
	smotor_move_back();
	smotor_catch_finish_flag=1;
}
//舵机拿下卡片并放置
void smotor_put_card(void)
{
//	servo_slow_ctrl(80, 135, SERVO_MOVE_SPEED);
//	servo_slow_ctrl(80, 12, SERVO_MOVE_SPEED);
	servo_slow_ctrl(48, 10, SERVO_MOVE_SPEED);
	servo_slow_ctrl(38, 10, SERVO_MOVE_SPEED);
	magnet_ctr(1);
//	servo_slow_ctrl(120, 10, SERVO_MOVE_SPEED);
//	servo_slow_ctrl(120, 135, SERVO_MOVE_SPEED);
	servo_slow_ctrl(60, 20, SERVO_MOVE_SPEED);
	servo_slow_ctrl(80, 135, SERVO_MOVE_SPEED);
	magnet_ctr(0);
	smotor_move_back();
	smotor_put_finish_flag=1;
}

void smotor_part(void)
{
	if(catch_card_flag==1)
	{
		smotor_catch_card();
		catch_card_flag=0;
		somtor_c_finish_flag=1;
	}
	if(put_card_flag==1)
	{
		smotor_put_card();
		put_card_flag=0;
		somtor_p_finish_flag=1;
	}
	if(op_smotor_catch_flag==1)
	{
		smotor_catch_card();
		op_smotor_catch_flag=0;
		somtor_c_finish_flag=1;
	}
	if(op_smotor_put_flag==1)
	{
		smotor_put_card();
		op_smotor_put_flag=0;
		somtor_p_finish_flag=1;
	}
}



















