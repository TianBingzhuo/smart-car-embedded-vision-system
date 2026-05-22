/*
 * tft180.c
 *
 *  Created on: 2022年5月6日
 *      Author: dell
 */
#include "ips200.h"
#include "camera.h"
#include "zf_common_headfile.h"
#include "motor.h"
#include "tracking.h"
#include "u660.h"
#include "car_move.h"
#include "wir_usart.h"
//#include "icm.h"

extern int sanchachazhi;
extern ImageStatustypedef ImageStatus;                //图像的全局变量
extern int16 speed1;
extern int16 speed2;
extern uint16 adcleft;
extern uint16 adcmiddle;
extern uint16 adcright;
extern uint16 adcsum;
extern SystemDatatypdef SystemData;
extern int16 Speed_Goal_l;
extern int16 Speed_Goal_r;
extern int ycircle;
extern int xcircle;
extern int axcircle;
extern int aycircle;
extern int bxcircle;
extern int bycircle;
extern float ang_l;
extern int outacc_flag;
extern int Speed_Goal;
extern float Speed_gain;
extern int tt_distance;
extern uint16 L;
extern uint16 LM;
extern uint16 RM;
extern uint16 R;
extern float error_p;
extern float error_num;
extern float ADC_error;
extern int16 Speed_Encoder_r;//目标速度
extern int16 Speed_Encoder_l;//当前速度
extern int Right_RingsFlag_Point1_Ysite, Right_RingsFlag_Point2_Ysite; //右圆环判断的两点纵坐标
extern int Zebra_Lenth;
extern int ceshi_flag;
extern ImageFlagtypedef ImageFlag;
extern int16 v_error_final;//经过算法之后的error值
int16 vision_error;
int16 up_err;
extern int flag;

extern uint8_t have_card_flag;
extern uint8 temp_test_duty;
void ips200()
{
    ips200_show_gray_image(0,0,Pixle[0],LCDW,LCDH,80,60,1);             //二值化图像Pixle[0] 最后一个参数给1           第三个参数给Image_Use[0]，最后一个参数给0显示为灰度图像
    drawleftline();     //画左边线
    drawrightline();    //画右边线
	if(turn_flag==1){drawupline();}       //画上边界线
    drawcenterline();   //画中线
    drawoffline();      //画图像顶边
    drawtowpointUP();   //画前瞻范围上线,前瞻向上减5行
    drawtowpointDOWN(); //画前瞻范围下线,前瞻向下加5行
    ips200_show_string(0,60,"Err");
    ips200_show_int(30,60,ImageStatus.Det_True-ImageStatus.MiddleLine,3);
	
	
	
	up_err=ImageDeal[40].UpBorder-20;
    ips200_show_string(0,100,"left_line");
    ips200_show_int(80,100,border_line_left,3);
//    ips200_show_string(120,100,"right_line");
//    ips200_show_int(200,100,border_line_right,3);

    ips200_show_string(0,120,"cross_flag");
    ips200_show_int(80,120,ImageFlag.cross_flag,3);

    ips200_show_string(0,140,"obst_l");
    ips200_show_int(80,140,ImageFlag.left_obst_flag,3);
    ips200_show_string(120,140,"obst_r");
    ips200_show_int(200,140,ImageFlag.right_obst_flag,3);

//    ips200_show_string(0,160,"angle_z");
//    ips200_show_int(80,160,angleZ,3);

//    ips200_show_string(0,180,"R1");
//    ips200_show_int(80,180,Right_RingsFlag_Point1_Ysite,3);
//    ips200_show_string(120,180,"R2");
//     ips200_show_int(200,180,Right_RingsFlag_Point2_Ysite,3);

//     ips200_show_string(0,200,"ring");
//     ips200_show_int(80,200,ImageFlag.image_element_rings_flag,3);
////	 
//	 ips200_show_string(0,250,"tu_flag");
//     ips200_show_int(100,250,turn_s_ang_finish_flag,3);
//	 
//	 ips200_show_string(0,270,"mo_flag");
//     ips200_show_int(100,270,move_spe_dist_finish_flag,3);

    ips200_show_string(120,160,"duty3");
    ips200_show_int(200,160,temp_test_duty,3);

    ips200_show_string(0,160,"turn_fi");
    ips200_show_int(80,160,turn_s_ang_finish_flag,3);

    ips200_show_string(0,180,"ring_flag");
    ips200_show_int(80,180,ImageFlag.image_element_rings,3);
//    ips200_show_string(120,180,"h_card");
//     ips200_show_int(200,180,have_card_flag,3);
//	
//     ips200_show_string(0,200,"art_x_er");
//     ips200_show_int(80,200,op_x_er,3);
//	 
//	 ips200_show_string(0,250,"art_y_er");
//     ips200_show_int(100,250,op_y_er,3);
	 
	 ips200_show_string(120,0,"art_x_er");
     ips200_show_int(200,0,op_x_er,3);
	 ips200_show_string(120,20,"art_y_er");
     ips200_show_int(200,20,op_y_er,3);
	 ips200_show_string(120,40,"hv_cd_flag");
     ips200_show_int(200,40,have_card_flag,3);
	 ips200_show_string(120,60,"ou_cd_flag");
     ips200_show_int(200,60,out_card_flag,3);
	 ips200_show_string(120,80,"catch_step");
     ips200_show_int(200,80,cali_catch_card_step,3);
	 
	 ips200_show_string(120,100,"cross_dir");
     ips200_show_int(200,100,ImageFlag.cross_dir,3);
	 
//	 ips200_show_string(0,270,"hv_ca_f");
//     ips200_show_int(100,270,have_card_flag,3);
//	 ips200_show_string(0,270,"cam_step");
//     ips200_show_int(100,270,f_cir_step_flag,3);
	ips200_show_string(120,180,"center");
    ips200_show_int(200,180,ImageDeal[2].Center,3);
	 
	 ips200_show_string(120,290,"ring");
     ips200_show_int(200,290,ImageFlag.image_element_rings_flag,3);
}

