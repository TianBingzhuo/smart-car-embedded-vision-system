#include "zf_common_headfile.h"
#include "stdlib.h"
#include "camera.h"
#include "Fuzzy.h"
#include <math.h>
#include "u660.h"
#include "car_move.h"
#include "ips200.h"
int ImageScanInterval;                         //扫边范围    上一行的边界+-ImageScanInterval
int ImageScanInterval_Cross;                   //270°的弯道后十字的扫线范围
uint8 Image_Use[LCDH][LCDW];      //压缩后的灰度图像(60*80)，   mt9v03x_image[row][line](120*188)为摄像头获取的原始图像
uint8 Pixle[LCDH][LCDW];          //用于处理的二值化图像
uint8 uPixle[uLCDH][uLCDW];       //用于屏幕显示解压后的二值化图像，便于调试观察
uint8 UImage_Use[uLCDH][uLCDW];   //用于屏幕显示解压后的灰度图像，便于调试观察
static int Ysite = 0, Xsite = 0;               //Y坐标=列；X坐标=行
static uint8* PicTemp;                         //保存单行图像
static int IntervalLow = 0, IntervalHigh = 0;  //定义高低扫描区间
static int ytemp = 0;                          //存放行
static int TFSite = 0, FTSite = 0;             //存放行
static float DetR = 0, DetL = 0;               //存放斜率
static int BottomBorderRight = 79,             //59行右边界
BottomBorderLeft = 0,                          //59行左边界
BottomCenter = 0;                              //59行中点





ImageDealDatatypedef ImageDeal[60];            //记录单行的信息

ImageDealDatatypedef ImageDeal_line[60];        //记录列的信息

ImageStatustypedef ImageData;                  //需要修改的图像阈值参数
SystemDatatypdef SystemData;
ImageFlagtypedef ImageFlag;                     //图像元素种类的标志位
RoadType_quest Statu;                           //各元素的种类
int Right_RingsFlag_Point1_Ysite, Right_RingsFlag_Point2_Ysite; //右圆环判断的两点纵坐标
int Left_RingsFlag_Point1_Ysite, Left_RingsFlag_Point2_Ysite;   //左圆环判断的两点纵坐标
uint8 Ring_Help_Flag = 0;                       //进环辅助标志
uint16 circle_num,position;
uint8 Front_Ring_Continue_Count = 0;           //蓝牙传输用的，这个没用
int Point_Xsite,Point_Ysite;                   //拐点横纵坐标
int Repair_Point_Xsite,Repair_Point_Ysite;     //补线点横纵坐标

int forklenth;                                 //确定三叉位置
int barnlenth;                                 //确定车库位置
int ramplenth;                                 //确定坡道位置
float Weighting[10] = {0.96, 0.92, 0.88, 0.83, 0.77,0.71, 0.65, 0.59, 0.53, 0.47};//10行权重参数，随意更改，基本不影响，大致按照正态分布即可
uint8 ExtenLFlag = 0;  //是否左延长标志
uint8 ExtenRFlag = 0;  //是否右延长标志
float ang_l;
int ycircle=0;
int xcircle=0;
int axcircle=0;
int aycircle=0;
int bxcircle=0;
int bycircle=0;
int left_missline=0;
int right_missline=0;
int border_line_left=0;
int border_line_right=0;
int cross_right_missline=0,cross_left_missline=0;     //判断十字丢边的数量
int card_out_flag_left,card_out_flag_right;
float K;
extern float Speed_gain;
extern float var_gain,con_speed,base;
uint8 buxianwide[60]={6,7,8,9,10,11,12,13,14,15,
                      16,17,18,19,20,21,22,23,24,25,
                      26,27,28,29,30,31,32,33,34,35,
                      36,37,38,39,40,41,42,43,44,45,
                      46,47,48,49,50,51,52,53,54,55,
                      56,57,58,59,60,61,62,63,64,65};

uint8 Half_Road_Wide[60] =                      //直到赛道半宽
{ 2,3,3,4,4,4,5,5,6,6,
  7,7,8,8,8,9,10,10,11,11,
  11,12,13,13,13,14,14,15,15,15,
  16,16,17,17,18,18,18,19,19,20,
  21,21,21,22,22,25,25,25,26,26,
  27,28,28,28,29,30,31,31,31,32,
};
uint8 Half_Bend_Wide[60] =                      //弯道赛道半宽
{   33,33,33,33,33,33,33,33,33,33,
    33,33,32,32,31,31,30,30,29,28,
    28,27,27,26,26,25,25,24,24,23,
    23,22,22,23,23,23,24,24,24,24,
    25,25,25,26,26,26,27,27,28,28,
    28,29,29,30,30,31,31,32,32,33,
};

int adcsum;   //电感值之和
int tuoluoyijifen;
int turn_flag=0;





ImageStatustypedef ImageStatus;   //原先在steer.c文件中



