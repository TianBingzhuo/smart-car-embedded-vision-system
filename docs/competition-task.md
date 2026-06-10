# 19th Vision Group Task Context

This repository comes from a project in the 19th National Undergraduate Smart Car Competition, Vision Group. This file tries to explain what the competition was actually asking us to do, so the RT1064 firmware, OpenART scripts, IMU tuning notes, PCB files, and debugging logs in this repo make a bit more sense. It's not a replacement for the official rules — for that, you'd want to check whatever the competition committee actually published.

本文件用于说明第十九届智能车视觉组的任务背景，帮助读者理解本仓库中的 RT1064 固件、OpenART 脚本、IMU 调参、PCB 文件和调车记录。它不是官方规则替代文本；正式参赛要求仍以竞赛组委会发布的规则和现场说明为准。

Referenced local materials (not included in this public repository — they are either vendor-provided PDFs, internal preparation spreadsheets, or contain third-party content that cannot be redistributed):

- `第十九届视觉组准备流程及思路介绍-逐飞科技.pdf`
- `19th培训大纲.xlsx`
- `智能视觉组.xlsx`
- `视觉组文件/规则类/` and related visual-group preparation notes

## Task Summary

The 19th Vision Group task is a mix of autonomous driving and visual handling. The car starts from the starting area, drives along the track (which doubles as a guidance map), looks for target boards scattered or stacked near the track, detects and classifies them, and then physically moves them to the correct placement areas. It sounds straightforward when you list it like that, but getting all of it to work on a real vehicle is a different story.

第十九届视觉组不是单纯循迹竞速。车辆需要完成一条更长的工程链路：

- vehicle motion control: make the four-mecanum-wheel chassis move smoothly and repeatably;
- line/track following: use the track as a ground-guidance map to find useful areas;
- target-board detection: locate scattered boards and board stacks near ring/cross-loop areas;
- classification: identify the target-board category through the vision module;
- handling: pick, store, move, and release target boards to the correct placement zones;
- debugging: tune PID, IMU feedback, serial communication, vision output, and mechanical timing together.

## Rule-Derived Design Points

| Rule or preparation point | Engineering implication |
|---|---|
| Vehicle model is a four-mecanum-wheel smart car. | The chassis needs four-motor control, wheel-speed feedback, and omnidirectional movement planning for target handling. |
| The control platform uses an NXP MCU. | The project centers on RT1064 firmware and embedded peripheral integration. |
| The car starts from the starting zone and follows the track as guidance. | A track camera and line/edge extraction provide the basic route reference. |
| Target boards may be scattered beside the track or stacked near ring/cross-loop centers. | The strategy needs both search/detection and short-range approach logic. |
| Boards must be classified and transported to corresponding areas. | OpenART recognition output must be mapped into mission states and placement decisions. |
| Competition-stage boards/drivers should be self-made rather than only using learning boards/modules. | PCB design, soldering, power distribution, connectors, and driver-board reliability are part of the engineering result. |
| Wireless serial and Wi-Fi image transmission are useful for debugging but must be handled carefully for official runs. | Debug telemetry belongs in bring-up notes; official vehicle configuration should remove or disable non-compliant aids. |

## System Parts

The preparation material breaks the car into five core parts, which is honestly a pretty sensible way to think about it:

1. Vehicle motion control.
2. Track following.
3. Target-board detection.
4. Target-board classification.
5. Target-board handling.

The hardware chain we ended up using follows that same structure:

```text
grayscale track camera / OpenART vision modules
  -> RT1064 main control firmware
  -> mission state and target-board logic
  -> motor, steering, servo, and electromagnet control
  -> encoder and IMU feedback
  -> field debugging and repeated bring-up
```

## Vision And Handling Strategy

The 19th Vision Group preparation document describes two broad kinds of target-board situations:

- scattered target boards near the sides of the track;
- stacked target boards near ring and cross-loop centers.

That gives you two connected problems to solve. First, the car needs to find a target board and get close to it. Then it needs to figure out what kind of board it is and decide how to pick it up, store it, or release it. The preparation material talks about several handling strategies, from just carrying boards one at a time to building multi-slot classification boxes. The code in this repo reflects that competition pressure — you'll see card/box logic, servo motor control, OpenART serial modes, and a lot of field debugging notes that came from trying to make all of this work under time pressure.

## How This Maps To The Repository

| Repository part | Competition role |
|---|---|
| `firmware/rt1064/project/code/` | Main embedded control modules: camera input, mission state, motion control, encoders, motor/servo control, filtering, IMU, UART, Wi-Fi/debug interfaces, and card/box logic. |
| `firmware/openart/` | OpenART-side scripts and label files for target detection and recognition communication. |
| `hardware/pcb/` | Competition-stage custom board snapshots and EasyEDA exports, including board iterations used to support the vehicle. |
| `hardware/imu/` | IMU660RA configuration and tuning notes for attitude and motion feedback. |
| `docs/bringup-log.md` | Field debugging timeline showing PID, path tracking, OpenART integration, ring logic, servo logic, and hardware-board work. |
| `docs/openart-vision.md` | Notes on OpenART command modes, deployment, labels, and model-side workflow. |

There are no standalone desktop simulation scripts in this repo. The Python files here are all OpenART firmware-side or deployment scripts, because those were part of the actual visual-recognition chain on the car.

