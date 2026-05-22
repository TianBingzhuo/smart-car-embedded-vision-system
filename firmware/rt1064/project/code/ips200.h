/*
 * tft180.h
 *
 *  Created on: 2022年5月6日
 *      Author: dell
 */

#ifndef IPS200_H_
#define IPS200_H_

#include "zf_common_headfile.h"

void ips200(void);
void drawleftline(void);          //画左边线
void drawrightline(void);         //画右边线
void drawleftline_migong();        //迷宫扫线左边线
void drawrightline_migong();       //迷宫扫线右边界
void drawupline();                 //画上边界线
void drawcenterline(void);        //画中线
void drawoffline(void);           //画图像顶边
void drawtowpointUP(void);        //画前瞻范围上线
void drawtowpointDOWN(void);      //画前瞻范围下线
void adcview(void);

extern int16 vision_error;

#endif /* IPS200_H_ */