//压缩图像的函数
float Mh = MT9V03X_H;
float Lh = LCDH;
float Mw = MT9V03X_W;
float Lw = LCDW;
void compressimage() {
  int i, j, row, line;
  const float div_h = Mh / Lh, div_w = Mw / Lw;

  for (i = 0; i < LCDH; i++) {
    row = (int)(i * div_h+0.5 );
    for (j = 0; j < LCDW; j++) {
      line = (int)(j * div_w +0.5);
      Image_Use[i][j] = mt9v03x_image[row][line];
    }
  }
  mt9v03x_finish_flag = 0;  //使用完一帧DMA传输的图像图像  可以开始传输下一帧
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      优化的大津法
//  @param      image  图像数组
//  @param      clo    宽
//  @param      row    高
//  @param      pixel_threshold 阈值分离
//  @return     uint8
//  @since      2021.6.23
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 Threshold_deal(uint8* image,
                     uint16 col,
                     uint16 row,
                     uint32 pixel_threshold) {
#define GrayScale 256
  uint16 width = col;
  uint16 height = row;
  int pixelCount[GrayScale];
  float pixelPro[GrayScale];
  int i, j, pixelSum = width * height;
  uint8 threshold = 0;
  uint8* data = image;  //指向像素数据的指针
  for (i = 0; i < GrayScale; i++) {
    pixelCount[i] = 0;
    pixelPro[i] = 0;
  }

  uint32 gray_sum = 0;
  //统计灰度级中每个像素在整幅图像中的个数
  for (i = 0; i < height; i += 1) {
    for (j = 0; j < width; j += 1) {
      // if((sun_mode&&data[i*width+j]<pixel_threshold)||(!sun_mode))
      //{
      pixelCount[(
          int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
      gray_sum += (int)data[i * width + j];  //灰度值总和
      //}
    }
  }

  //计算每个像素值的点在整幅图像中的比例
  for (i = 0; i < GrayScale; i++) {
    pixelPro[i] = (float)pixelCount[i] / pixelSum;
  }


  //遍历灰度级[0,255]
  float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
  w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
  for (j = 0; j < pixel_threshold; j++) {
    w0 +=
        pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和 即背景部分的比例
    u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

    w1 = 1 - w0;
    u1tmp = gray_sum / pixelSum - u0tmp;

    u0 = u0tmp / w0;    //背景平均灰度
    u1 = u1tmp / w1;    //前景平均灰度
    u = u0tmp + u1tmp;  //全局平均灰度
    deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
    if (deltaTmp > deltaMax) {
      deltaMax = deltaTmp;
      threshold = j;
    }
    if (deltaTmp < deltaMax) {
      break;
    }
  }
  return threshold;  //大津法计算出来的阈值
}




//图像二值化    2.7ms       使用大津法将灰度图像变为黑白的二值化图像，只要二值化阈值取好，效果和二值化图像一样，根据光线的强弱动态的改变阈值，灰度图像点小于阈值为白点，大于阈值为黑点,一定要设置阈值下限和阳光算法强光的分离上限（把光线很强的点（比如反光）舍弃了，不再算在大津的范畴里否则会影响整幅图像分割效果）
void Get01change_dajin() {
	
	 ImageStatus.Threshold_static = 140;          //原先位置不在这里

	ImageStatus.Threshold = Threshold_deal(Image_Use[0], LCDW, LCDH, ImageStatus.Threshold_detach);//通过大津法获得这一帧图像的阈值
  if (ImageStatus.Threshold < ImageStatus.Threshold_static)
    ImageStatus.Threshold = ImageStatus.Threshold_static;
  uint8 i, j = 0;
  uint8 thre;
  for (i = 0; i < LCDH; i++) {
    for (j = 0; j < LCDW; j++) {
      if (j <= 15)
        thre = ImageStatus.Threshold - 10;
      else if ((j > 70 && j <= 75))
        thre = ImageStatus.Threshold - 10;
      else if (j >= 65)
        thre = ImageStatus.Threshold - 10;
      else
        thre = ImageStatus.Threshold;

      if (Image_Use[i][j] >
          (thre))         //数值越大，显示的内容越多，较浅的图像也能显示出来
        Pixle[i][j] = 1;  //白
      else
        Pixle[i][j] = 0;  //黑
    }
  }
}



//像素滤波
void Pixle_Filter() {                           //如果一个点的四周中有三个点为同一颜色，则中间点为该颜色
  int nr;  //行
  int nc;  //列

  for (nr = 10; nr < 40; nr++) {
    for (nc = 10; nc < 70; nc = nc + 1) {
      if ((Pixle[nr][nc] == 0) && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] +
                                       Pixle[nr][nc + 1] + Pixle[nr][nc - 1] >=
                                   3)) {
        Pixle[nr][nc] = 1;
      }
	}
  }
}



//扫描某一行，确定该行的左右边线跳变点状态，并记录跳变点坐标（正常跳变坐标准确，大跳变点只是取左右间隔坐标值，后面函数还会对于大跳变点的坐标进行更改。如果扫描小间隔内全为白点则使用上一行的边界坐标值，后面函数还会对于这类的无边行的边界坐标进行更改）
void GetJumpPointFromDet(uint8* p,uint8 type,int L,int H,JumpPointtypedef* Q)  //第一个参数是要查找的数组（80个点）
                                                                               //第二个扫左边线还是扫右边线
{                                                                              //三四是开始和结束点
  int i = 0;
  if (type == 'L')                              //扫描左边线
  {
    for (i = H; i >= L; i--) {
      if (*(p + i) == 1 && *(p + i - 1) != 1)   //由黑变白
      {
        Q->point = i;                           //记录左边线
        Q->type = 'T';                          //正确跳变
        break;
      } else if (i == (L + 1))                  //若果扫到最后也没找到
      {
        if (*(p + (L + H) / 2) != 0)            //如果中间是白的
        {
          Q->point = (L + H) / 2;               //认为左边线是中点
          Q->type = 'W';                        //非正确跳变且中间为白，认为没有边
          break;
        } else                                  //非正确跳变且中间为黑
        {
          Q->point = H;                         //左边线直接最大值
          Q->type = 'H';                        //认为是大跳变
          break;
        }
      }
    }
  } else if (type == 'R')                       //扫描右边线
  {
    for (i = L; i <= H; i++)                    //从右往左扫
    {
      if (*(p + i) == 1 && *(p + i + 1) != 1)   //找由黑到白的跳变
      {
        Q->point = i;                           //记录
        Q->type = 'T';
        break;
      } else if (i == (H - 1))                  //若果扫到最后也没找到
      {
        if (*(p + (L + H) / 2) != 0)            //如果中间是白的
        {
          Q->point = (L + H) / 2;               //认为右边线是中点
          Q->type = 'W';                        //非正确跳变且中间为白，认为没有边
          break;
        } else                                  //如果中点是黑的
        {
          Q->point = L;                         //右边线直接最大值
          Q->type = 'H';                        //认为是大跳变
          break;
        }
      }
    }
  }
}



//绘制底边（底边五行（55~60）的信息提取，左右赛道边线和赛道中线坐标提取，赛道宽度，是否左右有边标志位判断）      30us         T为正常跳变边    W为无边   P为障碍类多跳边的内边       H为大跳变边
static uint8 DrawLinesFirst(void) {
  PicTemp = Pixle[59];
  if (*(PicTemp + ImageSensorMid) == 0)                 //如果底边图像中点为黑，异常情况
  {
    for (Xsite = 0; Xsite < ImageSensorMid; Xsite++)    //找左右边线
    {
      if (*(PicTemp + ImageSensorMid - Xsite) != 0)     //一旦找到左或右赛道到中心距离，就break
        break;                                          //并且记录Xsite
      if (*(PicTemp + ImageSensorMid + Xsite) != 0)
        break;
    }

    if (*(PicTemp + ImageSensorMid - Xsite) != 0)       //赛道如果在左边的话
    {
      BottomBorderRight = ImageSensorMid - Xsite + 1;   // 59行右边线有啦
      for (Xsite = BottomBorderRight; Xsite > 0; Xsite--)  //开始找59行左边线
      {
        if (*(PicTemp + Xsite) == 0 &&
            *(PicTemp + Xsite - 1) == 0)                //连续两个黑点，滤波
        {
          BottomBorderLeft = Xsite;                     //左边线找到
          break;
        } else if (Xsite == 1) {
          BottomBorderLeft = 0;                         //搜索到最后了，看不到左边线，左边线认为是0
          break;
        }
      }
    } else if (*(PicTemp + ImageSensorMid + Xsite) != 0)  //赛道如果在右边的话
    {
      BottomBorderLeft = ImageSensorMid + Xsite - 1;    // 59行左边线有啦
      for (Xsite = BottomBorderLeft; Xsite < 79; Xsite++)  //开始找59行左边线
      {
        if (  *(PicTemp + Xsite) == 0
            &&*(PicTemp + Xsite + 1) == 0)              //连续两个黑点，滤波
        {
          BottomBorderRight = Xsite;                    //右边线找到
          break;
        } else if (Xsite == 78) {
          BottomBorderRight = 79;                       //搜索到最后了，看不到右边线，左边线认为是79
          break;
        }
      }
    }
  }
  else                                                //左边线中点是白的，比较正常的情况
  {
    for (Xsite = 79; Xsite >ImageSensorMid; Xsite--)   //一个点一个点地搜索右边线
    {
      if (  *(PicTemp + Xsite) == 1
          &&*(PicTemp + Xsite - 1) == 1)                //连续两个白点，滤波
      {
        BottomBorderRight = Xsite;                      //找到就记录
        break;
      } else if (Xsite == 40) {
        BottomBorderRight = 39;                         //找不到认为79
        break;
      }
    }
    for (Xsite = 0; Xsite < ImageSensorMid; Xsite++)    //一个点一个点地搜索左边线
    {
      if (  *(PicTemp + Xsite) == 1
          &&*(PicTemp + Xsite + 1) == 1)                //连续两个黑点，滤波
      {
        BottomBorderLeft = Xsite;                       //找到就记录
        break;
      } else if (Xsite == 38) {
        BottomBorderLeft = 39;                           //找不到认为0
        break;
      }
    }
  }
  BottomCenter =(BottomBorderLeft + BottomBorderRight) / 2;   // 59行中点直接取平均
  ImageDeal[59].LeftBorder = BottomBorderLeft;                //在数组里面记录一下信息，第一行特殊一点而已
  ImageDeal[59].RightBorder = BottomBorderRight;
  ImageDeal[59].Center = BottomCenter;                        //确定最底边
  ImageDeal[59].Wide = BottomBorderRight - BottomBorderLeft;  //存储宽度信息
  ImageDeal[59].IsLeftFind = 'T';
  ImageDeal[59].IsRightFind = 'T';



  for (Ysite = 59; Ysite > 54; Ysite--)                       //由中间向两边确定底边五行
  {
    PicTemp = Pixle[Ysite];
    for (Xsite = 79; Xsite > ImageDeal[Ysite + 1].Center;        //和前面一样的搜索，从最右边向上一行图像的的中点寻找两个连续的由黑变白色的跳变点，记录X坐标
         Xsite--)
    {
      if (*(PicTemp + Xsite) == 1 && *(PicTemp + Xsite - 1) == 1) {
        ImageDeal[Ysite].RightBorder = Xsite;
        break;
      } else if (Xsite == (ImageDeal[Ysite + 1].Center+1)) {
        ImageDeal[Ysite].RightBorder = ImageDeal[Ysite + 1].Center;
        break;
      }
    }
    for (Xsite = 0; Xsite < ImageDeal[Ysite + 1].Center;         //和前面一样的搜索，从最左边向上一行图像的的中点寻找两个连续的由黑变白色的跳变点，记录X坐标
         Xsite++)
    {
      if (*(PicTemp + Xsite) == 1 && *(PicTemp + Xsite + 1) == 1) {
        ImageDeal[Ysite].LeftBorder = Xsite;
        break;
      } else if (Xsite == (ImageDeal[Ysite + 1].Center-1)) {
        ImageDeal[Ysite].LeftBorder = ImageDeal[Ysite + 1].Center;
        break;
      }
    }
    ImageDeal[Ysite].IsLeftFind = 'T';                        //这些信息存储到数组里
    ImageDeal[Ysite].IsRightFind = 'T';
    ImageDeal[Ysite].Center =
        (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) /2; //存储中点
    ImageDeal[Ysite].Wide =
        ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;      //存储宽度
  }
  return 'T';
}                                                             //最基本的要求，最近的五行首先不会受到干扰，这需要在安装的时候调整摄像头的视角


/*边线追逐大致得到全部边线*/
static void DrawLinesProcess(void)  //////不用更改
{
    ImageStatus.Miss_Left_lines=0;//丢边清0
    ImageStatus.Miss_Right_lines=0;//丢边清0
  uint8 L_Found_T = 'F';  //确定无边斜率的基准有边行是否被找到的标志
  uint8 Get_L_line = 'F';  //找到这一帧图像的基准左斜率
  uint8 R_Found_T = 'F';  //确定无边斜率的基准有边行是否被找到的标志
  uint8 Get_R_line = 'F';  //找到这一帧图像的基准右斜率
  float D_L = 0;           //延长线左边线斜率
  float D_R = 0;           //延长线右边线斜率
  int ytemp_W_L;           //记住首次左丢边行
  int ytemp_W_R;           //记住首次右丢边行
  ExtenRFlag = 0;          //标志位清0
  ExtenLFlag = 0;
  for (Ysite = 54 ; Ysite > ImageStatus.OFFLine; Ysite--)            //前5行处理过了，下面从55行到（设定的不处理的行OFFLine（图像顶边））
  {                        //太远的图像不稳定，OFFLine以后的不处理
    PicTemp = Pixle[Ysite];
    JumpPointtypedef JumpPoint[2];                                          // 数组[0]代表左边线跳变信息，数组[1]代表右边线跳变信息
    if (ImageStatus.Road_type != Cross_ture  )       //270°的弯道后十字)
            {
      IntervalLow =ImageDeal[Ysite + 1].RightBorder -ImageScanInterval;             //从上一行右边线-Interval的点开始（确定扫描开始点）         ImageScanInterval=2
      IntervalHigh =ImageDeal[Ysite + 1].RightBorder + ImageScanInterval;           //到上一行右边线+Interval的点结束（确定扫描结束点）         ImageScanInterval=2
    } else {
      IntervalLow =ImageDeal[Ysite + 1].RightBorder -ImageScanInterval_Cross;       //从上一行右边线-Interval_Cross的点开始（确定扫描开始点）   ImageScanInterval_Cross=2
      IntervalHigh = ImageDeal[Ysite + 1].RightBorder + ImageScanInterval_Cross;    //到上一行右边线+Interval_Cross的点开始（确定扫描开始点）   ImageScanInterval_Cross=2
    }

    LimitL(IntervalLow);   //确定左扫描区间并进行限制
    LimitH(IntervalHigh);  //确定右扫描区间并进行限制
    GetJumpPointFromDet(PicTemp, 'R', IntervalLow, IntervalHigh,&JumpPoint[1]);     //扫右边线     //扫描某一行，确定该行的左右边线跳变点状态，并记录跳变点坐标（正常跳变坐标准确，大跳变点只是取左右间隔坐标值，后面函数还会对于大跳变点的坐标进行更改。如果扫描小间隔内全为白点则使用上一行的边界坐标值，后面函数还会对于这类无边行的左右边界坐标进行更改）

    IntervalLow =ImageDeal[Ysite + 1].LeftBorder -ImageScanInterval;                //从上一行左边线-2的点开始（确定扫描开始点）
    IntervalHigh =ImageDeal[Ysite + 1].LeftBorder +ImageScanInterval;               //到上一行左边线+2的点结束（确定扫描结束点）

    LimitL(IntervalLow);   //确定左扫描区间并进行限制
    LimitH(IntervalHigh);  //确定右扫描区间并进行限制
    GetJumpPointFromDet(PicTemp, 'L', IntervalLow, IntervalHigh,&JumpPoint[0]);     //扫左边线      //扫描某一行，确定该行的左右边线跳变点状态，并记录跳变点坐标（正常跳变坐标准确，大跳变点只是取左右间隔坐标值，后面函数还会对于大跳变点的坐标进行更改。如果扫描小间隔内全为白点则使用上一行的边界坐标值，后面函数还会对于这类无边行的左右边界坐标进行更改）

    if (JumpPoint[0].type =='W')                                                    //如果本行左边线不正常跳变，即这5个点都是白的
    {
      ImageDeal[Ysite].LeftBorder =ImageDeal[Ysite + 1].LeftBorder;                 //本行左边线用上一行的数值
    } else                                                                          //左边线正常
    {
      ImageDeal[Ysite].LeftBorder = JumpPoint[0].point;                             //记录下来啦
    }

    if (JumpPoint[1].type == 'W')                                                   //如果本行右边线不正常跳变，即这10个点都是白的
    {
      ImageDeal[Ysite].RightBorder =ImageDeal[Ysite + 1].RightBorder;               //本行右边线用上一行的数值
    } else                                                                          //右边线正常
    {
      ImageDeal[Ysite].RightBorder = JumpPoint[1].point;                            //记录下来啦
    }

    ImageDeal[Ysite].IsLeftFind =JumpPoint[0].type;                                 //记录本行是否找到左边线，即左边线类型
    ImageDeal[Ysite].IsRightFind = JumpPoint[1].type;                               //记录本行是否找到右边线，即右边线类型

    //重新确定那些大跳变的边缘
    if (( ImageDeal[Ysite].IsLeftFind == 'H'
         ||ImageDeal[Ysite].IsRightFind == 'H')) {
      if (ImageDeal[Ysite].IsLeftFind == 'H')                                   //如果左边线大跳变
        for (Xsite = (ImageDeal[Ysite].LeftBorder + 1); Xsite <= (ImageDeal[Ysite].RightBorder - 1); Xsite++)//左右边线之间重新扫描
        {
          if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite + 1) != 0)) {
            ImageDeal[Ysite].LeftBorder =Xsite;                                 //如果上一行左边线的右边有黑白跳变则为绝对边线直接取出
            ImageDeal[Ysite].IsLeftFind = 'T';
            break;
          } else if (*(PicTemp + Xsite) != 0)                                   //一旦出现白点则直接跳出
            break;
          else if (Xsite ==(ImageDeal[Ysite].RightBorder - 1))
          {
             ImageDeal[Ysite].LeftBorder = Xsite;
             ImageDeal[Ysite].IsLeftFind = 'T';
            break;
          }
        }
      if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder) <=7)     //图像宽度限定，当某一行的左右两边赛道宽度小于等于7时，以这一行作为OFFLine，这一行以上的行全部舍弃不做处理。因为太靠上的图像光线杂，畸变多，因此直接舍弃。
      {
       ImageStatus.OFFLine = Ysite + 1;  //如果这行比7小了后面直接不要了
        break;
      }

      if (ImageDeal[Ysite].IsRightFind == 'H')                                   //如果右边线大跳变
        for (Xsite = (ImageDeal[Ysite].RightBorder - 1);Xsite >= (ImageDeal[Ysite].LeftBorder + 1); Xsite--)//左右边线之间重新扫描
        {
          if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite - 1) != 0)) {
            ImageDeal[Ysite].RightBorder =Xsite;                        //如果右边线的左边有黑白跳变则为绝对边线直接取出
            ImageDeal[Ysite].IsRightFind = 'T';
            break;
          } else if (*(PicTemp + Xsite) != 0)                            //一旦出现白点则直接跳出
            break;
          else if (Xsite == (ImageDeal[Ysite].LeftBorder + 1))
          {
            ImageDeal[Ysite].RightBorder = Xsite;
            ImageDeal[Ysite].IsRightFind = 'T';
            break;
          }
        }
    }

 /***********重新确定无边行************/         //如果下面几行都是无边行，那么就不进行斜率补线了
    int ysite = 0;
    uint8 L_found_point = 0;
    uint8 R_found_point = 0;

    if (ImageDeal[Ysite].IsRightFind == 'W'&&Ysite > 10&&Ysite < 50)  //最早出现的右无边行
    {
      if (Get_R_line == 'F')    //这一帧图像没有跑过这个找基准线的代码段才运行
      {
        Get_R_line = 'T';       //找了  一帧图像只跑一次 置为T
        ytemp_W_R = Ysite + 2;
        for (ysite = Ysite + 1; ysite < Ysite + 15; ysite++) {
          if (ImageDeal[ysite].IsRightFind =='T')  //往无边行下面搜索  一般都是有边的
            R_found_point++;
        }
        if (R_found_point >8)                      //找到基准斜率边  做延长线重新确定无边   当有边的点数大于8
        {
          D_R = ((float)(ImageDeal[Ysite + R_found_point].RightBorder - ImageDeal[Ysite + 3].RightBorder)) /((float)(R_found_point - 3));  //求这些点连起来的斜率，好给无边行做延长线做个基准   K=(y2-y1)/(X2-X1)

          if (D_R > 0) {                          //如果斜率大于0  那么找到了这个基准行  因为梯形畸变
            R_Found_T ='T';                      //所以一般情况都是斜率大于0  小于0的情况也不用延长 没必要

          } else {
            R_Found_T = 'F';                      //没有找到这个基准行
            if (D_R < 0)
              ExtenRFlag = 'F';                   //这个标志位用于十字角点补线  防止图像误补用的
          }
        }
      }
      if (R_Found_T == 'T')
        ImageDeal[Ysite].RightBorder =ImageDeal[ytemp_W_R].RightBorder -D_R * (ytemp_W_R - Ysite);  //如果找到了 那么以基准行做延长线      y=y0-K(x0-x)

      LimitL(ImageDeal[Ysite].RightBorder);  //限幅
      LimitH(ImageDeal[Ysite].RightBorder);  //限幅
    }

    if (ImageDeal[Ysite].IsLeftFind == 'W' && Ysite > 10 && Ysite < 50 )    //下面同理     //最早出现的左无边行
    {
      if (Get_L_line == 'F') {      //这一帧图像没有跑过这个找基准线的代码段才运行
        Get_L_line = 'T';           //找了  一帧图像只跑一次 置为T
        ytemp_W_L = Ysite + 2;
        for (ysite = Ysite + 1; ysite < Ysite + 15; ysite++) {
          if (ImageDeal[ysite].IsLeftFind == 'T')   //往无边行下面搜索  一般都是有边的
            L_found_point++;
        }
        if (L_found_point > 8)              //找到基准斜率边  做延长线重新确定无边  当有边的点数大于8
        {
          D_L = ((float)(ImageDeal[Ysite + 3].LeftBorder -ImageDeal[Ysite + L_found_point].LeftBorder)) /((float)(L_found_point - 3));//求这些点连起来的斜率，好给无边行做延长线做个基准   K'=(y1-y2)/(X2-X1)
          if (D_L > 0) {         //如果斜率大于0  那么找到了这个基准行  因为梯形畸变
            L_Found_T = 'T';    //所以一般情况都是斜率大于0  小于0的情况也不用延长 没必要

          } else {
            L_Found_T = 'F';     //没有找到这个基准行
            if (D_L < 0)
              ExtenLFlag = 'F';  //这个标志位用于十字角点补线  防止图像误补用的
          }
        }
      }

      if (L_Found_T == 'T')
        ImageDeal[Ysite].LeftBorder =ImageDeal[ytemp_W_L].LeftBorder + D_L * (ytemp_W_L - Ysite); //如果找到了 那么以基准行做延长线      y=y0+K'(x0-x)

      LimitL(ImageDeal[Ysite].LeftBorder);  //限幅
      LimitH(ImageDeal[Ysite].LeftBorder);  //限幅
    }

    /**************************处理右边线的无边行***************************/
    /************************************重新确定无边行（即W类）的边界****************************************************************/


    /************************************都处理完之后，其他的一些数据整定操作*************************************************/
      if (ImageDeal[Ysite].IsLeftFind == 'W'&&ImageDeal[Ysite].IsRightFind == 'W')
      {
          ImageStatus.WhiteLine++;  //要是左右都无边，丢边数+1
      }
     if (ImageDeal[Ysite].IsLeftFind == 'W'&&Ysite<55)
     {
          ImageStatus.Miss_Left_lines++;
     }
     if (ImageDeal[Ysite].IsRightFind == 'W'&&Ysite<55)
     {
          ImageStatus.Miss_Right_lines++;
     }

      LimitL(ImageDeal[Ysite].LeftBorder);   //限幅
      LimitH(ImageDeal[Ysite].LeftBorder);   //限幅
      LimitL(ImageDeal[Ysite].RightBorder);  //限幅
      LimitH(ImageDeal[Ysite].RightBorder);  //限幅

      ImageDeal[Ysite].Wide =ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;//边界宽度
      ImageDeal[Ysite].Center =(ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2; //中线

    if (ImageDeal[Ysite].Wide <= 7)         //重新确定可视距离  //图像宽度限定，当某一行的左右两边赛道宽度小于等于7时，以这一行作为OFFLine，这一行以上的行全部舍弃不做处理。因为太靠上的图像光线杂，畸变多，因此直接舍弃。
    {
      ImageStatus.OFFLine = Ysite + 1;      //如果这行比7小了后面直接不要了
      break;}
    else if (  ImageDeal[Ysite].RightBorder <= 10||ImageDeal[Ysite].LeftBorder >= 70){
              ImageStatus.OFFLine = Ysite + 1;
              break;                         //当图像宽度小于0或者左右边达到一定的限制时，则终止巡边
    }
  }
  return;
}

