#include "encoder.h"
#include "motor.h"
#include "filter.h"
#include "location.h"
#include "flash_param.h"
#include "u660.h"
#include "car_move.h"
int16 encoder_data_1 = 0,RC_encoder_data_1;//滤波之后encoder的值
int16 encoder_data_2 = 0,RC_encoder_data_2;//滤波之后encoder的值
int16 encoder_data_3 = 0,RC_encoder_data_3;//滤波之后encoder的值
int16 encoder_data_4 = 0,RC_encoder_data_4;//滤波之后encoder的值
float wheel_distance1,wheel_distance2,wheel_distance3,wheel_distance4;//车轮速度
//---------------------结构体---------------------//
struct RC_Para Encoder1_Para = {0,0,0.25};
struct RC_Para Encoder2_Para = {0,0,0.25};
struct RC_Para Encoder3_Para = {0,0,0.25};
struct RC_Para Encoder4_Para = {0,0,0.25};

RC_Filter_pt RC_Encoder1 = &Encoder1_Para;
RC_Filter_pt RC_Encoder2 = &Encoder2_Para;
RC_Filter_pt RC_Encoder3 = &Encoder3_Para;
RC_Filter_pt RC_Encoder4 = &Encoder4_Para;

void omni_mileage()// 计算全方位移动距离
{
    float detax=0,detay=0;
    detax=(float)(RC_encoder_data_1 - RC_encoder_data_2 + RC_encoder_data_3 - RC_encoder_data_4)/4;
    detay=(float)(RC_encoder_data_1 + RC_encoder_data_2 + RC_encoder_data_3 + RC_encoder_data_4)/4;
    Car.MileageX+=(float)(detax*MileageKx);
    Car.MileageY+=(float)(detay*MileageKy);
}

void encoder_init(void)
{
	encoder_dir_init(ENCODER_1, ENCODER_1_LSB, ENCODER_1_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
    encoder_dir_init(ENCODER_2, ENCODER_2_LSB, ENCODER_2_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
    encoder_dir_init(ENCODER_3, ENCODER_3_LSB, ENCODER_3_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
    encoder_dir_init(ENCODER_4, ENCODER_4_LSB, ENCODER_4_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
}

void encoder_get_val(void)		
{
	encoder_data_1 = encoder_get_count(ENCODER_1);                              // 获取编码器计数
   
    encoder_data_2 = encoder_get_count(ENCODER_2);                              // 获取编码器计数
    
    encoder_data_3 = encoder_get_count(ENCODER_3);                              // 获取编码器计数
    
    encoder_data_4 = encoder_get_count(ENCODER_4);                              // 获取编码器计数
   
//	omni_mileage();

    RC_encoder_data_1 = (int16_t)RCFilter(encoder_data_1,RC_Encoder1);	        //滤波之后encoder的值
    RC_encoder_data_2 = (int16_t)RCFilter(encoder_data_2,RC_Encoder2);          //滤波之后encoder的值
    RC_encoder_data_3 = (int16_t)RCFilter(encoder_data_3,RC_Encoder3);          //滤波之后encoder的值
    RC_encoder_data_4 = (int16_t)RCFilter(encoder_data_4,RC_Encoder4);          //滤波之后encoder的值
    
    encoder_clear_count(ENCODER_1);                                             // 清空编码器计数
	encoder_clear_count(ENCODER_2);                                             // 清空编码器计数
	encoder_clear_count(ENCODER_3);                                             // 清空编码器计数
	encoder_clear_count(ENCODER_4);                                             // 清空编码器计数
}

void distance_cal(void)//用于斑马线停车
{
	wheel_distance1=(RC_encoder_data_1/1024.00)*(45.00/104.00)*2*PI*0.03;		//最好整数后加小数点，这样才能算出浮点数
	wheel_distance2=-(RC_encoder_data_2/1024.00)*(45.00/104.00)*2*PI*0.03;       //最好整数后加小数点，这样才能算出浮点数
	wheel_distance3=(RC_encoder_data_3/1024.00)*(45.00/104.00)*2*PI*0.03;       //最好整数后加小数点，这样才能算出浮点数
	wheel_distance4=-(RC_encoder_data_4/1024.00)*(45.00/104.00)*2*PI*0.03;       //最好整数后加小数点，这样才能算出浮点数
	
}