void drawleftline()
{
    int x;
     for(int i=0;i<=59;i+=1)
     {
        x=ImageDeal[i].LeftBorder;
        ips200_draw_point(x,i,RGB565_RED);
     }
}

void drawrightline()
{
    int x;
      for(int i=0;i<=59;i+=1)
      {
          x=ImageDeal[i].RightBorder;
          ips200_draw_point(x,i,RGB565_RED);
      }
}

void drawcenterline()
{
    int x;
     for(int i=0;i<=58;i+=1)
     {
        x = (ImageDeal[i].RightBorder + ImageDeal[i].LeftBorder)/2;
        ips200_draw_point(x,i,RGB565_BLUE);
        
     }
}

void drawupline()
{
	int y;
	for (int i=20;i<60;i+=1)
	{
		y=ImageDeal[i].UpBorder;
		ips200_draw_point(i,y,RGB565_GREEN);
	}
}
void drawoffline()
{
    int x;
     for(int i=0;i<=79;i+=1)
     {
         x=ImageStatus.OFFLine;
         ips200_draw_point(i,x,RGB565_RED);
     }
}

void drawtowpointUP()
{
    int x;
      for(int i=0;i<=79;i+=1)
      {
          x=ImageStatus.TowPoint-5;
          ips200_draw_point(i,x,RGB565_CYAN);
      }
}

void drawtowpointDOWN()
{
    int x;
      for(int i=0;i<=79;i+=1)
      {
          x=ImageStatus.TowPoint+5;
          ips200_draw_point(i,x,RGB565_CYAN);
      }
}

void drawleftline_migong()
{
    int x;
     for(int i=0;i<=59;i+=1)
     {
        x=ImageDeal[i].LeftBoundary;
        ips200_draw_point(x,i,RGB565_BLUE);
     }
}

void drawrightline_migong()
{
    int x;
      for(int i=0;i<=59;i+=1)
      {
          x=ImageDeal[i].RightBoundary;
          ips200_draw_point(x,i,RGB565_PURPLE);
      }
}