//按列扫描，得到车与边线之间的距离，并且进行判断，使其左右移动
static void distance_line(void)
{
	for(Xsite=78;Xsite>2;Xsite--)
	{
		for(Ysite=20;Ysite<60;Ysite++)
		{
			if(Pixle[Ysite][Xsite]==1&&Pixle[Ysite-1][Xsite]==0&&Pixle[Ysite-2][Xsite]==0
				&&Pixle[Ysite+2][Xsite]==1&&Pixle[Ysite+1][Xsite]==1&&Pixle[Ysite-3][Xsite]==0)
			{
				ImageDeal[Xsite].UpBorder=Ysite;
			}
		}
	}
	return;
}


/*上交大左右手法则扫线，作为处理圆环等判断元素的第二依据*/
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//  @name           Search_Bottom_Line_OTSU
//  @brief          获取底层左右边线
//  @param          imageInput[IMAGE_ROW][IMAGE_COL]        传入的图像数组
//  @param          Row                                     图像的Ysite
//  @param          Col                                     图像的Xsite
//  @return         Bottonline                              底边行选择
//  @time           2022年10月9日
//  @Author         陈新云
//  Sample usage:   Search_Bottom_Line_OTSU(imageInput, Row, Col, Bottonline);
//--------------------------------------------------------------------------------------------------------------------------------------------

void Search_Bottom_Line_OTSU(uint8 imageInput[LCDH][LCDW], uint8 Row, uint8 Col, uint8 Bottonline)
{

    //寻找左边边界
    for (int Xsite = Col / 2-2; Xsite > 0; Xsite--)
    {
        if (imageInput[Bottonline][Xsite] == 1 && imageInput[Bottonline][Xsite - 1] == 0)
        {
            ImageDeal[Bottonline].LeftBoundary = Xsite;//获取底边左边线
            break;
        }
    }
    for (int Xsite = Col / 2+2; Xsite < LCDW; Xsite++)
    {
        if (imageInput[Bottonline][Xsite] == 1 && imageInput[Bottonline][Xsite + 1] == 0)
        {
            ImageDeal[Bottonline].RightBoundary = Xsite;//获取底边右边线
            break;
        }
    }


}




//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//  @name           Search_Left_and_Right_Lines
//  @brief          通过sobel提取左右边线
//  @param          imageInput[IMAGE_ROW][IMAGE_COL]        传入的图像数组
//  @param          Row                                     图像的Ysite 行
//  @param          Col                                     图像的Xsite 列
//  @param          Bottonline                              底边行选择
//  @return         无
//  @time           2022年10月7日
//  @Author         陈新云
//  Sample usage:   Search_Left_and_Right_Lines(imageInput, Row, Col, Bottonline);
//--------------------------------------------------------------------------------------------------------------------------------------------

void Search_Left_and_Right_Lines(uint8 imageInput[LCDH][LCDW], uint8 Row, uint8 Col, uint8 Bottonline)
{
    //定义小人的当前行走状态位置为 上 左 下 右 一次要求 上：左边为黑色 左：上边为褐色 下：右边为黑色  右：下面有黑色
/*  前进方向定义：
                *   0
                * 3   1
                *   2
*/
/*寻左线坐标规则*/
    uint8 Left_Rule[2][8] = {
                                  {0,-1,1,0,0,1,-1,0 },//{0,-1},{1,0},{0,1},{-1,0},  (x,y )
                                  {-1,-1,1,-1,1,1,-1,1} //{-1,-1},{1,-1},{1,1},{-1,1}
    };
    /*寻右线坐标规则*/
    int Right_Rule[2][8] = {
                              {0,-1,1,0,0,1,-1,0 },//{0,-1},{1,0},{0,1},{-1,0},
                              {1,-1,1,1,-1,1,-1,-1} //{1,-1},{1,1},{-1,1},{-1,-1}
    };
      int num=0;
    uint8 Left_Ysite = Bottonline;
    uint8 Left_Xsite = ImageDeal[Bottonline].LeftBoundary;
    uint8 Left_Rirection = 0;//左边方向
    uint8 Pixel_Left_Ysite = Bottonline;
    uint8 Pixel_Left_Xsite = 0;

    uint8 Right_Ysite = Bottonline;
    uint8 Right_Xsite = ImageDeal[Bottonline].RightBoundary;
    uint8 Right_Rirection = 0;//右边方向
    uint8 Pixel_Right_Ysite = Bottonline;
    uint8 Pixel_Right_Xsite = 0;
    uint8 Ysite = Bottonline;
    ImageStatus.OFFLineBoundary = 5;
    while (1)
    {
            num++;
            if(num>400)
            {
                 ImageStatus.OFFLineBoundary = Ysite;
                break;
            }
        if (Ysite >= Pixel_Left_Ysite && Ysite >= Pixel_Right_Ysite)
        {
            if (Ysite < ImageStatus.OFFLineBoundary)
            {
                ImageStatus.OFFLineBoundary = Ysite;
                break;
            }
            else
            {
                Ysite--;
            }
        }
        /*********左边巡线*******/
        if ((Pixel_Left_Ysite > Ysite) || Ysite == ImageStatus.OFFLineBoundary)//左边扫线
        {
            /*计算前方坐标*/
            Pixel_Left_Ysite = Left_Ysite + Left_Rule[0][2 * Left_Rirection + 1];   //行坐标   -1，0，1，0  向左，向上，向右，向下
            Pixel_Left_Xsite = Left_Xsite + Left_Rule[0][2 * Left_Rirection];       //列坐标   0，1，0，-1

            if (imageInput[Pixel_Left_Ysite][Pixel_Left_Xsite] == 0)//前方是黑色
            {
                //顺时针旋转90   
                if (Left_Rirection == 3)     //向下变为向左
                    Left_Rirection = 0;
                else
                    Left_Rirection++;        //如果不是，继续向上下左右寻找合适的
            }
            else                             //前方是白色
            {
                /*计算左前方坐标*/
                Pixel_Left_Ysite = Left_Ysite + Left_Rule[1][2 * Left_Rirection + 1];   //-1，1，-1，1   右，下，上，左
                Pixel_Left_Xsite = Left_Xsite + Left_Rule[1][2 * Left_Rirection];       //1，1，-1，-1

                if (imageInput[Pixel_Left_Ysite][Pixel_Left_Xsite] == 0)//左前方为黑色
                {
                    //方向不变  Left_Rirection
                    Left_Ysite = Left_Ysite + Left_Rule[0][2 * Left_Rirection + 1];
                    Left_Xsite = Left_Xsite + Left_Rule[0][2 * Left_Rirection];
                    if (ImageDeal[Left_Ysite].LeftBoundary_First == 0)
                        ImageDeal[Left_Ysite].LeftBoundary_First = Left_Xsite;
                    ImageDeal[Left_Ysite].LeftBoundary = Left_Xsite;
                }
                else//左前方为白色
                {
                    // 方向发生改变 Left_Rirection  逆时针90度
                    Left_Ysite = Left_Ysite + Left_Rule[1][2 * Left_Rirection + 1];
                    Left_Xsite = Left_Xsite + Left_Rule[1][2 * Left_Rirection];
                    if (ImageDeal[Left_Ysite].LeftBoundary_First == 0 )
                        ImageDeal[Left_Ysite].LeftBoundary_First = Left_Xsite;
                    ImageDeal[Left_Ysite].LeftBoundary = Left_Xsite;
                    if (Left_Rirection == 0)
                        Left_Rirection = 3;
                    else
                        Left_Rirection--;
                }

            }
        }
        /*********右边巡线*******/
        if ((Pixel_Right_Ysite > Ysite) || Ysite == ImageStatus.OFFLineBoundary)//右边扫线
        {
            /*计算前方坐标*/
            Pixel_Right_Ysite = Right_Ysite + Right_Rule[0][2 * Right_Rirection + 1];
            Pixel_Right_Xsite = Right_Xsite + Right_Rule[0][2 * Right_Rirection];

            if (imageInput[Pixel_Right_Ysite][Pixel_Right_Xsite] == 0)//前方是黑色
            {
                //逆时针旋转90
                if (Right_Rirection == 0)
                    Right_Rirection = 3;
                else
                    Right_Rirection--;
            }
            else//前方是白色
            {
                /*计算右前方坐标*/
                Pixel_Right_Ysite = Right_Ysite + Right_Rule[1][2 * Right_Rirection + 1];
                Pixel_Right_Xsite = Right_Xsite + Right_Rule[1][2 * Right_Rirection];

                if (imageInput[Pixel_Right_Ysite][Pixel_Right_Xsite] == 0)//左前方为黑色
                {
                    //方向不变  Right_Rirection
                    Right_Ysite = Right_Ysite + Right_Rule[0][2 * Right_Rirection + 1];
                    Right_Xsite = Right_Xsite + Right_Rule[0][2 * Right_Rirection];
                    if (ImageDeal[Right_Ysite].RightBoundary_First == 79 )
                        ImageDeal[Right_Ysite].RightBoundary_First = Right_Xsite;
                    ImageDeal[Right_Ysite].RightBoundary = Right_Xsite;
                }
                else//左前方为白色
                {
                    // 方向发生改变 Right_Rirection  逆时针90度
                    Right_Ysite = Right_Ysite + Right_Rule[1][2 * Right_Rirection + 1];
                    Right_Xsite = Right_Xsite + Right_Rule[1][2 * Right_Rirection];
                    if (ImageDeal[Right_Ysite].RightBoundary_First == 79)
                        ImageDeal[Right_Ysite].RightBoundary_First = Right_Xsite;
                    ImageDeal[Right_Ysite].RightBoundary = Right_Xsite;
                    if (Right_Rirection == 3)
                        Right_Rirection = 0;
                    else
                        Right_Rirection++;
                }

            }
        }

        if (abs(Pixel_Right_Xsite - Pixel_Left_Xsite) < 3)//Ysite<80是为了放在底部是斑马线扫描结束  3 && Ysite < 30
        {

            ImageStatus.OFFLineBoundary = Ysite;
            break;
        }

    }
}


