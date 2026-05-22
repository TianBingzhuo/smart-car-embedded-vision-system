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
#include "math.h"
#include "box_logic_code.h"
//用于定位要放入哪个仓位中
int box_location_in_str=0;
int box_location_in_cir_cro=5;

/********************
函数功能:排序函数
********************/
// 交换两个整数的函数  
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// 降序排序函数（这里使用简单的冒泡排序）  
void bubbleSortDescending(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// 找出出现次数最多的数字（这里假设没有重复元素）  
int findMostFrequent(int arr[], int n) {
    int maxCount = 1;  // 至少有一个元素  
    int mostFrequent = arr[0];  // 初始化为数组的第一个元素  

    for (int i = 1; i < n; i++) {
        int count = 1;  // 当前元素的计数  
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                count++;
                // 为了保持降序，将后面的相同元素移动到前面  
                for (int k = j; k > i; k--) {
                    swap(&arr[k], &arr[k - 1]);
                }
                // 因为已经移动了元素，所以不需要继续检查后面的元素  
                j = n;  // 跳出内层循环  
            }
        }
        if (count > maxCount) {
            maxCount = count;
            mostFrequent = arr[i];
        }
    }

    // 注意：这个函数在排序后也改变了数组的顺序（将相同的元素移到了一起）  
    // 但因为我们只关心最大值和它的出现次数，所以这不是问题  

    return mostFrequent;
}
// 函数定义：检查值是否存在于数组中  
int isValueInArray(box_message arr[], int size, int value) {  
    for (int i = 0; i < size; i++) {  
        if (arr[i].value == value) {  
            return i; // 找到值，返回true  
        }  
    }  
    return -1; // 没有找到值，返回false  
}
/********************
函数功能:卡片放入仓库函数
********************/
void put_in_box(int position)
{
	switch(position)
	{
		case 0:servo_slow_ctrl_360(0  ,SERVO_MOVE_SPEED);smotor_catch_card();break;
		case 1:servo_slow_ctrl_360(60 ,SERVO_MOVE_SPEED);smotor_catch_card();break;
		case 2:servo_slow_ctrl_360(120,SERVO_MOVE_SPEED);smotor_catch_card();break;
		case 3:servo_slow_ctrl_360(180,SERVO_MOVE_SPEED);smotor_catch_card();break;
		case 4:servo_slow_ctrl_360(240,SERVO_MOVE_SPEED);smotor_catch_card();break;
		case 5:servo_slow_ctrl_360(300,SERVO_MOVE_SPEED);smotor_catch_card();break;
		case 6:servo_slow_ctrl_360(360,SERVO_MOVE_SPEED);smotor_catch_card();break;
	}
	op_x_er=op_y_er=300;
}
/********************
函数功能:卡片放出仓库函数
********************/
void put_out_box(int position)
{
	switch(position)
	{
		case 0:servo_slow_ctrl_360(0  ,SERVO_MOVE_SPEED);smotor_put_card();break;
		case 1:servo_slow_ctrl_360(60 ,SERVO_MOVE_SPEED);smotor_put_card();break;
		case 2:servo_slow_ctrl_360(120,SERVO_MOVE_SPEED);smotor_put_card();break;
		case 3:servo_slow_ctrl_360(180,SERVO_MOVE_SPEED);smotor_put_card();break;
		case 4:servo_slow_ctrl_360(240,SERVO_MOVE_SPEED);smotor_put_card();break;
		case 5:servo_slow_ctrl_360(300,SERVO_MOVE_SPEED);smotor_put_card();break;
		case 6:servo_slow_ctrl_360(360,SERVO_MOVE_SPEED);smotor_put_card();break;
	}
	
}
/********************
函数功能:卡片小分类转为类(该函数该函数用于已经对准卡片后，使用)
********************/
int card_convert_small_to_big(int nums)
{
	int big_nums;
	if(nums>=1&&nums<=5){big_nums=1;}
	else if(nums>=6&&nums<=11){big_nums=2;}
	else if(nums>=12&&nums<=15){big_nums=3;}
	return big_nums;
}
/********************
函数功能:卡片分类(该函数该函数用于已经对准卡片后，使用)
********************/
uint8_t card_sorting_in_straight_step=0;

