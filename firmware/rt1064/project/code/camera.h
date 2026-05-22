/*
 * C_H.h
 *
 *  Created on: 2022年5月6日
 *      Author: dell
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#include "motor.h"
#include "zf_common_headfile.h"



#define LCDH 60                              //用于图像处理图像的高度
#define LCDW 80                              //用于图像处理图像的宽度
#define uLCDH 120                            //用于图像显示的图像高度
#define uLCDW 160                            //用于图像显示的图像宽度
#define HMAX (LCDH - 1)                      //没用
#define WMAX (LCDW - 1)                      //没用
#define LimitL(L) (L = ((L < 1) ? 1 : L))    //限制幅度
#define LimitH(H) (H = ((H > 78) ? 78 : H))  //限制幅度
#define ImageSensorMid 39                    //图像屏幕中点
extern int ImageScanInterval;  //扫边范围    上一行的边界+-ImageScanInterval
extern int ImageScanInterval_Cross;  //十字扫边范围
typedef struct {
  int point;
  uint8 type;
} JumpPointtypedef;

//每一行的属性
typedef struct {
  /*左右边边界标志    T为正常跳变边    W为无边   P为障碍类多跳边的内边*/
  uint8 IsRightFind;      //右边有边标志
  uint8 IsLeftFind;       //左边有边标志
  uint8 isBlackFind;      //三叉边
  int Wide;               //边界宽度
  int LeftBorder;         //左边界
  int RightBorder;        //右边界
  int UpBorder;
  int close_LeftBorder;   //靠边边界
  int close_RightBorder;  //靠边边界
  int opp_LeftBorder;     //反向边界
  int opp_RightBorder;    //反向边界
  int Center;             //中线
  int RightTemp;          //右边临时值
  int LeftTemp;           //左边临时值
  int CenterTemp;         //中线临时值
  int Black_Point;        //单行黑点数量

  // fork   下面的变量用于岔路口的检测
  int BlackWide;     //三叉口黑色宽度
  int Black_Wide_L;  //三叉口黑色左边
  int Black_Wide_R;  //三叉口黑色右边
  int Black_Pro;     //三叉口黑点比例
  int mid_temp;      //中线临时值

  //左右手法则扫线数据
int LeftBoundary_First;  //左边界 保存第一次数据
int RightBoundary_First; //右边界 保存第一次数据
int LeftBoundary;        //左边界 保存最后一次数据
int RightBoundary;       //右边界 保存最后一次数据

} ImageDealDatatypedef;

//元素类型
typedef enum {
  Normol,       //无任何特征
  Straight,     ////直道
  Cross,        ////十字
  Ramp,         //坡道
  LeftCirque,   ////左圆环
  RightCirque,  ////右圆环
  Forkin,       //岔路进口
  Forkout,      //岔路出口
  Barn_out,     //出库
  Barn_in,      //入库
  Cross_ture,   //270°的弯道后十字

} RoadType_e;