//延长线绘制，理论上来说是很准确的
static void DrawExtensionLine(void)        //绘制延长线并重新确定中线 ，把补线补成斜线
{
  if (
          ImageFlag.image_element_rings == 0
       && ImageFlag.Zebra_Flag == 0
       && ImageFlag.RoadBlock_Flag == 0
      )
  {
    if (ImageStatus.WhiteLine >= ImageStatus.TowPoint_True - 15)
    { TFSite = 55;}
    if (ImageStatus.CirqueOff == 'T' && ImageFlag.image_element_rings == 1)
    { TFSite = 55;}
    if (ExtenLFlag != 'F')
      for (Ysite = 54; Ysite >= (ImageStatus.OFFLine + 4);
           Ysite--)                       //从第五行开始网上扫扫到顶边下面两行   多段补线
                                          //不仅仅只有一段
      {
        PicTemp = Pixle[Ysite];           //存当前行
        if (ImageDeal[Ysite].IsLeftFind =='W')                          //如果本行左边界没扫到但扫到的是白色，说明本行没有左边界点
        {
          //**************************************************//**************************************************
          if (ImageDeal[Ysite + 1].LeftBorder >= 70)                    //如果左边界实在是太右边
          {
            ImageStatus.OFFLine = Ysite + 1;
            break;                        //直接跳出（极端情况）
          }
          //************************************************//*************************************************

          while (Ysite >= (ImageStatus.OFFLine + 4))                    //此时还没扫到顶边
          {
            Ysite--;                      //继续往上扫
            if (  ImageDeal[Ysite].IsLeftFind == 'T'
                &&ImageDeal[Ysite - 1].IsLeftFind == 'T'
                &&ImageDeal[Ysite - 2].IsLeftFind == 'T'
                &&ImageDeal[Ysite - 2].LeftBorder > 0
                &&ImageDeal[Ysite - 2].LeftBorder <70
                )                                                       //如果扫到本行出现了并且本行以上连续三行都有左边界点（左边界在空白上方）
            {
              FTSite = Ysite - 2;          //把本行上面的第二行存入FTsite
              break;
            }
          }

          DetL =
              ((float)(ImageDeal[FTSite].LeftBorder -
                       ImageDeal[TFSite].LeftBorder)) /
              ((float)(FTSite - TFSite));  //左边界的斜率：列的坐标差/行的坐标差
          if (FTSite > ImageStatus.OFFLine)
            for (
                ytemp = TFSite; ytemp >= FTSite; ytemp--)               //从第一次扫到的左边界的下面第二行的坐标开始往上扫直到空白上方的左边界的行坐标值
            {
              ImageDeal[ytemp].LeftBorder =
                  (int)(DetL * ((float)(ytemp - TFSite))) +
                  ImageDeal[TFSite]
                      .LeftBorder;                                      //将这期间的空白处补线（补斜线），目的是方便图像处理
            }
        } else
          TFSite = Ysite + 2;                                           //如果扫到了本行的左边界，该行存在这里面，（算斜率）
      }

    if (ImageStatus.WhiteLine >= ImageStatus.TowPoint_True - 15)
      TFSite = 55;
    // g5.22
    if (ImageStatus.CirqueOff == 'T' && ImageFlag.image_element_rings == 2)
      TFSite = 55;
    if (ExtenRFlag != 'F')
      for (Ysite = 54; Ysite >= (ImageStatus.OFFLine + 4);
           Ysite--)               //从第五行开始网上扫扫到顶边下面两行
      {
        PicTemp = Pixle[Ysite];  //存当前行

        if (ImageDeal[Ysite].IsRightFind =='W')                       //如果本行右边界没扫到但扫到的是白色，说明本行没有右边界点，但是处于赛道内的
        {
          if (ImageDeal[Ysite + 1].RightBorder <= 10)                 //如果右边界实在是太左边
          {
            ImageStatus.OFFLine =Ysite + 1;                           //直接跳出，说明这种情况赛道就尼玛离谱
            break;
          }
          while (Ysite >= (ImageStatus.OFFLine + 4))                  //此时还没扫到顶边下面两行
          {
            Ysite--;
            if (  ImageDeal[Ysite].IsRightFind == 'T'
                &&ImageDeal[Ysite - 1].IsRightFind == 'T'
                &&ImageDeal[Ysite - 2].IsRightFind == 'T'
                &&ImageDeal[Ysite - 2].RightBorder < 70
                &&ImageDeal[Ysite - 2].RightBorder > 10
                )                                                      //如果扫到本行出现了并且本行以上连续三行都有左边界点（左边界在空白上方）
            {
              FTSite = Ysite - 2;                                      // 把本行上面的第二行存入FTsite
              break;
            }
          }

          DetR =((float)(ImageDeal[FTSite].RightBorder -ImageDeal[TFSite].RightBorder)) /((float)(FTSite - TFSite));         //右边界的斜率：列的坐标差/行的坐标差
          if (FTSite > ImageStatus.OFFLine)
            for (ytemp = TFSite; ytemp >= FTSite;ytemp--)              //从第一次扫到的右边界的下面第二行的坐标开始往上扫直到空白上方的右边界的行坐标值
            {
              ImageDeal[ytemp].RightBorder =(int)(DetR * ((float)(ytemp - TFSite))) +ImageDeal[TFSite].RightBorder;          //将这期间的空白处补线（补斜线），目的是方便图像处理
            }
        } else
          TFSite =Ysite +2;                                           //如果本行的右边界找到了，则把该行下面第二行坐标送个TFsite
      }
  }
  for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--) {
    ImageDeal[Ysite].Center =(ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder) /2;                                //扫描结束，把这一块经优化之后的中间值存入
    ImageDeal[Ysite].Wide =-ImageDeal[Ysite].LeftBorder +ImageDeal[Ysite].RightBorder;                                       //把优化之后的宽度存入
  }
}

//出现丢边的时候  重新确定无边行的中线   从第一个上方有边行到下方的无边行中心做斜率重新整定
static void RouteFilter(void) {
  for (Ysite = 58; Ysite >= (ImageStatus.OFFLine + 5);
       Ysite--)                                     //从开始位到停止位
  {
    if (   ImageDeal[Ysite].IsLeftFind == 'W'
         &&ImageDeal[Ysite].IsRightFind == 'W'
         &&Ysite <= 45
         &&ImageDeal[Ysite - 1].IsLeftFind == 'W'
         &&ImageDeal[Ysite - 1].IsRightFind =='W')  //当前行左右都无边，而且在前45行   滤波
    {
      ytemp = Ysite;
      while (ytemp >= (ImageStatus.OFFLine +5))     // 改改试试，-6效果好一些
      {
        ytemp--;
        if (  ImageDeal[ytemp].IsLeftFind == 'T'
            &&ImageDeal[ytemp].IsRightFind == 'T')  //寻找两边都正常的，找到离本行最近的就不找了
        {
          DetR = (float)(ImageDeal[ytemp - 1].Center - ImageDeal[Ysite + 2].Center) /(float)(ytemp - 1 - Ysite - 2);          //算斜率
          int CenterTemp = ImageDeal[Ysite + 2].Center;
          int LineTemp = Ysite + 2;
          while (Ysite >= ytemp) {
            ImageDeal[Ysite].Center =(int)(CenterTemp +DetR * (float)(Ysite - LineTemp));                                     //用斜率补    y=y0+K(x-x0)
            Ysite--;
          }
          break;
        }
      }
    }
    ImageDeal[Ysite].Center =(ImageDeal[Ysite - 1].Center + 2 * ImageDeal[Ysite].Center) /3;                                  //求平均，应该会比较滑  本来是上下两点平均
  }
}



//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//  @name           Search_Border_OTSU
//  @brief          通过OTSU获取边线 和信息
//  @param          imageInput[IMAGE_ROW][IMAGE_COL]        传入的图像数组
//  @param          Row                                     图像的Ysite
//  @param          Col                                     图像的Xsite
//  @param          Bottonline                              底边行选择
//  @return         无
//  @time           2022年10月7日
//  @Author         陈新云
//  Sample usage:   Search_Border_OTSU(mt9v03x_image, IMAGE_ROW, IMAGE_COL, IMAGE_ROW-8);
//--------------------------------------------------------------------------------------------------------------------------------------------

void Search_Border_OTSU(uint8 imageInput[LCDH][LCDW], uint8 Row, uint8 Col, uint8 Bottonline)
{
    ImageStatus.WhiteLine_L = 0;
    ImageStatus.WhiteLine_R = 0;//一帧图像进来一次，把上一帧的丢边情况清零，重新处理
    //ImageStatus.OFFLine = 1;
    /*封上下边界处理*/       //防止扫线时图像长出去
    for (int Xsite = 0; Xsite < LCDW; Xsite++)      //最上面一行和第58行全部都置0，黑色封边
    {
        imageInput[0][Xsite] = 0;
        imageInput[Bottonline + 1][Xsite] = 0;
    }
    /*封左右边界处理*/
    for (int Ysite = 0; Ysite < LCDH; Ysite++)      //最左边一列和最右边一列全部都置0，黑色封边
    {
            ImageDeal[Ysite].LeftBoundary_First = 0;
            ImageDeal[Ysite].RightBoundary_First = 79;

            imageInput[Ysite][0] = 0;
            imageInput[Ysite][LCDW - 1] = 0;
    }
    /********获取底部边线*********/
    Search_Bottom_Line_OTSU(imageInput, Row, Col, Bottonline);//39列左右扫边获得58行的左右边线
    /********获取左右边线*********/
    Search_Left_and_Right_Lines(imageInput, Row, Col, Bottonline);     //上交迷宫向上爬出左右边界



    for (int Ysite = Bottonline; Ysite > ImageStatus.OFFLineBoundary + 1; Ysite--)
    {
        if (ImageDeal[Ysite].LeftBoundary < 3)
        {
            ImageStatus.WhiteLine_L++;                      
        }
        if (ImageDeal[Ysite].RightBoundary > LCDW - 3)
        {
            ImageStatus.WhiteLine_R++;
        }
    }
}

