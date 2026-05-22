#include "wir_usart.h"
#include "car_move.h"
#include "box_logic_code.h"

int8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8_t fifo_get_data;   //用于缓冲区                                                   // fifo 输出读出缓冲区                            
uint8_t get_data = 0;   //用于最开始串口读数据使用                                                          // 接收数据变量
fifo_struct uart_data_fifo;
uint32 data_len;
uint32 fifo_data_count = 0;                                                     // fifo 数据个数

int8 uart_get_data_2[64];                                                        // 串口接收数据缓冲区
uint8_t fifo_get_data_2;   //用于缓冲区                                                   // fifo 输出读出缓冲区                            
uint8_t get_data_2 = 0;   //用于最开始串口读数据使用                                                          // 接收数据变量
fifo_struct uart_data_fifo_2;
uint32 data_len_2;
uint32 fifo_data_count_2 = 0;                                                     // fifo 数据个数

int8 uart_get_data_4[64];                                                        // 串口接收数据缓冲区
uint8_t fifo_get_data_4;   //用于缓冲区                                                   // fifo 输出读出缓冲区                            
uint8_t get_data_4 = 0;   //用于最开始串口读数据使用                                                          // 接收数据变量
fifo_struct uart_data_fifo_4;
uint32 data_len_4;
uint32 fifo_data_count_4 = 0;                                                     // fifo 数据个数

static int rx_num   = 0;//存数据标志位
static int rx_num_2 = 0;//存数据标志位
static int rx_num_4 = 0;//存数据标志位

bool isDetection=false;//是否进入分类图片
int temp;
int op_x_er=300;
int op_y_er=300;
int op_x_er_counts=0;
int op_y_er_counts=0;
int op_x_er_tt=0;
int op_y_er_tt=0;

#define	x_calibration_value	(51)
#define	y_calibration_value	(38)//37

void wir_usart_init(void)
{
	fifo_init(&uart_data_fifo_4, FIFO_DATA_8BIT, uart_get_data_4, 64);              // 初始化 fifo 挂载缓冲区
    uart_init(UART_INDEX_4, UART_BAUDRATE, UART_TX_PIN_4, UART_RX_PIN_4);             
    uart_rx_interrupt(UART_INDEX_4, ZF_ENABLE);                                   // 开启 UART_INDEX 的接收中断
    interrupt_set_priority(UART_PRIORITY_4, 0);   	// 设置对应 UART_INDEX 的中断优先级为 0
		                             // 输出测试信息
    uart_write_byte(UART_INDEX_4, '\r');                                          // 输出回车
    uart_write_byte(UART_INDEX_4, '\n');

	
	
	fifo_init(&uart_data_fifo_2, FIFO_DATA_8BIT, uart_get_data_2, 64);              // 初始化 fifo 挂载缓冲区
    uart_init(UART_INDEX_2, UART_BAUDRATE, UART_TX_PIN_2, UART_RX_PIN_2);             
    uart_rx_interrupt(UART_INDEX_2, ZF_ENABLE);                                   // 开启 UART_INDEX 的接收中断
    interrupt_set_priority(UART_PRIORITY_2, 0);   	// 设置对应 UART_INDEX 的中断优先级为 0
		                             // 输出测试信息
    uart_write_byte(UART_INDEX_2, '\r');                                          // 输出回车
    uart_write_byte(UART_INDEX_2, '\n');                 


	fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区
    uart_init(UART_INDEX_1, UART_BAUDRATE, UART_TX_PIN_1, UART_RX_PIN_1);             
    uart_rx_interrupt(UART_INDEX_1, ZF_ENABLE);                                   // 开启 UART_INDEX 的接收中断
    interrupt_set_priority(UART_PRIORITY_1, 0);   	// 设置对应 UART_INDEX 的中断优先级为 0
		                             // 输出测试信息
    uart_write_byte(UART_INDEX_1, '\r');                                          // 输出回车
    uart_write_byte(UART_INDEX_1, '\n');
	
}
void wir_usart_re_4(void)
{
	uart_query_byte(UART_INDEX_4, &get_data_4);                    // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE//先用串口读然后送到缓冲区然后读缓冲区，读法同无线串口
	fifo_write_buffer(&uart_data_fifo_4, &get_data_4, 1);     
    data_len_4 = fifo_used(&uart_data_fifo_4);   	// 查看 fifo 是否有数据
				
    if(data_len_4!= 0)                                                // 读取到数据了
    {	
		fifo_read_buffer(&uart_data_fifo_4, &fifo_get_data_4, &data_len_4, FIFO_READ_AND_CLEAN);   // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
		if(fifo_get_data_4=='z')
		{
			isDetection=true;
//			gpio_set_level(B9,1);
						
						
		}
		else if(fifo_get_data_4=='w')
		{
			rx_num_4=0;
			isDetection=false;
//			gpio_set_level(B9,0);
					
		}//必须在帧尾之后加加，上述帧头帧尾代码后期可改 
		else
		{	
			//加入低通滤波
			rx_num_4++;
			if (isDetection)
			{
				if(rx_num_4==1)
				{
					op_x_er=fifo_get_data_4;
					/*
					这里在art传出的数据为int类型而接受的fifo_get_data为uinr类型所以会转化为256的形式，想要得负数只能在一定会范围时减去
					256，但是这个范围不能影响到正数的取值范围。还有一个解决方法是将fifo_get_data转化为int类型，但是相关函数又会报警告。
					*/
					if(op_x_er>200){op_x_er-=256;}
//					if(op_x_er>=120){op_x_er=120;}	 //做一个小限幅
//					if(op_x_er<=0){op_x_er=0;}       //做一个小限幅
					op_x_er-=x_calibration_value;
					if(op_x_er>=53){op_x_er=53;}	 //做一个小限幅
					if(op_x_er<=-53){op_x_er=-53;}
//					op_x_er_tt+=fifo_get_data;
//					op_x_er_counts++;
//					if(op_x_er_counts==2)
//					{
//						op_x_er=op_x_er_tt/2;
//						op_x_er_counts=0;
//						op_x_er_tt=0;
//					}
				}
				else if(rx_num_4==2)
				{
					op_y_er=fifo_get_data_4;
					op_y_er-=y_calibration_value;
//					op_y_er_tt+=fifo_get_data;
//					op_y_er_counts++;
//					if(op_y_er_counts==5)
//					{
//						op_y_er=op_y_er_tt/5;
//						op_y_er_counts=0;
//						op_y_er_tt=0;
//					}
				}
			}
		}
	}
}
uint8_t	op_reg_num=0;	//接受的数字，不代表任何含义
uint8_t	op_reg_mode=-1;	//接收到的数字，代表模式
uint8_t	op_reg_alph=0;	//接收到的数字，代表相应的字母
uint8_t	op_reg_small_cate=0;//接收到的数字，代表识别卡片的小种类的数字		
uint8_t	op_reg_big_cate=0;//接收到的数字，代表识别卡片的大种类的数字	

