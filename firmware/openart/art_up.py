


# IO接线方法
# 屏         openart
# GND       ---> GND
# VCC       ---> 3.3V
# SCL       ---> B0(SCLK)
# SDA/MISO  ---> B1(MISO)
# RESET     ---> B12
# DC        ---> B13
# CS        ---> B3
# BL        ---> B16 (背光控制)

from pyb import LED
import seekfree, pyb
import sensor, image, time, tf, gc
from machine import UART
uart = UART(2, baudrate=115200)     # 初始化串口 波特率设置为115200
blue = LED(3)
sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
sensor.set_brightness(1500)  # 设置图像亮度 越大越亮
#sensor.skip_frames(time=1500)  # 跳过n张照片 待稳定
sensor.set_auto_gain(False)  # 自动增益
sensor.set_auto_whitebal(False)  # 白平衡
#sensor.set_auto_exposure(False, 50)#曝光，越低越暗
#sensor.set_auto_exposure(False, 2000)#曝光，越低越暗
clock = time.clock()                # Create a clock object to track the FPS.
# 3：屏幕显示方向 参数范围0-3
#sensor.set_auto_exposure(False, 500)
ZhenShu = 0
y_flag = 0
find_ny = 0
nx=0
ny=0

map_flag = 0
show = True
uart_str = 0
save_img_num = 0
sb = 1
points_num = 0
map_flag = 0
#比赛用的
thresholdfind = (0, 13, -7, 17, -27, 1)

def data_process(send_x,send_y):
    send_fuhao = 0
    if send_x < 0:
        send_fuhao = 10
    if send_y < 0:
        send_fuhao = send_fuhao + 1
    #print(1,send_fuhao,send_x,send_y)
    sending_data(1,send_fuhao,abs(send_x),abs(send_y))

def sending_data(one,two,three,four):      #发送函数四位
    uart_array = bytearray([0x7a,int(three),int(four),0x77])
    uart.write(uart_array)

def GetDataQuery():                        #获得串口发送函数
    global uart_str
    uart_num = uart.any()
    if uart_num:
        uart_str = uart.read(uart_num)




lcd = seekfree.LCD180(3)
lcd.full()  # 将背景颜色显示到整个屏幕

#设置模型路径
face_detect = '/sd/new.tflite'
#载入模型
net = tf.load(face_detect)

while(True):
    clock.tick()
    img = sensor.snapshot()
    #lcd.show_image(img, 320, 240, 0, 0, zoom=2)
    #lcd.show_str('+',80, 50, lcd.RED, 1)
    GetDataQuery()
    #print(uart_str)
    #img.draw_rectangle((35,0,240,240),(255,255,255))
    #if uart_str == b'\x08':
    if sb == 1:
    #if uart_str == b'\x04':
        #write.off()
       # blue.off()
        #sensor.set_auto_exposure(False, 1000)
        #if map_flag == 0:
        ok = 12
        if ok == 12:
            map_flag=1
            img.draw_rectangle((35,0,240,240),(255,255,255))
        #show = False
        #lcd.show_image(img, 320, 240, 0, 0, zoom=2)
    #else:
    if uart_str == b'\x04':
    #if sb == 1:
        #blue.on()
        #write.on()


        #使用模型进行识别
        for obj in tf.detect(net,img):
            x1,y1,x2,y2,label,scores = obj

            if(scores>0.70):
                #print(obj)
                #lcd.show_image(img, 320, 240, 10, 0, zoom=2)
                w = x2- x1
                h = y2 - y1
                x1 = int((x1-0.1)*img.width())
                y1 = int(y1*img.height())
                w = int(w*img.width())
                h = int(h*img.height())
                img.draw_rectangle((x1,y1,w,h),thickness=2)
                img.draw_rectangle((35,0,240,240),(255,255,255))
                errorx=((x2+x1)/2)*120/100-5
                errory=((y2+y1)/2)*60/100
                errorx = round(errorx)  # 四舍五入
                #errorx = int(errorx)  # 转换为整数
                errory = round(errory)  # 四舍五入
                #errory = int(errory)  # 转换为整数
                #print(errorx)
                #print(errory)
                data_process(errorx,errory)
                #lcd.show_image(img, 320, 240, 0, 0, zoom=2)
                #lcd.show_image(img, 320, 240, 0, 0, zoom=2)
            else:
                #img.draw_rectangle((35,0,240,240),(255,255,255))
                blue.off()
                img.draw_rectangle((35,0,240,240),(255,255,255))
                #lcd.show_image(img, 320, 240, 0, 0, zoom=2)
    #lcd.show_str('+',80, 50, lcd.RED, 1)
    img.draw_string(0, 0,str(int(clock.fps())),(255,255,255))
    lcd.show_image(img, 320, 240, zoom=2)
                #uart_array1 = [errorx]  # 定义一个列表 保存数字
            #uart_array2 = [errory]  # 定义一个列表 保存数字
            ##uart.write(bytearray(uart_array))   # 发送列表

            #uart.write(bytearray([0x7a]))
            #uart.write(bytearray(uart_array1))   # 发送列表
            #uart.write(bytearray(uart_array2))   # 发送列表
            ##uart.write(errorx)
            ##uart.write(errory)
            #uart.write(bytearray([0x77]))
 #               img.draw_string(120,30,str(errorx),2000,2)
  #              img.draw_string(120,60,str(errory),2000,2)
               # lcd.show_image(img, 320, 240, 0, 0, zoom=2)
            ##lcd.show_string(str(errorx),160,200,lcd.red,2)
            ##lcd.show_string(str(errory),180,200,lcd.red,2)
    #lcd.show_image(img, 300, 200, 0, 0, zoom=2)
    #print(clock.fps())