/*****************直线判断******************/
float Straight_Judge(uint8 dir, uint8 start, uint8 end)     //返回结果小于1即为直线
{
    int i;
    float S = 0, Sum = 0, Err = 0, k = 0;
    switch (dir)
    {
    case 1:k = (float)(ImageDeal[start].LeftBorder - ImageDeal[end].LeftBorder) / (start - end);
        for (i = 0; i < end - start; i++)
        {
            Err = (ImageDeal[start].LeftBorder + k * i - ImageDeal[i + start].LeftBorder) * (ImageDeal[start].LeftBorder + k * i - ImageDeal[i + start].LeftBorder);
            Sum += Err;
        }
        S = Sum / (end - start);
        break;
    case 2:k = (float)(ImageDeal[start].RightBorder - ImageDeal[end].RightBorder) / (start - end);
        for (i = 0; i < end - start; i++)
        {
            Err = (ImageDeal[start].RightBorder + k * i - ImageDeal[i + start].RightBorder) * (ImageDeal[start].RightBorder + k * i - ImageDeal[i + start].RightBorder);
            Sum += Err;
        }
        S = Sum / (end - start);
        break;
    }
    return S;
}
//用于加速的直道检测
//用中线的方差和可视距离进行直道检测
float variance, variance_acc;  //方差
void Straightacc_Test(void) {
  int sum = 0;
  for (Ysite = 55; Ysite > ImageStatus.OFFLine + 1; Ysite--) {
    sum += (ImageDeal[Ysite].Center - ImageStatus.MiddleLine) *(ImageDeal[Ysite].Center - ImageStatus.MiddleLine);}

  variance_acc = (float)sum / (54 - ImageStatus.OFFLine);

  if ( variance_acc < ImageStatus.variance_acc                          //和30比大小
    && ImageStatus.OFFLine <= 10  //  && ImageStatus.Road_type == 0
   // &&SystemData.SpeedData.Length*OX>350
    ) {
      ImageStatus.straight_acc = 1;
      ImageFlag.straight_long = 1;}
  else{
      ImageStatus.straight_acc = 0;
      ImageFlag.straight_long = 0;
      }
}
//--------------------------------------------------------------
//  @name           Element_Judgment_Left_Rings()
//  @brief          整个图像判断的子函数，用来判断左圆环类型.
//  @parameter      void
//  @time
//  @Author         MRCHEN
//  Sample usage:   Element_Judgment_Left_Rings();
//--------------------------------------------------------------
int ceshi_flag=0;
void Element_Judgment_Left_Rings()
{
    if (   ImageStatus.Miss_Right_lines > 5 || ImageStatus.Miss_Left_lines < 13
        || ImageStatus.OFFLine > 12                //可能要变大点//12
         ||   Straight_Judge(2, 5, 55) > 1
            || ImageStatus.WhiteLine>4
        || ImageDeal[52].IsLeftFind == 'W'
        || ImageDeal[53].IsLeftFind == 'W'
        || ImageDeal[54].IsLeftFind == 'W'
        || ImageDeal[55].IsLeftFind == 'W'      //  如果底边都是丢边的话肯定是判断不了的，所以就直接跳出圆环的判断函数
        || ImageDeal[56].IsLeftFind == 'W'      //调试的时候可以先把这部分注释掉
        || ImageDeal[57].IsLeftFind == 'W'
        || ImageDeal[58].IsLeftFind == 'W'

    )
      return;
   if(ImageFlag.image_element_rings == 1){return;}

    ceshi_flag=1;

    int ring_ysite = 25;
    Left_RingsFlag_Point1_Ysite = 0;
    Left_RingsFlag_Point2_Ysite = 0;

    for (int Ysite = 58; Ysite > ring_ysite; Ysite--)
    {
        if (ImageDeal[Ysite].LeftBoundary_First - ImageDeal[Ysite - 1].LeftBoundary_First > 4)  //左圆环的左下拐点
        {
            Left_RingsFlag_Point1_Ysite = Ysite;
            break;
        }
    }
    for (int Ysite = 58; Ysite > ring_ysite; Ysite--)
    {
        if (ImageDeal[Ysite + 1].LeftBoundary - ImageDeal[Ysite].LeftBoundary > 4)          //左圆环的左上拐点
        {
            Left_RingsFlag_Point2_Ysite = Ysite;
            break;
        }
    }

    for (int Ysite = Left_RingsFlag_Point1_Ysite; Ysite > ImageStatus.OFFLine; Ysite--)     //从左圆环的左下拐点往上搜，寻找圆环内圈的左小弧：） 的中点
    {
        if (ImageDeal[Ysite + 6].LeftBorder < ImageDeal[Ysite+3].LeftBorder
            && ImageDeal[Ysite + 5].LeftBorder < ImageDeal[Ysite+3].LeftBorder         // 寻找圆环内圈的左小弧：） 的中点
            && ImageDeal[Ysite+3].LeftBorder > ImageDeal[Ysite + 2].LeftBorder         //这个是只有圆弧才有的特征，只有搜到这个小圆弧，圆环标志位才会置1
            && ImageDeal[Ysite+3].LeftBorder > ImageDeal[Ysite + 1].LeftBorder
            )
        {
            Ring_Help_Flag = 1;
            break;
        }
    }
    if(Left_RingsFlag_Point2_Ysite > Left_RingsFlag_Point1_Ysite+3 && Ring_Help_Flag == 0)  //如果进环标志位没有被置起，那么会进行第二次对于进环的判断
    {
        if(ImageStatus.Miss_Left_lines>10)
            Ring_Help_Flag = 1;
    }
    if (Left_RingsFlag_Point2_Ysite > Left_RingsFlag_Point1_Ysite+3
		&& Ring_Help_Flag == 1)
    {
        ImageFlag.image_element_rings = 1;
        ImageFlag.image_element_rings_flag = 1;      //这个是圆环处理的真正标志位
        circle_num++;
        if(circle_num == position)
            ImageFlag.ring_big_small=2;
        else
            ImageFlag.ring_big_small=1;
    }
    Ring_Help_Flag = 0;
}


//--------------------------------------------------------------
//  @name           Element_Judgment_Right_Rings()
//  @brief          整个图像判断的子函数，用来判断右圆环类型.
//  @parameter      void
//  @time
//  @Author         MRCHEN
//  Sample usage:   Element_Judgment_Right_Rings();
//--------------------------------------------------------------
void Element_Judgment_Right_Rings()
{
    if (   ImageStatus.Miss_Left_lines > 5 || ImageStatus.Miss_Right_lines < 13
        || ImageStatus.OFFLine > 12
        || Straight_Judge(1, 5, 55) > 1
        || ImageStatus.WhiteLine>4
        || ImageDeal[52].IsRightFind == 'W'
        || ImageDeal[53].IsRightFind == 'W'         //  如果底边都是丢边的话肯定是判断不了的，所以就直接跳出圆环的判断函数
        || ImageDeal[54].IsRightFind == 'W'          //调试的时候可以先把这部分注释掉
        || ImageDeal[55].IsRightFind == 'W'
        || ImageDeal[56].IsRightFind == 'W'
        || ImageDeal[57].IsRightFind == 'W'
        || ImageDeal[58].IsRightFind == 'W'
        )
    { return;}

    if(ImageFlag.image_element_rings == 2){return;}

    int ring_ysite = 25;
    Right_RingsFlag_Point1_Ysite = 0;
    Right_RingsFlag_Point2_Ysite = 0;
    for (int Ysite = 58; Ysite > ring_ysite; Ysite--)
    {
        if (ImageDeal[Ysite - 1].RightBoundary_First - ImageDeal[Ysite].RightBoundary_First > 4)     //右圆环的右下拐点
        {
            Right_RingsFlag_Point1_Ysite = Ysite;
            break;
        }
    }
    for (int Ysite = 58; Ysite > ring_ysite; Ysite--)
    {
        if (ImageDeal[Ysite].RightBoundary - ImageDeal[Ysite + 1].RightBoundary > 4)                //右圆环的右上拐点
        {
            Right_RingsFlag_Point2_Ysite = Ysite;
            break;
        }
    }
    for (int Ysite = Right_RingsFlag_Point1_Ysite; Ysite > ImageStatus.OFFLine; Ysite--)                             //从右圆环的右下拐点往上搜，寻找圆环内圈的右小弧：（ 的中点
    {
        if (   ImageDeal[Ysite + 6].RightBorder > ImageDeal[Ysite + 3].RightBorder
            && ImageDeal[Ysite + 5].RightBorder > ImageDeal[Ysite + 3].RightBorder                 // 寻找圆环内圈的右小弧：（ 的中点
            && ImageDeal[Ysite + 3].RightBorder < ImageDeal[Ysite + 2].RightBorder                 //这个是只有圆弧才有的特征，只有搜到这个小圆弧，圆环标志位才会置1
            && ImageDeal[Ysite + 3].RightBorder < ImageDeal[Ysite + 1].RightBorder
           )
        {
            Ring_Help_Flag = 1;
            break;
        }
    }
    if(Right_RingsFlag_Point2_Ysite > Right_RingsFlag_Point1_Ysite+15 && Ring_Help_Flag == 0)//如果进环标志位没有被置起，那么会进行第二次对于进环的判断
    {
        if(ImageStatus.Miss_Right_lines>10)
            Ring_Help_Flag = 1;
    }
    if (Right_RingsFlag_Point2_Ysite > Right_RingsFlag_Point1_Ysite+3 &&        //试着吧一个条件删除
            Ring_Help_Flag == 1)
    {
        ImageFlag.image_element_rings = 2;
        ImageFlag.image_element_rings_flag = 1;         //这个是圆环处理的真正标志位
        circle_num++;
        if(circle_num == position)
            ImageFlag.ring_big_small=2;     //大环
        else
            ImageFlag.ring_big_small=1;     //小环
    }

        //ips200_show_uint(100,220,ImageFlag.image_element_rings,3);
    Ring_Help_Flag = 0;
}