box_message boxs_in_str[6];
#define CARD_TEST_ARRAY_SIZE	(5)
int cards_test_array[CARD_TEST_ARRAY_SIZE];
int box_temp_pointer=0;
int temp_sort_alph;
int temp_card_message;
//在直道的情况(捡)
void card_sorting_in_straight(void)
{
	if(card_sorting_in_straight_step==0)
	{
		uart_write_byte(UART_INDEX_2,'1');
		if(op_reg_num!=0){card_sorting_in_straight_step=1;}
	}
	if(card_sorting_in_straight_step==1)
	{
		if(box_temp_pointer==CARD_TEST_ARRAY_SIZE)
		{
			card_sorting_in_straight_step=2;
			box_temp_pointer=0;
		}
		cards_test_array[box_temp_pointer]=op_reg_num;//这里可能有点问题，可能temp_pointer++的速度大于串口接受的速度。待验证。
	}
	if(card_sorting_in_straight_step==2)
	{
		int cards_test_array_size=sizeof(cards_test_array) / sizeof(cards_test_array[0]);
		bubbleSortDescending(cards_test_array, cards_test_array_size);
		temp_sort_alph=findMostFrequent(cards_test_array,cards_test_array_size);
		temp_card_message=isValueInArray(boxs_in_str,6,card_convert_small_to_big(temp_sort_alph));
		card_sorting_in_straight_step=3;
	}
	if(card_sorting_in_straight_step==3)
	{
		if(temp_card_message!=-1)
		{
			boxs_in_str[temp_card_message].nums++;
			put_in_box(boxs_in_str[temp_card_message].position);
			for(int i=0;i<5000;i++)
			{
				for(int j=0;j<1000;j++){};
			}
//			system_delay_ms(5);
			if(op_x_er==300&&smotor_catch_finish_flag==1)
			{
				card_sorting_in_straight_step=4;
				box_temp_pointer=0;
				smotor_catch_finish_flag=0;
			}
			else if(op_x_er!=300&&smotor_catch_finish_flag==1)
			{
				card_sorting_in_straight_step=1;
				box_temp_pointer=0;
			    memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
				smotor_catch_finish_flag=0;
			}
		}
		else if(temp_card_message==-1)
		{
			boxs_in_str[box_location_in_str].value=card_convert_small_to_big(temp_sort_alph);
			boxs_in_str[box_location_in_str].position=box_location_in_str;
			boxs_in_str[box_location_in_str].nums++;
			if(box_location_in_str>5){box_location_in_str=5;}
			put_in_box(box_location_in_str);
			for(int i=0;i<5000;i++)
			{
				for(int j=0;j<1000;j++){};
			}
//			system_delay_ms(5);
			if(op_x_er==300&&smotor_catch_finish_flag==1)
			{
				card_sorting_in_straight_step=4;
				box_temp_pointer=0;
				smotor_catch_finish_flag=0;
			}
			else if(op_x_er!=300&&smotor_catch_finish_flag==1)
			{
				card_sorting_in_straight_step=1;
				box_temp_pointer=0;
				memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
				smotor_catch_finish_flag=0;
			}
			box_location_in_str++;
		}
//		card_sorting_in_straight_step=4;
//		card_sorting_in_straight_step=5;
	}
	if(card_sorting_in_straight_step==4)
	{
		box_temp_pointer=0;
		memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
		uart_write_byte(UART_INDEX_2,'0');
		op_reg_num=0;
		op_reg_mode=0;
		card_sorting_in_straight_step=5;
		smotor_catch_finish_flag=0;
	}
}
//在直道的情况(放)
int put_card_in_str_step=0;
void put_card_in_str(void)
{
	if(put_card_in_str_step==0)
	{
		uart_write_byte(UART_INDEX_2,'3');
		if(op_reg_num!=0){put_card_in_str_step=1;}
	}
	if(put_card_in_str_step==1)
	{
//		if(box_temp_pointer==1)
//		{
			temp_sort_alph=op_reg_num;
			put_card_in_str_step=2;
//			box_temp_pointer=0;
//		}
//		cards_test_array[box_temp_pointer]=op_reg_num;//这里可能有点问题，可能temp_pointer++的速度大于串口接受的速度。待验证。
		
	}
	if(put_card_in_str_step==2)
	{
//		int cards_test_array_size=sizeof(cards_test_array) / sizeof(cards_test_array[0]);
//		bubbleSortDescending(cards_test_array, cards_test_array_size);
//		temp_sort_alph=findMostFrequent(cards_test_array,cards_test_array_size);
		int temp_card_podsition_str=0;
		for(int i=0;i<3;i++)
		{
			if(boxs_in_str[i].value==temp_sort_alph)
			{
				temp_card_podsition_str=i;
				break;
			}
		}
		while(boxs_in_str[temp_card_podsition_str].nums!=0)
		{
			put_out_box(boxs_in_str[temp_card_podsition_str].position);
			boxs_in_str[temp_card_podsition_str].nums--;
		}
		put_card_in_str_step=3;
	}
	if(put_card_in_str_step==3)
	{
		uart_write_byte(UART_INDEX_2,'0');
		box_temp_pointer=0;
		memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
		smotor_catch_finish_flag=0;
		op_reg_num=0;
		op_reg_mode=0;
//		put_card_in_str_step=4;
		put_card_in_str_step=0;//测试专用
	}
}
//圆环与十字情况(捡)
int card_sorting_cir_cro_step=0;
box_message boxs_in_cir_cro[6];

