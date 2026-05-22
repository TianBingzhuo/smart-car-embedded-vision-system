#include "key.h"
#include "smotor.h"

uint8 exti_state[4]={0,0,0,0};

uint8 temp_test_duty=0;
int temp_key_flag=0;
void key_part(void)
{
	if(!gpio_get_level(C15))
	{
		smotor_catch_card();
//		temp_key_flag=1;
//		if(temp_key_flag==1&&gpio_get_level(C15)==1)
//		{
//			temp_test_duty+=5;
//			temp_key_flag=0;
//		}
//		pwm_set_duty(SERVO_MOTOR_PWM3,(uint32_t)SERVO_MOTOR_DUTY(temp_test_duty));
		
	}
	if(!gpio_get_level(C14))
	{
		smotor_put_card();
	}
	if(!gpio_get_level(C13))
	{
//		smotor_put_card();
	
		//put_in_box();
	}
	if(!gpio_get_level(C12))
	{
		
		//put_in_box();
//		temp_test_duty++;
//		pwm_set_duty(SERVO_MOTOR_PWM3,temp_test_duty);
	}
//		if(exti_state[0])
//        {
//            gpio_toggle_level(LED1);                                        // 翻转 LED 引脚输出电平 控制 LED 亮灭
//			key_test_flag=1;
//            exti_state[0] = 0;                                               // 清空外部中断触发标志位
//        }
//		if(exti_state[1])
//        {
//            gpio_toggle_level(LED1);                                        // 翻转 LED 引脚输出电平 控制 LED 亮灭
//			key_test_flag=2;
//            exti_state[1] = 0;                                               // 清空外部中断触发标志位
//        }
//		if(exti_state[2])
//        {
//            gpio_toggle_level(LED1);                                        // 翻转 LED 引脚输出电平 控制 LED 亮灭
//            key_test_flag=3;
//			exti_state[2] = 0;                                               // 清空外部中断触发标志位
//        }
//		if(exti_state[3])
//        {
//            gpio_toggle_level(LED1);                                        // 翻转 LED 引脚输出电平 控制 LED 亮灭
//            key_test_flag=4;
//			exti_state[3] = 0;                                               // 清空外部中断触发标志位
//        }
}