//--------------------------------------------------------------
//  @name           Element_Handle_Left_Rings()
//  @brief          整个图像处理的子函数，用来处理左圆环类型.
//  @parameter      void
//  @time
//  @Author         MRCHEN
//  Sample usage:   Element_Handle_Left_Rings();
//-------------------------------------------------------------
void Element_Handle_Left_Rings()
{
    /***************************************判断**************************************/
    int num = 0;
    for (int Ysite = 55; Ysite > 30; Ysite--)
    {
        if(ImageDeal[Ysite].IsLeftFind == 'W')
        {
            num++;      //计算左边赛道的无边行数（丢边数）
        }
        if(    ImageDeal[Ysite+3].IsLeftFind == 'W' && ImageDeal[Ysite+2].IsLeftFind == 'W'//圆环的中心小圆环下面的第一条有边行
            && ImageDeal[Ysite+1].IsLeftFind == 'W' && ImageDeal[Ysite].IsLeftFind == 'T' ) //在圆环的弯道时会出现这种情况
            break;
    }
        //准备进环                                                                                                  //状态在1 2 3 4 时进行半宽处理
    if (ImageFlag.image_element_rings_flag == 1 && num>10) //左边的无边行（丢线）大于10时此时进入2状态
    {
        ImageFlag.image_element_rings_flag = 2;
    }
    if (ImageFlag.image_element_rings_flag == 2 && num<5)   //一开始无边行多，随着车往前走，无边行的数目减少。此时进入5状态
    {
        ImageFlag.image_element_rings_flag = 5;             //当5状态时车身已经基本处于环岛的中间了，此时就可以寻找环岛的左上角点进行拉线了
        ceshi_flag=3;
    }
        //刚到圆弧
    if(ImageFlag.image_element_rings_flag == 4)
    {
        //Stop= 1;
        Point_Ysite = 0;
        Point_Xsite = 0;
        for (int Ysite = 20; Ysite > ImageStatus.OFFLine + 2; Ysite--)
        {
            if(ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite-1].IsLeftFind == 'T')
            {
                Point_Ysite = Ysite;
                Point_Xsite = 0;
                break;
            }
        }
        if(Point_Ysite > 6 && ImageFlag.ring_big_small == 1)
        {
            ImageFlag.image_element_rings_flag=5;
        }
    }
        //进环
    if(ImageFlag.image_element_rings_flag == 5 && ImageStatus.Miss_Right_lines>15)  //从5，6开始就要开始准备拉线了（结合下面处理部分来看）
    {
        ImageFlag.image_element_rings_flag = 6;
    }
        //进环小圆环
    if(ImageFlag.image_element_rings_flag == 6 && ImageStatus.Miss_Right_lines<7)//因此此时已经入环拉线了，小车向左拐了。随着左转一开始的时候右边的丢边数很多，但是随着左拐的进行，丢边数量开始逐渐减少。等到完全进入环岛就不丢边了
    {
        ImageFlag.image_element_rings_flag = 7;//一旦右边的丢边数小于7时此时说明已经进入了环岛了。标志位7的时候不做任何处理，就是常规扫线巡线
    }
    if (ImageFlag.image_element_rings_flag == 7)    //此时在环岛里面进行环岛的出环拐点的扫线判断
    {
        Point_Ysite = 0;
        Point_Xsite = 0;
        for (int Ysite = 50; Ysite > ImageStatus.OFFLine + 3; Ysite--)
        {
            if (    ImageDeal[Ysite].RightBorder <= ImageDeal[Ysite + 2].RightBorder
                 && ImageDeal[Ysite].RightBorder <= ImageDeal[Ysite - 2].RightBorder
                 && ImageDeal[Ysite].RightBorder <= ImageDeal[Ysite + 1].RightBorder
                 && ImageDeal[Ysite].RightBorder <= ImageDeal[Ysite - 1].RightBorder    //这部分就是在找出环状态时左圆环的左下拐点，然后和图像左上角拉线就可以正常驶出了
                 && ImageDeal[Ysite].RightBorder <= ImageDeal[Ysite + 4].RightBorder
                 && ImageDeal[Ysite].RightBorder <= ImageDeal[Ysite - 4].RightBorder
               )
            {
                Point_Xsite = ImageDeal[Ysite].RightBorder;         //把这个拐点存起来
                Point_Ysite = Ysite;
                break;
            }
        }
        if (Point_Ysite > 22)
          ImageFlag.image_element_rings_flag = 8;       //如果找到了这个出环拐点，那么就置起出环补线标志位

    }
        //出环后
    if (ImageFlag.image_element_rings_flag == 8)
    {
        if (    Straight_Judge(2, ImageStatus.OFFLine+15, 50) < 1
             && ImageStatus.Miss_Right_lines < 8
             && ImageStatus.OFFLine < 7)    //右边为直线且截止行（前瞻值）很小         //出环offline

            ImageFlag.image_element_rings_flag = 9;//此时进入状态9

    }
        //结束圆环进程    //圆环所有状态清0
    if (ImageFlag.image_element_rings_flag == 9/* && ImageFlag.ring_big_small==1*/)
    {
        int num=0;
        for (int Ysite = 40; Ysite > 10; Ysite--)
        {
            if(ImageDeal[Ysite].IsLeftFind == 'W' )
                num++;
        }
//        }
        if( num<5 )          //刚出圆环的时候，再次走到环岛中心向前一点点是存在左边丢边的。因此当左边的丢边数减小到足够小时说明已经彻底远离环岛。因此此时将环岛的所有状态标志位清0。
        {                   //这时环岛由于处于9标志位，只有当远离环岛时才会清零转态，因此具有防止二次误判的功能。
            ImageFlag.image_element_rings_flag = 0;
            ImageFlag.image_element_rings = 0;
            ImageFlag.ring_big_small = 0;
            Front_Ring_Continue_Count++;
//            gpio_set_level(B0, 0);
        }
    }

    /***************************************处理**************************************/
        //准备进环  半宽处理
    if (   ImageFlag.image_element_rings_flag == 1
        || ImageFlag.image_element_rings_flag == 2
        || ImageFlag.image_element_rings_flag == 3
        || ImageFlag.image_element_rings_flag == 4) //满足任意一个条件即可进行半宽处理
    {
        for (int Ysite = 59; Ysite > ImageStatus.OFFLine; Ysite--)
        {
            ImageDeal[Ysite].Center = ImageDeal[Ysite].RightBorder - Half_Road_Wide[Ysite]; //更改赛道中线，使得路径偏向赛道右侧
        }
    }
        //进环  补线
    if  ( ImageFlag.image_element_rings_flag == 5
        ||ImageFlag.image_element_rings_flag == 6
        )
    {
        int  flag_Xsite_1=0;
        int flag_Ysite_1=0;
        float Slope_Rings=0;
        for(Ysite=55;Ysite>ImageStatus.OFFLine;Ysite--)//寻找环岛的左上拐点
        {
            for(Xsite=ImageDeal[Ysite].LeftBorder + 1;Xsite<ImageDeal[Ysite].RightBorder - 1;Xsite++)
            {
                if(  Pixle[Ysite][Xsite] == 1 && Pixle[Ysite][Xsite + 1] == 0)
                 {
                   flag_Ysite_1 = Ysite;
                   flag_Xsite_1 = Xsite;
                   Slope_Rings=(float)(79-flag_Xsite_1)/(float)(59-flag_Ysite_1);    //环岛的左上拐点到图像右下点的斜率
                   break;
                 }
            }
            if(flag_Ysite_1 != 0)
            {
                break;
            }
        }
        if(flag_Ysite_1 == 0)
        {
            for(Ysite=ImageStatus.OFFLine+1;Ysite<30;Ysite++)//第二次寻找环岛的左上拐点
            {
                if(ImageDeal[Ysite].IsLeftFind=='T'&&ImageDeal[Ysite+1].IsLeftFind=='T'&&ImageDeal[Ysite+2].IsLeftFind=='W'
                    &&abs(ImageDeal[Ysite].LeftBorder-ImageDeal[Ysite+2].LeftBorder)>10
                  )
                {
                    flag_Ysite_1=Ysite;
                    flag_Xsite_1=ImageDeal[flag_Ysite_1].LeftBorder;
                    ImageStatus.OFFLine=Ysite;
                    Slope_Rings=(float)(79-flag_Xsite_1)/(float)(59-flag_Ysite_1);          //环岛的左上拐点到图像右下点的斜率
                    break;
                }

            }
        }
        //补线
        if(flag_Ysite_1 != 0)     //如果找到了左上拐点那么就进行补线
        {
            for(Ysite=flag_Ysite_1;Ysite<60;Ysite++)//从左上拐点开始向下进行补线
            {
                ImageDeal[Ysite].RightBorder=flag_Xsite_1+Slope_Rings*(Ysite-flag_Ysite_1);     //y=y0-K(x=x0);从左上拐点补到图像的右下点
                    ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder)/2;//重新计算中线
                if(ImageDeal[Ysite].Center<0)
                    ImageDeal[Ysite].Center = 0;
            }
            ImageDeal[flag_Ysite_1].RightBorder=flag_Xsite_1;//左上拐点到右下点的补线完成

            for(Ysite=flag_Ysite_1-1;Ysite>10;Ysite--) //左上拐点上方进行扫线
            {
                for(Xsite=ImageDeal[Ysite+1].RightBorder-10;Xsite<ImageDeal[Ysite+1].RightBorder+2;Xsite++)
                {
                    if(Pixle[Ysite][Xsite]==1 && Pixle[Ysite][Xsite+1]==0)
                    {
                        ImageDeal[Ysite].RightBorder=Xsite;
                            ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder)/2;
                        if(ImageDeal[Ysite].Center<0)
                            ImageDeal[Ysite].Center = 0;
                        ImageDeal[Ysite].Wide=ImageDeal[Ysite].RightBorder-ImageDeal[Ysite].LeftBorder;
                        break;
                    }
                }

                if(ImageDeal[Ysite].Wide>8 &&ImageDeal[Ysite].RightBorder< ImageDeal[Ysite+2].RightBorder)
                {
                    continue;
                }
                else
                {
                    ImageStatus.OFFLine=Ysite+2;
                    break;
                }
            }
        }
    }
        //环内 小环弯道减半宽 大环不减
    if (ImageFlag.image_element_rings_flag == 7)//状态7就是正常的巡线，不需要处理
    {
    }
        //大圆环出环 补线
    if (ImageFlag.image_element_rings_flag == 8)    //大圆环
    {
        Repair_Point_Xsite = 20;
        Repair_Point_Ysite = 0;
        for (int Ysite = 40; Ysite > 5; Ysite--)
        {
            if (Pixle[Ysite][28] == 1 && Pixle[Ysite-1][28] == 0)//30   //寻找出环补线的上边界，此时以上边界的28列的点和出环时圆环的左下拐点 作为补线的两个点
            {                                                           //上边界的列补线点（28）可以自己调整
                Repair_Point_Xsite = 28;
                Repair_Point_Ysite = Ysite-1;
                ImageStatus.OFFLine = Ysite + 1;  //截止行重新规划
                break;
            }
        }
        for (int Ysite = 57; Ysite > Repair_Point_Ysite-3; Ysite--)         //出环补线,这个是上边界的28列和58行的右边线斜拉补线，不应该是和圆环的左下拐点进行拉线吗？？？
        {
            ImageDeal[Ysite].RightBorder = (ImageDeal[58].RightBorder - Repair_Point_Xsite) * (Ysite - 58) / (58 - Repair_Point_Ysite)  + ImageDeal[58].RightBorder;//y=（y1-y2）/(x1-x2)*(x-x1)+y1
            ImageDeal[Ysite].Center =ImageDeal[Ysite].RightBorder-Half_Road_Wide[Ysite] ;
        }
    }
        //已出环 半宽处理
    if (ImageFlag.image_element_rings_flag == 9)
    {
        for (int Ysite = 59; Ysite > ImageStatus.OFFLine; Ysite--)
        {
            ImageDeal[Ysite].Center = ImageDeal[Ysite].RightBorder - Half_Road_Wide[Ysite];     //防止出圆环时圆环内切，因此左圆环出环靠右行驶
        }
    }
}