void card_sorting_cir_cro(void)
{
	if(card_sorting_cir_cro_step==0)
	{
		uart_write_byte(UART_INDEX_2,'1');
		if(op_reg_num!=0){card_sorting_cir_cro_step=1;}
	}
	if(card_sorting_cir_cro_step==1)
	{
		if(box_temp_pointer==CARD_TEST_ARRAY_SIZE)
		{
			card_sorting_cir_cro_step=2;
			box_temp_pointer=0;
		}
		cards_test_array[box_temp_pointer]=op_reg_num;//这里可能有点问题，可能temp_pointer++的速度大于串口接受的速度。待验证。
	}
	if(card_sorting_cir_cro_step==2)
	{
		int cards_test_array_size=sizeof(cards_test_array) / sizeof(cards_test_array[0]);
		bubbleSortDescending(cards_test_array, cards_test_array_size);
		temp_sort_alph=findMostFrequent(cards_test_array,cards_test_array_size);
		temp_card_message=isValueInArray(boxs_in_cir_cro,6,temp_sort_alph);
		card_sorting_cir_cro_step=3;
	}
	if(card_sorting_cir_cro_step==3)
	{
		if(temp_card_message!=-1)
		{
			boxs_in_cir_cro[temp_card_message].nums++;
			put_in_box(boxs_in_cir_cro[temp_card_message].position);
			for(int i=0;i<5000;i++)
			{
				for(int j=0;j<1000;j++){};
			}
//			system_delay_ms(5);
			if(op_x_er==300&&smotor_catch_finish_flag==1)
			{
				card_sorting_cir_cro_step=4;
				box_temp_pointer=0;
				smotor_catch_finish_flag=0;
			}
			else if(op_x_er!=300&&smotor_catch_finish_flag==1)
			{
				card_sorting_cir_cro_step=1;
				box_temp_pointer=0;
			    memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
				smotor_catch_finish_flag=0;
			}
		}
		else if(temp_card_message==-1)
		{
			boxs_in_cir_cro[box_location_in_cir_cro].value=card_convert_small_to_big(temp_sort_alph);
			boxs_in_cir_cro[box_location_in_cir_cro].position=box_location_in_cir_cro;
			boxs_in_cir_cro[box_location_in_cir_cro].nums++;
			if(box_location_in_cir_cro<0){box_location_in_cir_cro=0;}
			put_in_box(box_location_in_cir_cro);
			for(int i=0;i<5000;i++)
			{
				for(int j=0;j<1000;j++){};
			}
//			system_delay_ms(5);
			if(op_x_er==300&&smotor_catch_finish_flag==1)
			{
				card_sorting_cir_cro_step=4;
				box_temp_pointer=0;
				smotor_catch_finish_flag=0;
			}
			else if(op_x_er!=300&&smotor_catch_finish_flag==1)
			{
				card_sorting_cir_cro_step=1;
				box_temp_pointer=0;
				memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
				smotor_catch_finish_flag=0;
			}
			box_location_in_cir_cro--;
		}
//		card_sorting_in_straight_step=4;
//		card_sorting_in_straight_step=5;
	}
	if(card_sorting_cir_cro_step==4)
	{
		box_temp_pointer=0;
		memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
		uart_write_byte(UART_INDEX_2,'0');
		op_reg_num=0;
		op_reg_mode=0;
		card_sorting_cir_cro_step=5;
		smotor_catch_finish_flag=0;
	}
}
//在圆环于十字的情况(放)
int put_card_in_cir_cro_step=0;
void put_card_in_cir_cro(void)
{
//	if(put_card_in_cir_cro_step==0)
//	{
//		uart_write_byte(UART_INDEX_2,'2');
//		if(data_len_2!= 0){put_card_in_cir_cro_step=1;}
//	}
//	if(put_card_in_cir_cro_step==1)
//	{
//		if(box_temp_pointer==20)
//		{
//			put_card_in_cir_cro_step=2;
//			box_temp_pointer=0;
//		}
//		cards_test_array[box_temp_pointer++]=op_reg_num;//这里可能有点问题，可能temp_pointer++的速度大于串口接受的速度。待验证。
//	}
//	if(put_card_in_cir_cro_step==2)
//	{
//		int cards_test_array_size=sizeof(cards_test_array) / sizeof(cards_test_array[0]);
//		bubbleSortDescending(cards_test_array, cards_test_array_size);
//		temp_sort_alph=findMostFrequent(cards_test_array,cards_test_array_size);
//		int temp_card_podsition_cir_cro=0;
//		for(int i=0;i<15;i++)
//		{
//			if(boxs_in_str[i].value==temp_sort_alph)
//			{
//				temp_card_podsition_cir_cro=i;
//				break;
//			}
//		}
//		while(boxs_in_str[temp_card_podsition_cir_cro].nums!=0)
//		{
//			put_out_box(boxs_in_str[temp_card_podsition_cir_cro].position);
//			boxs_in_str[temp_card_podsition_cir_cro].nums--;
//		}
//		put_card_in_cir_cro_step=3;
//	}
//	if(put_card_in_cir_cro_step==3)
//	{
//		uart_write_byte(UART_INDEX_2,'0');
//		put_card_in_cir_cro_step=0;
//		box_temp_pointer=0;
//		memset(cards_test_array,0,20);
//	}
	
	
	
	if(put_card_in_cir_cro_step==0)
	{
		uart_write_byte(UART_INDEX_2,'2');
		if(op_reg_num!=0){put_card_in_cir_cro_step=1;}
	}
	if(put_card_in_cir_cro_step==1)
	{
		temp_sort_alph=op_reg_num;
		put_card_in_cir_cro_step=2;
		
	}
	if(put_card_in_cir_cro_step==2)
	{
		int temp_card_podsition_cir_cro=0;
		for(int i=0;i<3;i++)
		{
			if(boxs_in_cir_cro[i].value==temp_sort_alph)
			{
				temp_card_podsition_cir_cro=i;
				break;
			}
		}
		while(boxs_in_cir_cro[temp_card_podsition_cir_cro].nums!=0)
		{
			put_out_box(boxs_in_cir_cro[temp_card_podsition_cir_cro].position);
			boxs_in_cir_cro[temp_card_podsition_cir_cro].nums--;
		}
		put_card_in_cir_cro_step=3;
	}
	if(put_card_in_cir_cro_step==3)
	{
		uart_write_byte(UART_INDEX_2,'0');
		put_card_in_cir_cro_step=0;
		box_temp_pointer=0;
		memset(cards_test_array,0,CARD_TEST_ARRAY_SIZE);
		smotor_catch_finish_flag=0;
		op_reg_num=0;
		op_reg_mode=0;
		put_card_in_str_step=4;
//		put_card_in_cir_cro_step=0;//测试专用
	}
}
