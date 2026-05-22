import pyb
import sensor, image, time, math
import os, tf
from machine import UART, Pin
from machine import PWM
from pyb import LED
from machine import Pin
import seekfree, pyb
from seekfree import Timer
import ustruct
#import openmv_numpy as np
uart = UART(2, baudrate=115200)     # 初始化串口 波特率设置为115200 TX是B12 RX是B13
blue = LED(3)  # 定义一个LED4   照明灯
green = LED(2)  # 定义一个LED4   照明灯
write = LED(4)

cards = ["A-firearms",#1
"B-explosive",#2
"C-dagger",#3
"D-spontoon",#4
"E-fire_axe",#5
"F-first_aid",#6
"G-flashlight",#7
"H-intercom",#8
"I-bulletproof",#9
"J-telescope",#10
"K-helmet",#11
"L-fire_engine",#12
"M-ambulannce",#13
"N-armoured",#14
"O-motorcycle"]#15

cards2 = ["1",#1
"2",#2
"3",#3
"A",#4
"B",#5
"C",#6
"D",#7
"E",#8
"F",#9
"G",#10
"H",#11
"I",#12
"J",#13
"K",#14
"L",#15
"M",#16
"N",#17
"O"]#18

cards3 = ["1",#1
"2",#2
"3"]#3

world_positions = [[35,170],[275,170],[275,0],[35,0]]
thresholds_xy = (0, 58, -87, 127,-128, 127)  #(0, 58, -87, 127,-128, 127)
text_x = 0
text_y = 0

net_path = "mobilenet_v2-2024-04-22T02-49-00.046Z_in-int8_out-int8_channel_ptq.tflite"                                  # 定义模型的路径
labels = [line.rstrip() for line in open("/sd/labels_animal_fruits_traffic.txt")]   # 加载标签
net = tf.load(net_path, load_to_fb=True)                                  # 加载模型

net_path2 = "mobilenet_v2-2024-05-17T13-50-15.890Z_in-int8_out-int8_channel_ptq.tflite"                                  # 定义模型的路径
labels2 = [line.rstrip() for line in open("/sd/labels.txt")]   # 加载标签
net2 = tf.load(net_path2, load_to_fb=True)                                  # 加载模型

net_path3 = "mobilenet_v2-2024-05-18T06-27-52.328Z_in-int8_out-int8_channel_ptq.tflite"                                  # 定义模型的路径
labels3 = [line.rstrip() for line in open("/sd/labels2.txt")]   # 加载标签
net3 = tf.load(net_path3, load_to_fb=True)                                  # 加载模型

send_x_error = 0
send_y_error = 0

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
#thresholdfind = (0, 4, -8, 14, -12, 4)

def get_card_content(card_content):
    xuhao = 1
    for i in cards:
        if card_content==i:
            return xuhao
        xuhao = xuhao + 1
def get_card_content2(card_content2):
    xuhao = 1
    for i in cards2:
        if card_content2==i:
            return xuhao
        xuhao = xuhao + 1

def get_card_content3(card_content3):
    xuhao = 1
    for i in cards3:
        if card_content3==i:
            return xuhao
        xuhao = xuhao + 1

def recognition_picture():
    sensor.reset()  # 感光元件初始化
    sensor.set_pixformat(sensor.RGB565)  # 设置像素模式 彩色
    sensor.set_framesize(sensor.QVGA)  # 图像大小
    sensor.set_brightness(1500)  # 设置图像亮度 越大越亮
    sensor.set_auto_gain(False)  # 自动增益
    sensor.set_auto_whitebal(False)  # 白平衡
    #比赛用的
    sensor.set_auto_exposure(False, 50)#曝光，越低越暗
    #sensor.set_auto_exposure(False, 1000)
    sensor.skip_frames(time=1000) # 跳过n张照片 待稳定


def data_process(card):
    sending_data(1,card,0,0)

def sending_data(one,two,three,four):      #发送函数四位
    uart_array = bytearray([0x7a,int(two),0x77])
    uart.write(uart_array)

def GetDataQuery():                        #获得串口发送函数
    global uart_str
    uart_num = uart.any()
    if uart_num:
        uart_str = uart.read(uart_num)