//--------------------------------------------------------------
//  @name           Element_Handle_Right_Rings()
//  @brief          整个图像处理的子函数，用来处理右圆环类型.
//  @parameter      void
//  @time
//  @Author         MRCHEN
//  Sample usage:   Element_Handle_Right_Rings();
//-------------------------------------------------------------
void Element_Handle_Right_Rings()
{
    /****************判断*****************/
    int num =0 ;
    for (int Ysite = 55; Ysite > 30; Ysite--)
    {
        if(ImageDeal[Ysite].IsRightFind == 'W')
        {
            num++;
        }
        if(    ImageDeal[Ysite+3].IsRightFind == 'W' && ImageDeal[Ysite+2].IsRightFind == 'W'  //圆环的中心小圆环下面的第一条有边行
            && ImageDeal[Ysite+1].IsRightFind == 'W' && ImageDeal[Ysite].IsRightFind == 'T' ) //在圆环的弯道时会出现这种情况
            break;
    }
            //准备进环                                                                                                  //状态在1 2 3 4 时进行半宽处理
    if (ImageFlag.image_element_rings_flag == 1 && num>10)  //右边的无边行（丢线）大于10时此时进入2状态
    {
        ImageFlag.image_element_rings_flag = 2;
    }
    if (ImageFlag.image_element_rings_flag == 2 && num<5)//一开始无边行多，随着车往前走，无边行的数目减少。此时进入5状态
    {
        ImageFlag.image_element_rings_flag = 5;          //当5状态时车身已经基本处于环岛的中间了，此时就可以寻找环岛的左上角点进行拉线了
    }
        //进环
    if(ImageFlag.image_element_rings_flag == 5 && ImageStatus.Miss_Left_lines>15) //从5，6开始就要开始准备拉线了（结合下面处理部分来看）
    {
        ImageFlag.image_element_rings_flag = 6;
    }
        //进环小圆环
    if(ImageFlag.image_element_rings_flag == 6 && ImageStatus.Miss_Left_lines<4)//因此此时已经入环拉线了，小车向右拐了。随着右转一开始的时候左边的丢边数很多，但是随着右拐的进行，丢边数量开始逐渐减少。等到完全进入环岛就不丢边了
    {
        ImageFlag.image_element_rings_flag = 7;     //一旦右边的丢边数小于7时此时说明已经进入了环岛了。标志位7的时候不做任何处理，就是常规扫线巡线
        //Stop=1;
    }
    if (ImageFlag.image_element_rings_flag == 7) //此时在环岛里面进行环岛的出环拐点的扫线判断
    {
        Point_Xsite = 0;
        Point_Ysite = 0;
        for (int Ysite = 45; Ysite > ImageStatus.OFFLine+3; Ysite--)
        {
            if (    ImageDeal[Ysite].LeftBorder >= ImageDeal[Ysite + 2].LeftBorder
                 && ImageDeal[Ysite].LeftBorder >= ImageDeal[Ysite - 2].LeftBorder
                 && ImageDeal[Ysite].LeftBorder >= ImageDeal[Ysite + 1].LeftBorder
                 && ImageDeal[Ysite].LeftBorder >= ImageDeal[Ysite - 1].LeftBorder
                 && ImageDeal[Ysite].LeftBorder >= ImageDeal[Ysite + 4].LeftBorder   //这部分就是在找出环状态时左圆环的右下拐点，然后和图像右上角拉线就可以正常驶出了
                 && ImageDeal[Ysite].LeftBorder >= ImageDeal[Ysite - 4].LeftBorder
                )

            {
                        Point_Xsite = ImageDeal[Ysite].LeftBorder;
                        Point_Ysite = Ysite;            //把这个拐点存起来
                        break;
            }
        }
        if (Point_Ysite > 22)
        {
            //Stop = 1;
            ImageFlag.image_element_rings_flag = 8;//如果找到了这个出环拐点，那么就置起出环补线标志位
        }
    }
        //环内 小圆环判断
//        //出环后
    if (ImageFlag.image_element_rings_flag == 8)
    {
         if (   Straight_Judge(1, ImageStatus.OFFLine+10, 45) < 1
             && ImageStatus.Miss_Left_lines < 8
             && ImageStatus.OFFLine < 7)    //右边为直线且截止行（前瞻值）很小
            {ImageFlag.image_element_rings_flag = 9;}//此时进入状态9

    }
    //结束圆环进程    //圆环所有状态清0
    if(ImageFlag.image_element_rings_flag == 9 )
    {
        int num=0;
        for (int Ysite = 40; Ysite > 10; Ysite--)
        {
            if(ImageDeal[Ysite].IsRightFind == 'W' )
            {
                num++;
            }
        }
        if(num < 5)     //刚出圆环的时候，再次走到环岛中心向前一点点是存在左边丢边的。因此当左边的丢边数减小到足够小时说明已经彻底远离环岛。因此此时将环岛的所有状态标志位清0。
        {                //这时环岛由于处于9标志位，只有当远离环岛时才会清零转态，因此具有防止二次误判的功能。
            ImageFlag.image_element_rings_flag = 0;
            ImageFlag.image_element_rings = 0;
            ImageFlag.ring_big_small = 0;
            Front_Ring_Continue_Count++;
        }
    }
    /***************************************处理**************************************/
         //准备进环  半宽处理
    if (   ImageFlag.image_element_rings_flag == 1
        || ImageFlag.image_element_rings_flag == 2
        || ImageFlag.image_element_rings_flag == 3
        || ImageFlag.image_element_rings_flag == 4)//满足任意一个条件即可进行半宽处理
    {
        for (int Ysite = 59; Ysite > ImageStatus.OFFLine; Ysite--)
        {
            ImageDeal[Ysite].Center = ImageDeal[Ysite].LeftBorder + Half_Road_Wide[Ysite];//更改赛道中线，使得路径偏向赛道左侧
        }
    }

        //进环  补线
    if (   ImageFlag.image_element_rings_flag == 5
        || ImageFlag.image_element_rings_flag == 6
       )
    {
        int flag_Xsite_1=0;
        int  flag_Ysite_1=0;
        float Slope_Right_Rings = 0;
        for(Ysite=55;Ysite>ImageStatus.OFFLine;Ysite--)//寻找环岛的右上拐点
        {
            for(Xsite=ImageDeal[Ysite].LeftBorder + 1;Xsite<ImageDeal[Ysite].RightBorder - 1;Xsite++)
            {
                if(Pixle[Ysite][Xsite]==1 && Pixle[Ysite][Xsite+1]==0)
                {
                    flag_Ysite_1=Ysite;
                    flag_Xsite_1=Xsite;
                    Slope_Right_Rings=(float)(0-flag_Xsite_1)/(float)(59-flag_Ysite_1);//环岛的右上拐点到图像左下点的斜率
                    break;
                }
            }
            if(flag_Ysite_1!=0)
            {
              break;
            }
        }
        if(flag_Ysite_1==0)
        {
        for(Ysite=ImageStatus.OFFLine+1;Ysite<30;Ysite++)//第二次寻找环岛的右上拐点
        {
         if(ImageDeal[Ysite].IsRightFind=='T'&&ImageDeal[Ysite+1].IsRightFind=='T'&&ImageDeal[Ysite+2].IsRightFind=='W'
               &&abs(ImageDeal[Ysite].RightBorder-ImageDeal[Ysite+2].RightBorder)>10
         )
         {
             flag_Ysite_1=Ysite;
             flag_Xsite_1=ImageDeal[flag_Ysite_1].RightBorder;
             ImageStatus.OFFLine=Ysite;
             Slope_Right_Rings=(float)(0-flag_Xsite_1)/(float)(59-flag_Ysite_1); //环岛的右上拐点到图像左下点的斜率
             break;
         }

        }

        }
        //补线
        if(flag_Ysite_1!=0)//如果找到了右上拐点那么就进行补线
        {
            for(Ysite=flag_Ysite_1;Ysite<60;Ysite++)//从右上拐点开始向下进行补线
            {
                ImageDeal[Ysite].LeftBorder=flag_Xsite_1+Slope_Right_Rings*(Ysite-flag_Ysite_1);//y=y0-K(x=x0);从右上拐点补到图像的左下点
                    ImageDeal[Ysite].Center=(ImageDeal[Ysite].LeftBorder+ImageDeal[Ysite].RightBorder)/2;//板块  //重新计算中线
                if(ImageDeal[Ysite].Center>79)
                    ImageDeal[Ysite].Center=79;
            }
            ImageDeal[flag_Ysite_1].LeftBorder=flag_Xsite_1;//右上拐点到左下点的补线完成



            for(Ysite=flag_Ysite_1-1;Ysite>10;Ysite--)  //右上拐点上方进行扫线
            {
                for(Xsite=ImageDeal[Ysite+1].LeftBorder+8;Xsite>ImageDeal[Ysite+1].LeftBorder-4;Xsite--)
                {
                    if(Pixle[Ysite][Xsite]==1 && Pixle[Ysite][Xsite-1]==0)
                    {
                     ImageDeal[Ysite].LeftBorder=Xsite;
                     ImageDeal[Ysite].Wide=ImageDeal[Ysite].RightBorder-ImageDeal[Ysite].LeftBorder;
                         ImageDeal[Ysite].Center=(ImageDeal[Ysite].LeftBorder+ImageDeal[Ysite].RightBorder)/2;//板块
                     if(ImageDeal[Ysite].Center>79)
                         ImageDeal[Ysite].Center=79;
                     break;
                    }
                }
                if(ImageDeal[Ysite].Wide>8 && ImageDeal[Ysite].LeftBorder>  ImageDeal[Ysite+2].LeftBorder)
                {
                    continue;
                }
                else
                {
                    ImageStatus.OFFLine=Ysite+2;
                    break;
                }
            }
        }
    }
        //环内不处理  //状态7就是正常的巡线，不需要处理
    if (ImageFlag.image_element_rings_flag == 7)
    {
    }

        //大圆环出环 补线
    if (ImageFlag.image_element_rings_flag == 8)  //大圆环
    {
        Repair_Point_Xsite = 60;
        Repair_Point_Ysite = 0;
        for (int Ysite = 40; Ysite > 5; Ysite--)
        {
            if (Pixle[Ysite][58] == 1 && Pixle[Ysite-1][58] == 0)//寻找出环补线的上边界，此时以上边界的58列的点和出环时圆环的左下拐点 作为补线的两个点
            {
                Repair_Point_Xsite = 58;                             //上边界的列补线点（58）可以自己调整
                Repair_Point_Ysite = Ysite-1;
                ImageStatus.OFFLine = Ysite + 1;  //截止行重新规划
                break;
            }
        }
        for (int Ysite = 57; Ysite > Repair_Point_Ysite-3; Ysite--)         //补线   //出环补线,这个是上边界的58列和58行的左边线斜拉补线，不应该是和圆环的右下拐点进行拉线吗？？？
        {
            ImageDeal[Ysite].LeftBorder = (ImageDeal[58].LeftBorder - Repair_Point_Xsite) * (Ysite - 58) / (58 - Repair_Point_Ysite)  + ImageDeal[58].LeftBorder;//y=（y1-y2）/(x1-x2)*(x-x1)+y1
            ImageDeal[Ysite].Center = ImageDeal[Ysite].LeftBorder+Half_Road_Wide[Ysite];
        }
    }
        //已出环 半宽处理
    if (ImageFlag.image_element_rings_flag == 9)
    {
        for (int Ysite = 59; Ysite > ImageStatus.OFFLine; Ysite--)
        {
            ImageDeal[Ysite].Center = ImageDeal[Ysite].LeftBorder + Half_Road_Wide[Ysite];//防止出圆环时圆环内切，因此左圆环出环靠右行驶
        }
    }
}

//--------------------------------------------------------------
//  @name           Element_Judgment_Zebra()
//  @brief          整个图像判断的子函数，用来判断斑马线
//  @parameter      void
//  @time
//  @Author         MRCHEN
//  Sample usage:   Element_Judgment_Zebra();
//--------------------------------------------------------------
void Element_Judgment_Zebra()//斑马线判断
{
    if (ImageFlag.Zebra_Flag) return;
    int NUM = 0, net = 0;//net为斑马线的条数
    if(ImageStatus.OFFLineBoundary<20)
    {
        for (int Ysite = 20; Ysite < 33; Ysite++)
        {
            net = 0;
            for (int Xsite =ImageDeal[Ysite].LeftBoundary ; Xsite < ImageDeal[Ysite].RightBoundary; Xsite++)
            {
                if (Pixle[Ysite][Xsite] == 0 && Pixle[Ysite][Xsite + 1] == 1)
                {
                    net++;
                    if (net > 4)
                        NUM++;
                }
            }
        }
    }

    if (NUM >= 4 && ImageFlag.Zebra_Flag == 0)
    {       
            ImageFlag.Zebra_Flag = 1;
			Statu = Zebra;
    }

}

//--------------------------------------------------------------
//  @name           Element_Handle_Zebra()
//  @brief          整个图像处理的子函数，用来处理斑马线
//  @parameter      void
//  @time
//  @Author         MRCHEN
//  Sample usage:   Element_Handle_Zebra();
//--------------------------------------------------------------
int Zebra_flag=0;
int Zebra_Lenth=0;
void Element_Handle_Zebra()//斑马线处理
{
    if(Zebra_flag==0){Zebra_Lenth=SystemData.SpeedData.Length * OX;Zebra_flag=1;}
    if(Zebra_flag==1)
    {
			SystemData.Stop = 1;
    }

}
void get_border_line(void)
{

	for(Ysite=2;Ysite<59;Ysite++)
	{
		if(Pixle[Ysite][5]==1&&Pixle[Ysite-1][5]==0&&Pixle[Ysite+2][5]==1&&Pixle[Ysite-2][5]==0
			&&Pixle[Ysite+1][5]==1&&Pixle[Ysite-2][5]==0&&Pixle[Ysite-3][5]==0&&Pixle[Ysite+3][5]==0)
		{
			border_line_left=60-Ysite;
		}
		if(Pixle[Ysite][75]==1&&Pixle[Ysite-1][75]==0&&Pixle[Ysite-2][75]==0
			&&Pixle[Ysite+1][75]==1)
		{
			border_line_right=60-Ysite;
		}
	}
}


//void Element_obst()
//{
//	for(Ysite=20;Ysite<50;Ysite++)
//		{
//		if(ImageDeal[Ysite].LeftBorder-ImageDeal[Ysite+1].LeftBorder>5
//			&&ImageDeal[Ysite].LeftBorder-ImageDeal[Ysite+2].LeftBorder>5
//			&&ImageDeal[Ysite].LeftBorder-ImageDeal[Ysite+3].LeftBorder>5)
//		{
//			ImageFlag.left_obst_flag=1;
//		}
//		else 
//		{
//			ImageFlag.left_obst_flag=0;
//		}
//		if(ImageDeal[Ysite].RightBorder-ImageDeal[Ysite+1].RightBorder>5
//			&&ImageDeal[Ysite].RightBorder-ImageDeal[Ysite+2].RightBorder>5
//			&&ImageDeal[Ysite].RightBorder-ImageDeal[Ysite+3].RightBorder>5)
//		{
//			ImageFlag.right_obst_flag=1;
//		}
//		else 
//		{
//			ImageFlag.right_obst_flag=0;
//		}
//	}
//}
int cross_time;
extern uint8_t dir_cross_flag;
void Element_cross()
{
	if(ImageStatus.WhiteLine>20&&ImageFlag.cross_flag==0&&(ImageDeal[50].RightBorder-ImageDeal[50].LeftBorder)>60)          //z左右两边的无边行数量大于20时判断为十字
	{	
		ImageFlag.cross_flag=1;
	}  
	if(ImageFlag.cross_flag==1)
	{
		if(ImageDeal[50].RightBorder-ImageDeal[50].LeftBorder<60)
		{
			ImageFlag.cross_flag=2;
		}
	}
	if(ImageFlag.cross_flag==2)
	{
		cross_time++;
		if(cross_time>20){ImageFlag.cross_flag=0;cross_time=0;}
		if(ImageDeal[5].Center>50&&cross_time>10){ImageFlag.cross_flag=4;cross_time=0;}            //右边         //如果不行的话，递减3
		else if(ImageDeal[5].Center<25&&cross_time>10){ImageFlag.cross_flag=3;cross_time=0;}       //左边         //如果不行的话，递增3
//		else {ImageFlag.cross_dir=0;}                                 //无

		
//		if(ImageDeal[5].Center>45&&vision_error>0){ImageFlag.cross_dir=2;}            //右边         //如果不行的话，递减3
//		else if(ImageDeal[5].Center<35&&vision_error<0){ImageFlag.cross_dir=1;}       //左边         //如果不行的话，递增3
//		else {ImageFlag.cross_dir=0;}                                 //无
	} 
	
	
//	if((ImageFlag.cross_flag==3||ImageFlag.cross_flag==4 )&&ImageDeal[6].Center>36&&ImageDeal[6].Center<44)
//	{
////		if(ImageFlag.cross_flag==3){ImageFlag.cross_dir=1;}
////		if(ImageFlag.cross_flag==4){ImageFlag.cross_dir=2;}
////		if(ImageFlag.cross_flag==3){dir_cross_flag=1;}
////		if(ImageFlag.cross_flag==4){dir_cross_flag=2;}
//		if(ImageStatus.WhiteLine<10) 
//		{ImageFlag.cross_flag=5;}
//	}
	if(ImageFlag.cross_flag==5)
	{
		if(ImageStatus.WhiteLine>30)
		{
			ImageFlag.cross_flag=6;
		}
	}
	if(ImageFlag.cross_flag==6)
	{
		if(ImageStatus.WhiteLine<10)
		{
			ImageFlag.cross_flag=7;
		}
	}
	if(ImageFlag.cross_flag==7)
	{
		if(ImageDeal[50].RightBorder-ImageDeal[50].LeftBorder<50)
		{
			ImageFlag.cross_flag=0;
		}
	}
}