typedef struct {
  /*以下关于全局图像正常参数*/

  //图像信息
  int TowPoint;             //初始比较点           给定前瞻
  int TowPointAdjust_v;     //初始比较点适应速度    没用
  int TowPoint_True;        //实际比较点           没用
  int TowPoint_Gain;        //增益系数             没用
  int TowPoint_Offset_Max;  //最大偏置系数         没用
  int TowPoint_Offset_Min;  //最小偏置系数         没用
  int Det_True;             //由GetDet()函数解算出来的平均图像偏差
  int Det_all;              //图像的近端到远端的总偏差
  float Det_all_k;          //斜率

  
  uint8 Threshold;          //二值化阈值
  uint32 Threshold_static;  //二值化静态下限
  uint8 Threshold_detach;   //阳光算法分割阈值
  uint8 MiddleLine;         //屏幕中心
  int Foresight;            //偏差速度因子   远端的偏差  用于控速
  uint8 OFFLine;            /////图像顶边
  uint8 LongestWhite;
  uint8 WhiteLine;          ////双边丢边数
  float ExpectCur;          /////图像期望曲率    没用
  float White_Ritio;        //有效行白点比例     没用
  int Black_Pro_ALL;        //整体黑点比例%      没用

  // PID
  float Piriod_P;  //分段P   值     没用
  float MU_P;

  RoadType_e Road_type;  //元素类型

  /****圆环***/
  uint8 IsCinqueOutIn;  //进出圆环
  uint8 CirquePass;     //圆环中
  uint8 CirqueOut;      //出圆环
  uint8 CirqueOff;      //圆环结束

  int Pass_Lenth;       //入环距离  用于防抖
                        /****圆环***/
  int Cirque1lenth;        //tly1
  int Cirque2lenth;        //tly2
  int Out_Lenth;     //
  int Fork_Out_Len;  //出岔口防抖
  int Dowm_lenth;    //三叉减速距离  //g5.13
  int Cross_Lenth;  // 270°转过之后一般是十字路口   作为三岔口的消抖
  int Cross_ture_lenth;
  int Sita;  //据此判断在过去一段路程中转过多少角度
  int pansancha_Lenth;

  //车库
  int Barn_Flag;   //判断库的次数
  int Barn_Lenth;  //入库停止距离
  int sanchaju;

  //保护
  int Stop_lenth;  //出界保护放误判距离
  //坡道减速
  int Ramp_lenth;

  int variance;  //直道检测阈值方差

  int straight_acc;  //直道加速标志位
  int  variance_acc;    //用于直道加速的阈值方差

  int ramptestlenth;//坡道检测间隔

  int rukuwait_lenth;
  int rukuwait_flag;
  int Blue_lenth;
  int newblue_flag;

  //左右手法则扫线数据
  uint8 WhiteLine_L;        //左边丢线数
  uint8 WhiteLine_R;        //右边丢线数
  uint8 OFFLineBoundary;   //八领域截止行
  uint8 Miss_Left_lines;      //左线丢失
  uint8 Miss_Right_lines;     //右线丢失
} ImageStatustypedef;

typedef struct {
  SpeedDatatypedef SpeedData;
  uint8_t SteerOK;      //舵机启动标志
  uint8_t CameraOK;     //摄像头启动显示标志
  uint8_t OldCameraOK;  //灰度传输标志
  uint8_t MotorOK;      //电机开关
  uint8_t Point;        //赛道实际中心
  uint8_t UpdataOK;     //数据更新
  uint8_t Stop;         //停止标志
  uint8_t GO_OK;        //冲冲冲
  int Model;          //车辆模式

  //图像参数
  int OutCicle_line;  //出环判断图像截止行  越大出环判断越严格越晚
  int L_T_R_W;  // 出环左有边右无边数量  越大出环结束越严格
  int Circleoff_offline;  //看到远处的距离判断出环结束  越小越严格
  int CircleP;            //环内P


//防误判距离
  int fork_start_lenth;  //岔路口检测起始
  int fork_off_lenth;//岔路口检测截止

  int circle_off_lenth;//出环防误判距离

  //小圆环
  int circles_pl;
  int circles_pr;
  int circles_off_lenth;//小圆环出环防误判距离

   //中圆环
  int circlem_pl;
  int circlem_pr;
  int circlem_off_lenth;//小圆环出环防误判距离

   //大圆环
  int circlel_pl;
  int circlel_pr;
  int circlel_off_lenth;//小圆环出环防误判距离
  int clrcle_priority[3];//圆环类型
  int clrcle_num;// 在第几个圆环


  int circle_kin;//入环补线半径
  float circle_kout;//出环补线斜率
  int circle_max_ang;//环内大叫限幅


  //直道
  float straight_p;//直道P
  float straight_d;//直道D
  int   straighet_towpoint;//直道前瞻


  int debug_lenth;//调试距离

  //摄像头配置
  int exp_time; //曝光时间
  int mtv_lroffset;//摄像头左右偏置
  int mtv_gain;//摄像头增益

  int ramp_lenth_start;//坡道距离
  int fork_lenth_start;//三叉距离
  int barn_lenth;//圆环距离

  int outbent_acc;//出弯加速


  int rounds; // 圈数
  int speed_per_round; // 每圈减速多少

} SystemDatatypdef;

