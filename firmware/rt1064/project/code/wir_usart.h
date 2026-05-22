#ifndef _WIR_USART_h
#define _WIR_USART_h

#include "zf_common_headfile.h"

#define UART_BAUDRATE           115200                          // 默认 115200

#define UART_INDEX_4              UART_4                          // 默认 UART_4
#define UART_TX_PIN_4             UART4_TX_C16                    // 默认 UART4_TX_C16
#define UART_RX_PIN_4             UART4_RX_C17                    // 默认 UART4_RX_C17
#define UART_PRIORITY_4           (LPUART4_IRQn)                                  // 对应串口中断的中断编号 在 MIMXRT1064.h 头文件中查看 IRQn_Type 枚举体

#define UART_INDEX_2              UART_2                          // 默认 UART_4
#define UART_TX_PIN_2             UART2_TX_B18                    // 默认 UART4_TX_C16
#define UART_RX_PIN_2             UART2_RX_B19                    // 默认 UART4_RX_C17
#define UART_PRIORITY_2           (LPUART2_IRQn)                                  // 对应串口中断的中断编号 在 MIMXRT1064.h 头文件中查看 IRQn_Type 枚举体

#define UART_INDEX_1              UART_1                          // 默认 UART_4
#define UART_TX_PIN_1             UART1_TX_B12                   // 默认 UART4_TX_C16
#define UART_RX_PIN_1             UART1_RX_B13                   // 默认 UART4_RX_C17
#define UART_PRIORITY_1           (LPUART1_IRQn)                                  // 对应串口中断的中断编号 在 MIMXRT1064.h 头文件中查看 IRQn_Type 枚举体

void wir_usart_init(void);
void wir_usart_re(void);
void check_card_exist(void);
void wir_usart_re_4(void);
void wir_usart_re_2(void);

extern int op_x_er;
extern int op_y_er;
extern int op_x_er_counts;
extern int op_y_er_counts;
extern int op_x_er_tt;
extern int op_y_er_tt;

extern uint8_t out_card_flag;
extern int op_y_error_storage;
extern int op_y_error_storage_counts;

extern uint8_t	op_reg_num;	//接受的数字，不代表任何含义
extern uint8_t	op_reg_mode;	//接收到的数字，代表模式
extern uint8_t	op_reg_alph;	//接收到的数字，代表相应的字母
extern uint8_t	op_reg_small_cate;//接收到的数字，代表识别卡片的小种类的数字		
extern uint8_t	op_reg_big_cate;//接收到的数字，代表识别卡片的大种类的数字	



extern int8 uart_get_data[64];                                                        // 串口接收数据缓冲区
extern uint8_t fifo_get_data;   //用于缓冲区                                                   // fifo 输出读出缓冲区                            
extern uint8_t get_data;   //用于最开始串口读数据使用                                                          // 接收数据变量
extern fifo_struct uart_data_fifo;
extern uint32 data_len; 
extern int8 uart_get_data_2[64];                                                        // 串口接收数据缓冲区
extern uint8_t fifo_get_data_2;   //用于缓冲区                                                   // fifo 输出读出缓冲区                            
extern uint8_t get_data_2;   //用于最开始串口读数据使用                                                          // 接收数据变量
extern fifo_struct uart_data_fifo_2;
extern uint32 data_len_2;
extern int8 uart_get_data_4[64];                                                        // 串口接收数据缓冲区
extern uint8_t fifo_get_data_4;   //用于缓冲区                                                   // fifo 输出读出缓冲区                            
extern uint8_t get_data_4;   //用于最开始串口读数据使用                                                          // 接收数据变量
extern fifo_struct uart_data_fifo_4;
extern uint32 data_len_4;
extern uint32 fifo_data_count;

#endif