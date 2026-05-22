#ifndef _encoder_h
#define _encoder_h

#include "zf_common_headfile.h"

#define ENCODER_1                       (QTIMER2_ENCODER2)
#define ENCODER_1_LSB                   (QTIMER2_ENCODER2_CH1_C5)
#define ENCODER_1_DIR                   (QTIMER2_ENCODER2_CH2_C25)
                                        
#define ENCODER_2                       (QTIMER2_ENCODER1)
#define ENCODER_2_LSB                   (QTIMER2_ENCODER1_CH1_C3)
#define ENCODER_2_DIR                   (QTIMER2_ENCODER1_CH2_C4)
                                        
#define ENCODER_3                       (QTIMER1_ENCODER1)
#define ENCODER_3_LSB                   (QTIMER1_ENCODER1_CH1_C0)
#define ENCODER_3_DIR                   (QTIMER1_ENCODER1_CH2_C1)
                                        
#define ENCODER_4                       (QTIMER1_ENCODER2)
#define ENCODER_4_LSB                   (QTIMER1_ENCODER2_CH1_C2)
#define ENCODER_4_DIR                   (QTIMER1_ENCODER2_CH2_C24)

void encoder_init(void);
void encoder_get_val(void);
void distance_cal(void);

extern int16 encoder_data_1,encoder_data_2,encoder_data_3,encoder_data_4;
extern int16 RC_encoder_data_1;//滤波之后encoder的值
extern int16 RC_encoder_data_2;//滤波之后encoder的值
extern int16 RC_encoder_data_3;//滤波之后encoder的值
extern int16 RC_encoder_data_4;//滤波之后encoder的值
extern float wheel_distance1,wheel_distance2,wheel_distance3,wheel_distance4;
#endif