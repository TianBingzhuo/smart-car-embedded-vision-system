#ifndef _tracking_h
#define _tracking_h

#include "zf_common_headfile.h"

void move_transfrom(double vx,double vy,double vz);
void tr_straight_move(int16_t v_error);
void tr_turnround_move(void);
void storage_carmove(void);
void back_carmove(void);

extern float str_carmove_dist;
extern int str_carmove_angle,move_test_flag;

extern float str_carmove_dist;
extern int str_carmove_angle;
extern int move_test_flag;

extern double targeta,targetb,targetc,targetd;

#endif