clock = time.clock()
recognition_picture()
lcd = seekfree.LCD180(3)
lcd.full()  # 将背景颜色显示到整个屏幕
#write.on()
while(True):
    clock.tick()
    #blue.on()
    #write.off()
    #write.on()
    img = sensor.snapshot()
    GetDataQuery()
    #print(uart_str)
    img.draw_rectangle((35,0,240,170),(255,255,255))
    #if uart_str == b'\x08':
    if sb == 1:
        write.off()
        blue.on()
        sensor.set_auto_exposure(False, 1000)
        #if map_flag == 0:
        ok = 12
        if ok == 12:
            map_flag=1
        #show = False

    #else:
    if uart_str == b'\x01':
        blue.off()
        write.on()
        if map_flag == 1:
            #比赛用的
            sensor.set_auto_exposure(False, 50)
            #sensor.set_auto_exposure(False, 1000)
            map_flag = 0
        blobs = img.find_blobs([thresholdfind],invert=1,roi=(0, 0, 320, 240),merge=False)
        for blob in blobs:
            #print("\n色块面积:", blob.area())
            #img.draw_rectangle(blob.rec0t(), color=(0,0,255))
            if blob.area() > 8000 and blob.area() < 20000 and abs(blob.w()-blob.h())<10:
                #print(blob.w(),blob.h())
                nx = blob.cx()
                ny = blob.cy()
                #print(find_ny)
                if find_ny<ny:
                    find_ny=ny
                    ZhengShu = 0
                if find_ny==ny:
                    y_flag = 1
                    ZhengShu = 0
                    find_ny = 0
                else:
                    find_ny = 0
                if y_flag==1:
                    img.draw_rectangle([nx-50,ny-50,100,100], color=(0,255,0))
                    #img.draw_rectangle(blob.rect(), color=(0,255,0))
                    y_flag = 0
                    send_card = 0
                    findimg=img.copy(1, 1,[nx-50,ny-50,100,100])

                    for obj in tf.classify(net , findimg, min_scale=1.0, scale_mul=0.9, x_overlap=0.0, y_overlap=0.0, scale=1, offset=0):
                        sorted_list = sorted(zip(labels, obj.output()), key = lambda x: x[1], reverse = True)
                    for i in range(1):
                        data_process(get_card_content(sorted_list[i][0]))
                        img.draw_string(0, 220,str(sorted_list[i][0]),(255,255,255))
                        print(get_card_content(sorted_list[i][0]))
                        #print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))

                    #save_img_num += 1;   #采集图片
                    #image_pat = "/num"+str(save_img_num) + str(sorted_list[0][0]) + str(sorted_list[0][1])+".jpg"
                    #img1.save(image_pat,quality=99)
                else:
                    img.draw_rectangle(blob.rect(), color=(255,0,0))

    #else:
    if uart_str == b'\x02':
        blue.off()
        write.on()
        if map_flag == 1:
            #比赛用的
            sensor.set_auto_exposure(False, 50)
            #sensor.set_auto_exposure(False, 1000)
            map_flag = 0
        blobs = img.find_blobs([thresholdfind],invert=1,roi=(0, 0, 320, 240),merge=False)
        for blob in blobs:
            #print("\n色块面积:", blob.area())
            #img.draw_rectangle(blob.rec0t(), color=(0,0,255))
            if blob.area() > 5000 and blob.area() < 20000 and abs(blob.w()-blob.h())<10:
                #print(blob.w(),blob.h())
                nx = blob.cx()
                ny = blob.cy()
                #print(find_ny)
                if find_ny<ny:
                    find_ny=ny
                    ZhengShu = 0
                if find_ny==ny:
                    y_flag = 1
                    ZhengShu = 0
                    find_ny = 0
                else:
                    find_ny = 0
                if y_flag==1:
                    img.draw_rectangle([nx-50,ny-50,100,100], color=(0,255,0))
                    #img.draw_rectangle(blob.rect(), color=(0,255,0))
                    y_flag = 0
                    send_card = 0
                    findimg=img.copy(1, 1,[nx-50,ny-50,100,100])

                    for obj in tf.classify(net2 , findimg, min_scale=1.0, scale_mul=0.9, x_overlap=0.0, y_overlap=0.0, scale=1, offset=0):
                        sorted_list = sorted(zip(labels2, obj.output()), key = lambda x: x[1], reverse = True)
                    for i in range(1):
                        data_process(get_card_content2(sorted_list[i][0]))
                        img.draw_string(0, 220,str(sorted_list[i][0]),(255,255,255))
                        print(get_card_content2(sorted_list[i][0]))
                        #print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))

                    #save_img_num += 1;   #采集图片
                    #image_pat = "/num"+str(save_img_num) + str(sorted_list[0][0]) + str(sorted_list[0][1])+".jpg"
                    #img1.save(image_pat,quality=99)
                else:
                    img.draw_rectangle(blob.rect(), color=(255,0,0))


    #else:
    if uart_str == b'\x03':
        blue.off()
        write.on()
        if map_flag == 1:
            #比赛用的
            sensor.set_auto_exposure(False, 50)
            #sensor.set_auto_exposure(False, 1000)
            map_flag = 0
        blobs = img.find_blobs([thresholdfind],invert=1,roi=(0, 0, 320, 240),merge=False)
        for blob in blobs:
            #print("\n色块面积:", blob.area())
            #img.draw_rectangle(blob.rec0t(), color=(0,0,255))
            if blob.area() > 5000 and blob.area() < 20000 and abs(blob.w()-blob.h())<10:
                #print(blob.w(),blob.h())
                nx = blob.cx()
                ny = blob.cy()
                #print(find_ny)
                if find_ny<ny:
                    find_ny=ny
                    ZhengShu = 0
                if find_ny==ny:
                    y_flag = 1
                    ZhengShu = 0
                    find_ny = 0
                else:
                    find_ny = 0
                if y_flag==1:
                    img.draw_rectangle([nx-50,ny-50,100,100], color=(0,255,0))
                    #img.draw_rectangle(blob.rect(), color=(0,255,0))
                    y_flag = 0
                    send_card = 0
                    findimg=img.copy(1, 1,[nx-50,ny-50,100,100])

                    for obj in tf.classify(net3 , findimg, min_scale=1.0, scale_mul=0.9, x_overlap=0.0, y_overlap=0.0, scale=1, offset=0):
                        sorted_list = sorted(zip(labels3, obj.output()), key = lambda x: x[1], reverse = True)
                    for i in range(1):
                        data_process(get_card_content3(sorted_list[i][0]))
                        img.draw_string(0, 220,str(sorted_list[i][0]),(255,255,255))
                        print(get_card_content3(sorted_list[i][0]))
                        #print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))

                    #save_img_num += 1;   #采集图片
                    #image_pat = "/num"+str(save_img_num) + str(sorted_list[0][0]) + str(sorted_list[0][1])+".jpg"
                    #img1.save(image_pat,quality=99)
                else:
                    img.draw_rectangle(blob.rect(), color=(255,0,0))


    img.draw_string(0, 0,str(int(clock.fps())),(255,255,255))
    lcd.show_image(img, 320, 240, zoom=2)
