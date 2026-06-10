# Hardware Overview

This project puts together a vision-based smart-car control stack with custom-designed boards and a bunch of sensing and actuation modules. The hardware side was my main responsibility in the team, so most of what's here reflects the PCB iteration and integration work I did during the competition stage.

## Main Hardware Chain

```text
camera / OpenART modules
  -> recognition result over serial
  -> RT1064 control board
  -> motor, servo, electromagnet, and actuator drivers
  -> vehicle motion
  -> IMU / encoder / field debugging feedback
```

## Hardware Files

- `hardware/pcb/`: schematic and PCB screenshots plus EasyEDA project exports.
- `hardware/rt1064-pin-map.txt`: RT1064 pin mapping note.
- `hardware/imu/`: IMU660RA configuration and tuning notes.

## PCB Status

The PCB and schematic files here are snapshots from the competition stage. They capture the interface decisions we made, how the board evolved through a few iterations, and some of the debugging history along the way. I (Tian Bingzhuo / 田秉卓) was responsible for the PCB design, soldering, and keeping track of board-level iteration notes in this team project.

These files are useful as evidence of how the electrical integration actually happened, but I'd be the first to admit they're not final production-ready boards. There's room for clearer connector labels, board-review notes, design-rule checks, and a proper write-up of what each board actually does on the car. That's a "someday" task.

---

# 中文版本

# 硬件概览

这个项目把基于视觉的智能车控制方案和我们自己设计的电路板、各种传感和执行模块搭在了一起。硬件这边主要是我在负责，所以这里的东西大部分反映的是竞赛阶段我做的 PCB 迭代和集成工作。

## 主要硬件链路

```text
camera / OpenART modules
  -> recognition result over serial
  -> RT1064 control board
  -> motor, servo, electromagnet, and actuator drivers
  -> vehicle motion
  -> IMU / encoder / field debugging feedback
```

## 硬件文件

- `hardware/pcb/`：原理图和 PCB 截图，加上嘉立创 EDA 工程导出。
- `hardware/rt1064-pin-map.txt`：RT1064 引脚映射说明。
- `hardware/imu/`：IMU660RA 配置和调参笔记。

## PCB 状态

这里的 PCB 和原理图文件是竞赛阶段的快照。它们记录了我们当时做的接口决策、板子是怎么一步步迭代的，还有一些调试过程中的痕迹。在这个团队项目里，我（田秉卓）负责 PCB 设计、焊接以及板级迭代记录。

这些文件作为电路集成过程的记录是有参考价值的，但我自己也清楚，它们算不上量产级成品。连接器标注可以更清楚、板级复盘笔记可以补上、设计规则检查可以做一遍、每块板在车上具体干什么也值得好好写一写。这些都是"以后有空再说"的事了。