void wir_usart_re_2(void)
{
	uart_query_byte(UART_INDEX_2, &get_data_2);                    // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE//先用串口读然后送到缓冲区然后读缓冲区，读法同无线串口
	fifo_write_buffer(&uart_data_fifo_2, &get_data_2, 1);     
    data_len_2 = fifo_used(&uart_data_fifo_2);   	// 查看 fifo 是否有数据
				
    if(data_len_2!= 0)                                                // 读取到数据了
    {	
		fifo_read_buffer(&uart_data_fifo_2, &fifo_get_data_2, &data_len_2, FIFO_READ_AND_CLEAN);   // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
		if(fifo_get_data_2=='z')
		{
			isDetection=true;
//			gpio_set_level(B9,1);
						
						
		}
		else if(fifo_get_data_2=='w')
		{
			rx_num_2=0;
			isDetection=false;
//			gpio_set_level(B9,0);
					
		}//必须在帧尾之后加加，上述帧头帧尾代码后期可改 
		else
		{	
			//加入低通滤波
			rx_num_2++;
			if (isDetection)
			{
				if(rx_num_2==1)
				{
					op_reg_mode=fifo_get_data_2;
				}
				else if(rx_num_2==2)
				{
					op_reg_num=fifo_get_data_2;
					box_temp_pointer++;
				}
			}
		}
	}
}



/********************
函数功能:检测是否有卡片
********************/
uint8_t out_card_flag=1;
int op_x_error_storage=0;
int op_x_error_storage_counts=0;
void check_card_exist(void)
{
	/*
	假如放在5ms中断内
	*/
	if(op_x_error_storage_counts==100)
	{
		out_card_flag=1;
		op_x_error_storage_counts=0;
	}
	if(op_x_er==300)
	{
		op_x_error_storage_counts++;
	}
	if(op_x_er!=300)
	{
		op_x_error_storage_counts=0;
		out_card_flag=0;
	}
//	if(op_x_error_storage_counts!=0)
//	{
//		if(op_x_error_storage==op_x_er)
//		{
//			op_x_error_storage_counts++;
//		}
//		else if(op_x_error_storage!=op_x_er)
//		{
//			if(op_x_error_storage_counts>=1&&op_x_error_storage_counts<200){out_card_flag=0;}
//			op_x_error_storage_counts=0;
//			op_x_error_storage=0;
//		}
//	}
//	/*
//	假如放在5ms中断内
//	*/
//	if(op_x_error_storage_counts==200)
//	{
//		out_card_flag=1;
//		//have_card_flag=0;
//		//car_move_mode=0;
//		op_x_error_storage_counts=0;
//		op_x_error_storage=0;
//	}
//	
//	if(op_x_error_storage_counts==0)
//	{
//		op_x_error_storage=op_x_er;
//		op_x_error_storage_counts++;
//	}
}
