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
#include "init.h"

#define IPS200_TYPE     (IPS200_TYPE_SPI)                                 // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8
                                                                                // 单排排针 SPI 两寸屏 这里宏定义填写 IPS200_TYPE_SPI

void init_all_parts(void)
{
	clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
	
	gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式  测试key功能是否正常
	gpio_init(BEEP, GPO, GPIO_LOW, GPO_PUSH_PULL);								//初始化BEEP，输出默认低电平。
	
	//初始化电磁铁引脚
    gpio_init(D1, GPO, 0, GPO_PUSH_PULL);
	/**************************
	无线串口初始化
	**************************/
	wirles_usart_init();
	/**************************
	按键
	**************************/
//	exti_init(KEY1, EXTI_TRIGGER_RISING);                                      // 初始化 KEY 为外部中断输入 上升沿触发
//	exti_init(KEY2, EXTI_TRIGGER_RISING);                                      // 初始化 KEY 为外部中断输入 上升沿触发
//	exti_init(KEY3, EXTI_TRIGGER_RISING);                                      // 初始化 KEY 为外部中断输入 上升沿触发
//	exti_init(KEY4, EXTI_TRIGGER_RISING);                                      // 初始化 KEY 为外部中断输入 上升沿触发

	gpio_init(C15, GPI, 1, GPI_PULL_UP);										// 初始化 KEY 为输入模式
	gpio_init(C14, GPI, 1, GPI_PULL_UP);										// 初始化 KEY 为输入模式
    gpio_init(C13, GPI, 1, GPI_PULL_UP);                                        // 初始化 KEY 为输入模式
    gpio_init(C12, GPI, 1, GPI_PULL_UP);                                        // 初始化 KEY 为输入模式
	/**************************
	舵机初始化
	舵机一垂直于地面，舵机二水平于地面
	**************************/
	smotor_init();
	/**************************
	总钻风
	**************************/
	ips200_set_dir(IPS200_PORTAIT);       // 需要先横屏 不然显示不下
    ips200_set_font(IPS200_8X16_FONT);    //设置显示字体,显示边线使用
    ips200_set_color(RGB565_RED, RGB565_WHITE);//设置显示颜色,显示边线使用
	mt9v03x_init();     //摄像头初始化
    Data_Settings();   // 参数给定  图像状态参数；舵机位置式PID参数
    ips200_init(IPS200_TYPE);
	/**************************
	open art相关初始化
	**************************/
	wir_usart_init();
	/**************************
	陀螺仪模块初始化
	**************************/
	u660_init();
	/**************************
		智能车运动
	**************************/
    encoder_init();			  //初始化
	motor_init();             //初始化
	/**************************
		BEEP初始化
	**************************/
	beep_ctr(0);
	/**************************
		中断配置
	**************************/
	
	system_delay_ms(6000);           //等待主板其他外设上电完成
	
	pit_ms_init(PIT_CH0,5);//读取pid
	pit_ms_init(PIT_CH1,5);//读取陀螺 仪
//	pit_ms_init(PIT_CH2,5000);
//	pit_ms_init(PIT_CH1,2500);//测试小车各个运动方式

    interrupt_global_enable(0);
	interrupt_set_priority(KEY_EXTI, 1);         // 设置 KEY对应外部中断的中断优先级  KEY1 KEY2 KEY3 KEY4共用一个中断函数
	interrupt_set_priority(PIT_IRQn, 0);
}