---

# 中文版本

# 第十九届视觉组任务背景

这个仓库来自第十九届全国大学生智能汽车竞赛视觉组的一个参赛项目。这个文件想把竞赛到底要求我们做什么讲清楚，这样再看仓库里的 RT1064 固件、OpenART 脚本、IMU 调参笔记、PCB 文件和调车记录的时候，会更容易理解上下文。这不是官方规则的替代品——正式参赛要求还是以竞赛组委会发布的规则和现场说明为准。

参考的本地资料（未包含在本公开仓库中——它们是厂商提供的 PDF、内部备赛表格，或包含无法再分发的第三方内容）：

- `第十九届视觉组准备流程及思路介绍-逐飞科技.pdf`
- `19th培训大纲.xlsx`
- `智能视觉组.xlsx`
- `视觉组文件/规则类/` and related visual-group preparation notes

## 任务概述

第十九届视觉组的任务是自主行驶和视觉操作的结合。小车从出发区出发，沿赛道行驶（赛道同时充当引导地图），在赛道附近寻找散落或堆叠的目标板，进行检测和分类，然后把它们搬运到对应的放置区域。列出来好像挺清晰的，但真正要在实车上全跑通，完全是另一回事。

第十九届视觉组不是单纯循迹竞速。车辆需要完成一条更长的工程链路：

- 车辆运动控制：让四麦轮底盘平稳、可重复地运动；
- 赛道循迹：把赛道当引导地图用，找到有用的区域；
- 目标板检测：在圆环/十字交叉区域附近定位散落的目标板和目标板堆；
- 分类：通过视觉模块识别目标板类别；
- 搬运：拣取、暂存、搬运和释放目标板到正确的放置区；
- 调试：PID、IMU 反馈、串口通信、视觉输出和机械时序都得一起调。

## 规则衍生的设计要点

| 规则或备赛要点 | 工程含义 |
|---|---|
| 车模为四麦轮智能车。 | 底盘需要四电机控制、轮速反馈和全向运动规划来配合目标搬运。 |
| 控制平台使用 NXP MCU。 | 项目以 RT1064 固件和嵌入式外设集成为核心。 |
| 车辆从出发区出发，以赛道为引导。 | 赛道摄像头和线/边缘提取提供基本路线参考。 |
| 目标板可能散落在赛道旁或堆叠在圆环/十字中心附近。 | 策略上既要搜索/检测，也要有近距逼近逻辑。 |
| 目标板须分类并搬运到对应区域。 | OpenART 的识别输出必须映射到任务状态和放置决策中。 |
| 竞赛阶段的板子/驱动应自制，不能只用学习板/模块。 | PCB 设计、焊接、配电、连接器和驱动板可靠性都是工程成果的一部分。 |
| 无线串口和 Wi-Fi 图传方便调试，但正式运行需谨慎处理。 | 调试遥测属于调车记录；正式上车配置应移除或禁用不合规的辅助手段。 |

## 系统组成

备赛资料把小车拆成五个核心部分，说实话这种拆分方式挺合理的：

1. 车辆运动控制。
2. 赛道循迹。
3. 目标板检测。
4. 目标板分类。
5. 目标板搬运。

我们实际用到的硬件链路也是按这个结构来的：

```text
grayscale track camera / OpenART vision modules
  -> RT1064 main control firmware
  -> mission state and target-board logic
  -> motor, steering, servo, and electromagnet control
  -> encoder and IMU feedback
  -> field debugging and repeated bring-up
```

## 视觉与搬运策略

第十九届视觉组备赛文档描述了两大类目标板场景：

- 赛道两侧散落的目标板；
- 圆环和十字交叉中心附近堆叠的目标板。

这就带来两个关联的控制问题。首先小车得找到目标板并靠近它，然后得识别出是什么类型的板子，再决定怎么拣取、暂存或释放。备赛资料讨论了好几种搬运策略，从简单的一次搬一块到多格分类箱都有。仓库里的代码就是在这样的竞赛压力下写出来的——你会看到卡片/箱子逻辑、舵机控制、OpenART 串口模式，还有大量现场调试笔记，都是当时赶着让这一切跑通留下来的。

## 仓库对应关系

| 仓库部分 | 竞赛角色 |
|---|---|
| `firmware/rt1064/project/code/` | 主要嵌入式控制模块：摄像头输入、任务状态、运动控制、编码器、电机/舵机控制、滤波、IMU、串口、Wi-Fi/调试接口，以及卡片/箱子逻辑。 |
| `firmware/openart/` | OpenART 端脚本和标签文件，用于目标检测和识别通信。 |
| `hardware/pcb/` | 竞赛阶段自制板快照和嘉立创 EDA 导出，包括用于支撑整车的板级迭代。 |
| `hardware/imu/` | IMU660RA 配置和调参笔记，用于姿态和运动反馈。 |
| `docs/bringup-log.md` | 现场调试时间线，涵盖 PID、路径跟踪、OpenART 集成、圆环逻辑、舵机逻辑和硬件板工作。 |
| `docs/openart-vision.md` | OpenART 命令模式、部署、标签和模型端工作流程的笔记。 |

仓库里没有独立的桌面仿真脚本。这里保留的 Python 文件都是 OpenART 固件端或部署脚本，因为它们本来就是实车视觉识别链路的一部分。
