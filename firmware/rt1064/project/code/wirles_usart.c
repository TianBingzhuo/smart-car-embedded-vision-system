#include "wirles_usart.h"
#include "motor.h"
#include "wir_usart.h"
#include "car_move.h"
#include "camera.h"
#include "ips200.h"
#include "u660.h"
#include "encoder.h"
#include "smotor.h"
#include "box_logic_code.h"
extern uint8 temp_test_duty;
void wirles_usart_init(void)
{
	seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);
	wireless_uart_init();
}
extern int16 RC_encoder_data_1,RC_encoder_data_2,RC_encoder_data_3,RC_encoder_data_4;
extern double temp_op_disc;
extern uint8_t card_sorting_in_straight_step,put_card_in_str_step,card_sorting_cir_cro_step,put_card_in_cir_cro_step;
extern int temp_sort_alph,temp_card_message;
void show_oscilloscope_wirlesusart(void)
{
	oscilloscope_data.data[0] = out_card_flag;
	oscilloscope_data.data[1] = op_reg_mode;//op_reg_mode
	oscilloscope_data.data[2] = op_reg_num;
	oscilloscope_data.data[3] = temp_sort_alph;
	oscilloscope_data.data[4] = card_sorting_cir_cro_step;
	oscilloscope_data.data[5] = put_card_in_cir_cro_step;
	if(temp_card_message==-1){oscilloscope_data.data[6] = boxs_in_str[box_location_in_str].position;}
	if(temp_card_message!=-1){oscilloscope_data.data[6] = boxs_in_str[temp_card_message].position;}
	oscilloscope_data.data[7] = box_temp_pointer;
	oscilloscope_data.channel_num =8;
	
//	oscilloscope_data.data[0] = op_x_er;
//    oscilloscope_data.data[1] = op_y_er;
//	oscilloscope_data.data[2] = have_card_flag;
//	oscilloscope_data.data[3] = out_card_flag;
//	oscilloscope_data.data[4] = f_cross_step_flag;
//	oscilloscope_data.data[5] = car_move_mode;
//	//oscilloscope_data.data[6] = temp_op_disc;
//	oscilloscope_data.data[6] = anglez_trsl_in_cir(angleZ);
//	oscilloscope_data.data[7] = car_move_mode;
//	oscilloscope_data.data[7] = temp_op_anglez;
//	oscilloscope_data.channel_num = 6;
	extern double temp_cir_disc,temp_cross_disc;
	extern int dir_cir_flag;
//	oscilloscope_data.data[0] = op_x_er;
//	oscilloscope_data.data[1] = op_y_er;
//	oscilloscope_data.data[2] = f_cir_step_flag;
//	oscilloscope_data.data[3] = have_card_flag;
//	oscilloscope_data.data[4] = car_move_mode;
//	oscilloscope_data.data[5] = have_card_flag;
//	oscilloscope_data.data[6] = temp_cir_disc;
//	oscilloscope_data.data[7] = dir_cir_flag;
//	oscilloscope_data.channel_num = 8;
//	oscilloscope_data.data[0] = ImageFlag.cross_flag;
//	oscilloscope_data.data[1] = vision_error;
//	oscilloscope_data.data[2] = f_cross_step_flag;
//	oscilloscope_data.data[3] = have_card_flag;
//	oscilloscope_data.data[4] = car_move_mode;
//	oscilloscope_data.data[5] = turn_s_ang_finish_flag;
//	oscilloscope_data.data[6] = out_card_flag;
//	oscilloscope_data.data[7] = (uint32_t)SERVO_MOTOR_DUTY(temp_test_duty);
//	oscilloscope_data.channel_num = 8;
//	oscilloscope_data.data[0] = RC_encoder_data_1;
//	oscilloscope_data.data[1] = RC_encoder_data_2;
//	oscilloscope_data.data[2] = RC_encoder_data_3;
//	oscilloscope_data.data[3] = RC_encoder_data_4;
//	oscilloscope_data.data[4] = angleZ;
//	oscilloscope_data.data[5] = have_card_flag;
//	oscilloscope_data.data[6] = temp_cir_disc;
//	oscilloscope_data.data[7] = dir_cir_flag;
//	oscilloscope_data.channel_num = 5;

//	oscilloscope_data.data[0] = move_spe_dist_finish_flag_x;
//	oscilloscope_data.data[1] = move_spe_dist_finish_flag;
//	oscilloscope_data.channel_num = 2;
	
//	int temp_op_x_dist=op_x_dist*1000;
//	int temp_op_y_dist=op_y_dist*1000;
	extern int op_turn_dir;
//	oscilloscope_data.data[0] = cir_move_x_error;
//	oscilloscope_data.data[1] = cir_move_z_error;
//	oscilloscope_data.data[2] = op_turn_dir;
////	oscilloscope_data.data[4] = (int)(wheel_distance3*1000);
////	oscilloscope_data.data[5] = (int)(wheel_distance4*1000);
//	oscilloscope_data.channel_num = 3;
	
	seekfree_assistant_oscilloscope_send(&oscilloscope_data);
}
