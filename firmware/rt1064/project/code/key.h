#ifndef _key_h
#define _key_h
#include "zf_common_headfile.h"

void key_part(void);

#define LED1                    (B9 )											//测试专用
#define KEY1                    (C15)                                           // 使用的外部中断输入引脚 如果修改 需要同步对应修改外部中断编号与 isr.c 中的调用
#define KEY2                    (C14)                                           // 使用的外部中断输入引脚 如果修改 需要同步对应修改外部中断编号与 isr.c 中的调用
#define KEY3                    (C13)                                           // 使用的外部中断输入引脚 如果修改 需要同步对应修改外部中断编号与 isr.c 中的调用
#define KEY4                    (C12)                                           // 使用的外部中断输入引脚 如果修改 需要同步对应修改外部中断编号与 isr.c 中的调用

#define KEY_EXTI               (GPIO2_Combined_16_31_IRQn)                     // 对应外部中断的中断编号 

extern uint8 exti_state[4];

#endif
