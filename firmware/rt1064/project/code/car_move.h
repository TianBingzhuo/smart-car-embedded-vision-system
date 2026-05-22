#ifndef CAR_MOVE_H_
#define CAR_MOVE_H_

#include "zf_common_headfile.h"
#define CAR_SPEED     (150)
void car_move_spe_dist(double dist);
void tr_circal_move(int16_t x_error,int16_t z_error);
void carmove_in_circular(void);
void car_find_card(void);
void op_go_for_card(void);
void op_dist_judge(void);
void car_move_part(void);
void art_catch_card_convert_speed(int temp_angle,int speed,int mode);
void car_move_spe_dist_x(double dist);
void op_find_card_move(int16_t x_error,int16_t z_error,int op_turn_dir);
void carmove_in_cross(void);

extern double dist_c_s_error;
extern double dist_l_s_error;
extern double dist_s_kp;
extern double dist_s_kd;
extern int dist_s_pwm_out;
extern int set_go_s_straight_speed;
extern double move_s_dist_c;
extern int catch_card_flag,put_card_flag,somtor_c_finish_flag,somtor_p_finish_flag,cir_tramove_start_flag,step_flag;
extern int cir_move_x_error,cir_move_z_error;
extern double kx;
extern double ky;
extern double kz;
extern int set_cir_y_speed;
extern int out_circal_flag,normal_tracking_flag,enter_circular_flag,move_spe_dist_finish_flag,temp_str_angleZ;
extern uint8_t op_smotor_catch_flag;
extern uint8_t op_smotor_put_flag;
extern uint8_t f_c_step_flag;
extern uint8_t use_art_num;
extern int temp_op_anglez;
extern int op_x_error,op_y_error,op_x_1_error;
extern uint8_t f_cir_step_flag;
extern int temp_op_y_er;
extern int temp_op_x_er;
extern uint8_t x_calibrat_finish_flag;
extern uint8_t y_calibrat_finish_flag;
extern double op_x_dist,op_y_dist;
extern uint8_t car_move_mode;
extern uint8_t have_card_flag;			//检测到卡片标志位
extern uint8_t cali_catch_card_step;
extern int move_spe_dist_finish_flag_x;
extern uint8_t f_cross_step_flag;
#endif 
