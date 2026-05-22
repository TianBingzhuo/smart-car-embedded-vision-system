/*
 * Fuzzy.c
 *
 *  Created on: 2022年5月6日
 *      Author: dell
 */

#include "Fuzzy.h"
#include "camera.h"
#include "steer.h"
//extern PID_Datatypedef SteerPIDdata;    //舵机的PID参数
#define H_Min   2
extern uint16 adcsum;
extern SystemDatatypdef SystemData;
float Left_Speed_Co = 0.6, Right_Speed_Co = 0.6;

void Data_Settings()           //参数赋值
{
  //图像参数
//  extern uint8_t Fork_in_1, Fork_in_2;
//  Fork_in_1 = Fork_in_2 = 'F';
  adcsum=0;
  ImageStatus.MiddleLine = 41;
//  ImageStatus.TowPoint_Gain = 0.2;
  ImageStatus.TowPoint_Offset_Max = 5;
  ImageStatus.TowPoint_Offset_Min = -2;
  ImageStatus.TowPointAdjust_v = 160;
  ImageStatus.Det_all_k = 0.7;  //待定
  ImageStatus.CirquePass = 'F';
  ImageStatus.IsCinqueOutIn = 'F';
  ImageStatus.CirqueOut = 'F';
  ImageStatus.CirqueOff = 'F';
  ImageStatus.Barn_Flag = 0;
  ImageStatus.sanchaju = 0;
  ImageStatus.straight_acc = 0;

  ImageStatus.TowPoint =35;           //前瞻//重点去调这个参数；这个值越大转向越晚，越小转向越提前
  ImageStatus.Threshold_static = 70;   //静态阈值（不要改）大津法；0~255    0是黑 255是最亮   虽然大津法会自动获取阈值，但是如果碰到亮斑干扰就需要这个参数
  ImageStatus.Threshold_detach = 180;  //阳光算法  亮斑分离（不要改）
  ImageScanInterval = 2;               //扫边范围（断边自动补线的范围）    上一行的边界+-ImageScanInterval
  ImageScanInterval_Cross = 2;         //十字扫线范围          如果数值太大的话十字有可能出去
 // ImageStatus.variance = 26;           //直道方差阈值
  ImageStatus.variance_acc = 30;       //直道加速检测的方差阈值
//  SystemData.outbent_acc  =  5;
  SystemData.clrcle_num=0;
  ImageStatus.newblue_flag=0;
  SystemData.SpeedData.MaxSpeed=100;
  SystemData.SpeedData.MinSpeed=90;


  /**位置式pid参数**/        //舵机位置式PID参数
//  SteerPIDdata.P = 2.5;  //2.8
//  SteerPIDdata.I = 0.0;
//  SteerPIDdata.D = 8.1;//原先5.1

//    steer_middle = 513;  //舵机中值440
//    steer_left = 435;    //左转   372
//    steer_right = 594;   //右转   505

}
