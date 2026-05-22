#include "wifi.h"
#include "string.h"
#include "stdio.h"

uint8 wifi_spi_test_buffer[] = "this is wifi spi test buffer";
uint8 wifi_spi_get_data_buffer[256];
uint16 data_length;

extern int flag;

/*****************
输入一个字符为开始，两个字符为停止,这是利用通信的原理来控制
*****************/

void wifi_sand(void)
{
	data_length = wifi_spi_read_buffer(wifi_spi_get_data_buffer, sizeof(wifi_spi_get_data_buffer));
	if(data_length)                                                  	// 如果接收到数据 则进行数据类型判断
	{
		flag=data_length;		
		if(!wifi_spi_send_buffer(wifi_spi_get_data_buffer, data_length))
		{
			memset(wifi_spi_get_data_buffer, 0, data_length);          	// 数据发送完成 清空数据
		}
		
	}
	system_delay_ms(5);
}
/*****************
这是利用调参的相关来控制
*****************/
void wifi_set_parameter(void)
{
	// 滴答客解析接收到的数据
    seekfree_assistant_data_analysis();
}

void wifi_part(void)
{
	wifi_sand();
	if(seekfree_assistant_parameter[0]==250)
	{
		gpio_set_level(B9, 0);              // 设置引脚电平为高电平
	}
	
	if(seekfree_assistant_parameter[1]==350)
	{
		gpio_set_level(B9, 1);
	}
}