typedef struct {
    uint8 zafawfwagsg;
    uint8 Bend_Road;                            /*0 :无               1 :右弯道     2 :左弯道*/
    uint8 image_element_rings;                  /*0 :无圆环          1 :左圆环       2 :右圆环*/
    uint8 ring_big_small;                       /*0:无                    2 :大圆环      1 :小圆环*/
    uint8 zafawfgwa;
    uint8 image_element_rings_flag;             /*圆环进程*/
    uint8 straight_long;                        /*长直道标志位*/
    uint8 straight_xie;
    uint8 Garage_Location;                      /*0 :无车库          1 :左车库       2 :右车库*/
    uint8 Zebra_Flag;                           /*0 :无斑马线       1 :斑马线*/
    uint8 Ramp;                                  /*0 :无坡道        1：坡道*/
    uint8 RoadBlock_Flag;                        /*0 :无路障        1 :左过路障     2 :右过路障*/
    uint8 Out_Road;                               /*0 :无断路      1 :断路*/
	uint8 cross_flag;
	uint8 right_obst_flag;                        /*0 右侧无障碍物   1：右侧障碍物*/
	uint8 left_obst_flag;                        /*0 左侧无障碍物   1：左侧障碍物*/
	uint8 ramptestflag;                            //坡道标志位
	uint8 cross_dir;                              //十字方向

} ImageFlagtypedef;


typedef enum//  此枚举定义不允许用户修改
{
    RoadBlock,
    Left_Rings,
    Right_Rings,
    Zebra,
    Normal,
}RoadType_quest;


extern uint8 Image_Use[LCDH][LCDW];
extern uint8 Pixle[LCDH][LCDW];
extern uint8 UImage_Use[uLCDH][uLCDW];
extern ImageStatustypedef ImageStatus;
extern ImageDealDatatypedef ImageDeal[60];
extern float variance;  //方差
extern int ForkLinePointy;
extern float variance_acc;
extern int Fork_dowm;
extern int circle_r;
extern float circlk_k;
extern int circleoff_lenth;//
extern int forklenth;
extern int barnlenth;
extern int ramplenth;
extern int ramptestflag;
extern int rampnum;
extern int Right_RingsFlag_Point2_Ysite;
extern int Right_RingsFlag_Point1_Ysite;
extern int Zebra_flag;
extern int turn_flag;
extern int border_line_left;
extern int border_line_right;
extern ImageFlagtypedef ImageFlag;
extern int card_out_flag_left,card_out_flag_right;
void camera_display(void);  //图像显示
void compressimage();       ////压缩图像
void Get01change();         ////二值化图像
void Pixle_Filter();        ////像素滤波，效果不大
void Draw_Road(void);       ///绘制赛道边界
void ImageProcess(void);    ////图像处理主函数
void uncompressimage();     //图像解压
void ForkTest();            //岔路口检测
void Cross_Test();          //圆环检测
void Cross_Handle();        //圆环处理
void Element_Test();        //元素检测
void Ramp_Test();           //坡道检测
void Barn_test_in();        //入库检测
void Element_Handle();      //元素处理
void Fork_Handle();         //三叉处理
void Barn_in_Handle();      //没用
void GetLenSita();          //没用了
void Pcir_Handle(void);
uint8 len_maxget(void);
void get_border_line(void);
int Limit(int num, int numH, int numL);
uint8 len_maxget(void);
uint8 Threshold_deal(uint8* image,
                     uint16 col,
                     uint16 row,
                     uint32 pixel_threshold);
#endif /*CAMERA_H_ */