/****元素检测*****/  //圆环 十字 车库 需要通过编码器积分自己跳出元素标志  而三叉只在一瞬间
                     //所以没检测到特征就直接此处跳出
void Element_Test(void) {
  if (       ImageFlag.RoadBlock_Flag == 0
          && ImageFlag.Zebra_Flag == 0
          && ImageFlag.image_element_rings == 0
          && ImageFlag.Bend_Road == 0
          && ImageFlag.straight_long==0
//		  && ImageFlag.Card_Flag==0
		  && ImageFlag.cross_flag==0
      )
      {Statu = Normal; }//赛道类型清0；初始化函数。除了上述发生的条件之外，每一帧图像都会进行元素清零，再去重新判断一下

  if (
          ImageFlag.RoadBlock_Flag == 0
          && ImageFlag.Zebra_Flag == 0
          && ImageFlag.image_element_rings == 0
          && ImageFlag.Bend_Road == 0
		  && ImageFlag.cross_flag==0
      )
	{ Straightacc_Test();}     //如果不是上述的情况，那么进行用于加速的直道检测。在车体回正后进行直线加速！！！

  if (
          ImageFlag.RoadBlock_Flag == 0
          && ImageFlag.Zebra_Flag == 0
          && ImageFlag.Bend_Road == 0
		  && ImageFlag.cross_flag==0
      )
  {
      Element_Judgment_Left_Rings();      //左圆环             //圆环不计角度里程
      Element_Judgment_Right_Rings();     //右圆环              //圆环不计角度里程
  }
  if (       ImageFlag.RoadBlock_Flag == 0
          && ImageFlag.image_element_rings == 0
          && ImageFlag.Bend_Road == 0
          && ImageFlag.cross_flag==0
  
      )
      {Element_Judgment_Zebra();}
	  
	  
	  if(ImageFlag.RoadBlock_Flag == 0
          && ImageFlag.Zebra_Flag == 0
		&& ImageFlag.image_element_rings == 0
          && ImageFlag.Bend_Road == 0
		  )
	  {Element_cross();}
	  
//	  if(ImageFlag.RoadBlock_Flag == 0
//          && ImageFlag.Zebra_Flag == 0
//          && ImageFlag.Bend_Road == 0
//		  && ImageFlag.cross_flag==0
//		  )
//	  Element_obst();
}

void Element_Handle() {
     if (ImageFlag.image_element_rings == 1)
        Element_Handle_Left_Rings();
    else if (ImageFlag.image_element_rings == 2)
        Element_Handle_Right_Rings();
     if(ImageFlag.straight_long==1){
    ; }
     if(ImageFlag.straight_long==0){;}

     if(ImageFlag.Zebra_Flag == 1){Element_Handle_Zebra();}
}


/*****************误差按权重重新整定**********************/
void GetDet() {
  float DetTemp = 0;
  int TowPoint = 0;//前瞻（偏差的基础行）；速度越快前瞻越靠上（数值小），速度越满前瞻越靠下（数值大）；因为速度快，前瞻靠上，转向越超前。
  float UnitAll = 0;


    if (ImageFlag.image_element_rings != 0)
    TowPoint = 29;                                                                      //圆环前瞻
 //   else if (ImageFlag.straight_long!=0) {                                       //直道前瞻
//    TowPoint = SystemData.straighet_towpoint;}


else
    TowPoint = ImageStatus.TowPoint;                                          //速度越快前瞻越长   ImageStatus.TowPoint初始化设定为26，需要重点去调参

  if (TowPoint < ImageStatus.OFFLine)
    TowPoint = ImageStatus.OFFLine + 1;

  if (TowPoint >= 49)
    TowPoint = 49;

  if ((TowPoint - 5) >= ImageStatus.OFFLine) {                                          //前瞻取设定前瞻还是可视距离  需要分情况讨论，根据图像真实情况获得真实的前瞻
    for (int Ysite = (TowPoint - 5); Ysite < TowPoint; Ysite++) {
      DetTemp = DetTemp + Weighting[TowPoint - Ysite - 1] * (ImageDeal[Ysite].Center);
      UnitAll = UnitAll + Weighting[TowPoint - Ysite - 1];
    }
    for (Ysite = (TowPoint + 5); Ysite > TowPoint; Ysite--) {
      DetTemp += Weighting[-TowPoint + Ysite - 1] * (ImageDeal[Ysite].Center);
      UnitAll += Weighting[-TowPoint + Ysite - 1];
    }
    DetTemp = (ImageDeal[TowPoint].Center + DetTemp) / (UnitAll + 1);                   //当前瞻与图像顶边距离大于5时：取前瞻上下5行，共10行。按照正态分布比例系数计算赛道中心点坐标

  } else if (TowPoint > ImageStatus.OFFLine) {
    for (Ysite = ImageStatus.OFFLine; Ysite < TowPoint; Ysite++) {
      DetTemp += Weighting[TowPoint - Ysite - 1] * (ImageDeal[Ysite].Center);
      UnitAll += Weighting[TowPoint - Ysite - 1];
    }
    for (Ysite = (TowPoint + TowPoint - ImageStatus.OFFLine); Ysite > TowPoint;
         Ysite--) {
      DetTemp += Weighting[-TowPoint + Ysite - 1] * (ImageDeal[Ysite].Center);
      UnitAll += Weighting[-TowPoint + Ysite - 1];
    }
    DetTemp = (ImageDeal[Ysite].Center + DetTemp) / (UnitAll + 1);                       //当前瞻与图像顶边距离小于5时：取前瞻与顶边之间的几行，和前瞻下面相同数量的行。按照正态分布比例系数计算赛道中心点坐标
  } else if (ImageStatus.OFFLine < 49) {
    for (Ysite = (ImageStatus.OFFLine + 3); Ysite > ImageStatus.OFFLine;
         Ysite--) {
      DetTemp += Weighting[-TowPoint + Ysite - 1] * (ImageDeal[Ysite].Center);
      UnitAll += Weighting[-TowPoint + Ysite - 1];
    }
    DetTemp = (ImageDeal[ImageStatus.OFFLine].Center + DetTemp) / (UnitAll + 1);        //根据图像顶边，再次整定前瞻对应的图像中心坐标

  } else
    DetTemp =ImageStatus.Det_True;                                                     //如果是出现OFFLine>50情况，保持上一次的偏差值

  ImageStatus.Det_True = DetTemp;                                                       //此时的解算出来的前瞻行的赛道中心坐标；这个数值与39做差然后将传递给舵机进行PID打角控制。取优化补线，完成元素最终目的都是为了得到这个值。在正确的时刻完成正确的打角。

  ImageStatus.TowPoint_True = TowPoint;                                                //此时的前瞻
  vision_error=ImageStatus.Det_True-ImageStatus.MiddleLine;
}




//图像处理函数
void ImageProcess(void) {
    compressimage();          //图像压缩 0.6ms。                     包含  mt9v03x_finish_flag = 0; 使用完一帧DMA传输的图像图像  可以开始传输下一帧，图像压缩之后才开始传输下一帧图像

    ImageStatus.OFFLine = 2;  //可视行（可视距离；截止行；在截止行以上的数据全部都舍弃了，行数越小说明看的越远）；这个值根据真实距离得到，必须进行限制
    ImageStatus.WhiteLine = 0;

  for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--) {
    ImageDeal[Ysite].IsLeftFind = 'F';
    ImageDeal[Ysite].IsRightFind = 'F';
    ImageDeal[Ysite].LeftBorder = 0;
    ImageDeal[Ysite].RightBorder = 79;
    ImageDeal[Ysite].LeftTemp = 0;
    ImageDeal[Ysite].RightTemp = 79;
    ImageDeal[Ysite].Black_Wide_L = 39;
    ImageDeal[Ysite].Black_Wide_R = 39;
    ImageDeal[Ysite].BlackWide = 0;
    // g  5.12
    ImageDeal[Ysite].close_LeftBorder = 0;
    ImageDeal[Ysite].close_RightBorder = 0;
    ImageDeal[Ysite].opp_LeftBorder = 0;
    ImageDeal[Ysite].opp_RightBorder = 0;
  }      //上面为边界与标志位初始化，每判断一幅图像都要重新清零一下

  Get01change_dajin();  //图像二值化    2.7ms       使用大津法将灰度图像变为黑白的二值化图像，只要二值化阈值取好，效果和二值化图像一样，根据光线的强弱动态的改变阈值，灰度图像点小于阈值为白点，大于阈值为黑点,一定要设置阈值下限和阳光算法强光的分离上限（把光线很强的点（比如反光）舍弃了，不再算在大津的范畴里否则会影响整幅图像分割效果）


{ Pixle_Filter();}       //腐蚀         1.7ms}    如果一个黑点周围有三个白点，那么就把这个黑点变成白点，因为这个黑点极有可能是个噪点。防止出现在道路中间的小噪点影响图像的质量
//上面为图像的预处理，使得图像的效果更好
//**********************************************************************************************************************************************************************

  DrawLinesFirst();     //绘制底边      30us    （底边五行（55~60）的信息提取，左右赛道边线和赛道中线坐标提取，赛道宽度，是否左右有边标志位判断）
//  DrawLinesProcess();   //得到基本边线  8us       /*边线追逐大致得到全部边线*/
	
	distance_line();
	get_border_line();
		       //得到转弯的标志位开启
	
	DrawLinesProcess(); 
	//得到基本边线  8us       /*边线追逐大致得到全部边线*/
//上面两个函数运行完之后二值化图像基础的边就已经出来了，已经采集到了赛道的初始信息，但是后续还要优化，而在元素识别部分使用的是现在采集到的赛道初始信息而并非优化后的。因为没有优化的赛道信息会比较符合真实的赛道图像，元素识别起来会比较准确
  //**********************************************************************************************************************************************************************
  if(!ImageFlag.RoadBlock_Flag)
      Search_Border_OTSU(Pixle, LCDH, LCDW, LCDH - 2);//58行位底行，这里用了边界追踪和上交的八邻域来互补爬出边界
  else
      ImageStatus.OFFLineBoundary = 5;
//上面为上交迷宫次扫线
//**********************************************************************************************************************************************************************

//  /***元素识别*****/
  Element_Test();                   //5us
//  /***元素识别*****/

  DrawExtensionLine();//得到延长线   8us//绘制延长线并重新确定中线 ，把补线补成斜线。主要优化十字的处理，注释掉也是能跑的。

  RouteFilter();        //中线滤波平滑 2us  让中线更平滑一些，让原始偏差平滑一些，舵机的控制也会丝滑很多。 //把毛毛糙糙的无边行原本斜率连接更加优化一下。出现丢边的时候  重新确定无边行的中线，将原本根据DrawLinesProcess（）这个函数内通过有边行延长线确定无边行左右边线进而再确定中线的方法跟改为：寻找两个拐点并将其相连作为边界，进而求出中线


//  /***元素处理*****/
  Element_Handle();  // 3us
//  /***元素处理*****/
  
 //**********************************************************************************************************************************************************************
     GetDet();             //偏差的基础行（控制行）就是前瞻；可以没有这些动态前瞻计算，取固定控制行（固定前瞻也是可以跑的），但是取动态前瞻上限更高/  GetDet();            //获取动态前瞻  并且计算图像偏差 3us  
						  //误差按权重重新整定。//根据图像真实情况获得真实的前瞻，然后去取他的偏差，
						  //这个偏差赋值给  ImageStatus.Det_True,这个数值将传递给舵机进行PID打角控制。
}









