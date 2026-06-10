# Smart Car Embedded Vision System

This repository holds the code and debugging records from our smart-car competition project. The car was built around an RT1064 board, with an OpenART module handling visual recognition, IMU and encoder feedback for motion control, and a custom PCB that I designed and soldered by hand. What you see here is a competition-stage snapshot: firmware, tuning notes, PCB exports, and bring-up logs. It is not a one-click tutorial project; rebuilding it on real hardware requires the matching SDK and toolchain.

Competition context: 19th National Undergraduate Smart Car Competition, Vision Group, regional second prize.

## Project At A Glance

| Item | Summary |
|---|---|
| Competition | 19th National Undergraduate Smart Car Competition, Vision Group. |
| System type | RT1064 embedded vehicle-control firmware with OpenART visual recognition, IMU/encoder feedback, actuator control, and PCB iteration. |
| Repository focus | Public-safe firmware snapshot, OpenART scripts, PCB images/exports, IMU notes, and bring-up notes. |
| My role | PCB design, soldering, board iteration notes, and public repository maintenance. |
| Not included | Full raw competition package, private media/certificates, unreleased teammate materials, binary build outputs, board-vendor SDK bundles, and unreviewed third-party files. |

## Competition Task Context

The Vision Group task is more involved than just making a car follow a line. The car starts from the starting area, drives along the track (which acts as a ground-guidance map), searches for target boards scattered or stacked near the track, detects and classifies them visually, and then physically transports them to designated placement areas. It's a full vehicle-level system challenge — you need to get the RT1064 main controller, the track camera, the OpenART recognition module, four-mecanum-wheel motion, encoder/IMU feedback, servo and electromagnet actuators, and a custom PCB all working together. In retrospect, getting these pieces to cooperate was the hardest part.

See [docs/competition-task.md](docs/competition-task.md) for the rule-derived task explanation and repository mapping.

## Visual Materials

These PCB images come from the actual project materials and show the boards as they existed during the competition iteration.

![Main-board PCB snapshot](hardware/pcb/PCB_main-board-v2_2026-05-22.png)

![Driver-board PCB snapshot](hardware/pcb/PCB_driver-board_2026-05-22.png)

## Project Overview

The core control flow looks like this:

```text
camera / vision input
  -> OpenART recognition / target detection
  -> RT1064 decision logic
  -> PD/PID, wheel-speed, servo, and handling commands
  -> motor driver and actuator output
  -> IMU feedback and field debugging
```

## Repository Layout

| Area | Path | Purpose |
|---|---|---|
| RT1064 firmware | `firmware/rt1064/project/code/` | Application-level modules for camera input, mission logic, motor control, steering, encoders, filters, IMU, UART, and wireless communication. |
| MDK project files | `firmware/rt1064/project/mdk/` | Minimal Keil/MDK project metadata for understanding the embedded build setup. |
| OpenART scripts | `firmware/openart/` | Upper/lower OpenART scripts, serial command modes, labels, and deployment helpers. |
| IMU notes | `hardware/imu/` | IMU660RA configuration and tuning notes. |
| PCB files | `hardware/pcb/` | Schematic/PCB screenshots and EasyEDA project exports from the competition iteration. |
| Competition context | `docs/competition-task.md` | 19th Vision Group task, constraints, and project mapping. |
| Contribution scope | `docs/contribution-scope.md` | Team contribution split and public repository boundary notes. |
| Bring-up notes | `docs/raw-notes/bringup-log.zh-CN.txt` | Field notes from vehicle debugging. |

## How To Read The Firmware Snapshot

If you're trying to understand the code, start with these modules:

- `box_logic_code.c`: high-level mission and box/logic handling.
- `camera.c`: camera processing and visual input handling.
- `car_move.c`: vehicle movement, chassis-level command conversion, and motion modes.
- `motor.c`, `encoder.c`, `smotor.c`, `steer.c`: actuator and feedback control.
- `filter.c`, `Fuzzy.c`, `location.c`, `tracking.c`: filtering, steering assistance, localization, and path tracking.
- `u660.c`: IMU interface and attitude-related integration.
- `wifi.c`, `wir_usart.c`, `wirles_usart.c`: wireless and serial communication.

What we kept here is the application-level code that helps explain the control chain. To rebuild and flash the firmware, you need to set up the matching board SDK, startup files, compiler settings, and local toolchain configuration for the RT1064 development board you are using.

The only Python files in this repo are OpenART-side scripts or deployment helpers — no standalone Python examples.

## Documents

- `docs/competition-task.md`: 19th Vision Group task context and project mapping.
- `docs/contribution-scope.md`: team contribution split and public repository boundaries.
- `docs/system-architecture.md`: system-chain diagram.
- `docs/hardware.md`: hardware and PCB overview.
- `docs/openart-vision.md`: OpenART command modes and deployment notes.
- `docs/imu-tuning.md`: IMU tuning summary.
- `docs/bringup-log.md`: vehicle bring-up milestone log.

## PCB Status

The PCB files are snapshots from the competition-stage iteration. They are useful for understanding how the electrical design evolved over time, but they are not presented as production-ready boards. Interface labeling, schematic cleanup, board-review notes, and manufacturing checks can still be improved later.

## Team And Copyright

This was a team effort. The competition result and engineering work happened because of everyone on the team; no single person completed the project alone. Every teammate's contribution matters and deserves proper credit. I maintain this repository as a readable record of the code, hardware notes, and debugging process from the competition stage.

Known contribution split:

| Member | Main contribution |
|---|---|
| 戴哲维 | Main program / RT1064 vehicle-control firmware. |
| 葛洪飞 | Main program / RT1064 vehicle-control firmware. |
| 黄得时 | Mechanical structure. |
| 么林坤 | Vision part, including OpenART-related recognition work. |
| 田秉卓 | PCB design, soldering, board iteration notes, and public repository maintenance. |

This project came out of a team competition, so the results aren't attributable to any one person. The table above reflects the best understanding of who did what; if we need more precise attribution, code ownership details, or licensing clarifications later, that should follow the team's shared agreement.

Unless otherwise stated, code and documents authored for this repository use the Apache License 2.0 where the contributors have the right to license them. SDKs, model files, libraries, board packages, and vendor materials should follow their own licenses.

---

# 中文版本

## 智能车嵌入式视觉控制系统

这个仓库是我们智能车竞赛项目留下来的代码和调试记录。车主控用的是 RT1064 开发板，OpenART 模块负责视觉识别，IMU 和编码器做运动反馈，PCB 是我自己设计并手工焊接的。这里保留的就是竞赛期间的真实状态——固件、调参笔记、PCB 导出文件、调车日志——不是重新包装过的一键教程项目。要真正跑在硬件上，仍然需要补齐对应的 SDK 和工具链。

竞赛背景：第十九届全国大学生智能汽车竞赛，视觉组，赛区二等奖。

## 项目概况

| 项目 | 说明 |
|---|---|
| 竞赛 | 第十九届全国大学生智能汽车竞赛，视觉组。 |
| 系统类型 | RT1064 嵌入式整车控制固件，配合 OpenART 视觉识别、IMU/编码器反馈、执行机构控制和 PCB 迭代。 |
| 仓库定位 | 可公开分享的固件快照、OpenART 脚本、PCB 图片/导出、IMU 笔记和调车记录。 |
| 我的角色 | 电路板设计、焊接、板级迭代记录，以及本公开仓库的维护。 |
| 未包含内容 | 完整竞赛原始资料包、私人媒体/证书、未公开的队友材料、二进制编译产物、开发板厂商 SDK 包，以及未经审核的第三方文件。 |

## 竞赛任务背景

视觉组的任务比单纯让车沿着赛道跑要复杂得多。小车从出发区出发，沿赛道行驶（赛道同时充当引导地图），在赛道附近搜索散落或堆叠的目标板，通过视觉检测和分类，然后把它们搬运到指定的放置区域。这是一个整车级别的系统工程——RT1064 主控、赛道摄像头、OpenART 识别模块、四麦轮运动、编码器/IMU 反馈、舵机和电磁铁执行机构、自制 PCB，全都要配合调通。回过头看，最难的部分其实是让这些模块真正协同工作。

详细的任务说明和仓库对应关系见 [docs/competition-task.md](docs/competition-task.md)。

## 视觉素材

这些就是项目里 PCB 的真实照片，没做什么特别处理，就是竞赛迭代时板子实际的样子。

![主板 PCB 快照](hardware/pcb/PCB_main-board-v2_2026-05-22.png)

![驱动板 PCB 快照](hardware/pcb/PCB_driver-board_2026-05-22.png)

## 项目总览

核心控制流程如下：

```text
camera / vision input
  -> OpenART recognition / target detection
  -> RT1064 decision logic
  -> PD/PID, wheel-speed, servo, and handling commands
  -> motor driver and actuator output
  -> IMU feedback and field debugging
```

## 仓库结构

| 区域 | 路径 | 用途 |
|---|---|---|
| RT1064 固件 | `firmware/rt1064/project/code/` | 应用层模块：摄像头输入、任务逻辑、电机控制、转向、编码器、滤波、IMU、串口和无线通信。 |
| MDK 工程文件 | `firmware/rt1064/project/mdk/` | 精简的 Keil/MDK 工程元数据，便于了解嵌入式构建方式。 |
| OpenART 脚本 | `firmware/openart/` | 上/下位 OpenART 脚本、串口命令模式、标签文件和部署辅助。 |
| IMU 笔记 | `hardware/imu/` | IMU660RA 配置和调参记录。 |
| PCB 文件 | `hardware/pcb/` | 竞赛迭代阶段的原理图/PCB 截图和嘉立创 EDA 工程导出。 |
| 竞赛背景 | `docs/competition-task.md` | 第十九届视觉组任务、约束条件和项目对应关系。 |
| 贡献范围 | `docs/contribution-scope.md` | 团队分工说明和公开仓库的材料边界。 |
| 调车记录 | `docs/raw-notes/bringup-log.zh-CN.txt` | 实车调试的现场笔记。 |

## 固件阅读指南

如果想理解代码，建议从这几个模块入手：

- `box_logic_code.c`：高层任务状态和搬运逻辑。
- `camera.c`：摄像头处理和视觉输入。
- `car_move.c`：车辆运动、底盘级指令转换和运动模式。
- `motor.c`、`encoder.c`、`smotor.c`、`steer.c`：执行机构和反馈控制。
- `filter.c`、`Fuzzy.c`、`location.c`、`tracking.c`：滤波、转向辅助、定位和路径跟踪。
- `u660.c`：IMU 接口和姿态相关集成。
- `wifi.c`、`wir_usart.c`、`wirles_usart.c`：无线和串口通信。

这里保留的是方便阅读和复盘的应用层代码。要真正编译上板，得根据你用的 RT1064 开发板补齐对应的 SDK、启动文件、编译器设置和本地工具链配置；它不是一键即可复现的完整构建环境。

仓库里仅有的 Python 文件都是 OpenART 端的脚本或部署辅助，没有独立的 Python 示例。

## 文档索引

- `docs/competition-task.md`：第十九届视觉组任务背景与项目对应关系。
- `docs/contribution-scope.md`：团队分工说明和公开仓库的材料边界。
- `docs/system-architecture.md`：系统链路图。
- `docs/hardware.md`：硬件与 PCB 概览。
- `docs/openart-vision.md`：OpenART 命令模式与部署笔记。
- `docs/imu-tuning.md`：IMU 调参总结。
- `docs/bringup-log.md`：整车调试里程碑记录。

## PCB 状态

PCB 文件是竞赛阶段的设计快照。它能看出电路设计是怎么一步步迭代过来的，但我不会说它已经是量产级别的成品。接口标注、原理图说明、板级复盘笔记和可制造性检查，这些都还有优化空间，后续如果有人想继续完善的话可以接着做。

## 团队与版权

这个项目是团队合作完成的，我想把这点说清楚。竞赛成绩和所有工程工作是因为团队里每个人的付出才实现的——没有谁是独自完成的。每位队友的贡献都很重要，都应该得到应有的认可。我维护这个仓库，是想把竞赛阶段的代码、硬件笔记和调试过程整理成一份可读的记录。

已知分工如下：

| 成员 | 主要贡献 |
|---|---|
| 戴哲维 | 主程序编写，RT1064 整车控制固件相关工作。 |
| 葛洪飞 | 主程序编写，RT1064 整车控制固件相关工作。 |
| 黄得时 | 机械结构制作与相关调试。 |
| 么林坤 | 视觉部分，包括 OpenART 识别相关工作。 |
| 田秉卓 | 电路板设计、焊接、板级迭代记录，以及本公开仓库维护。 |

本项目来自团队竞赛，成果不是单人完成。上表用于说明当前已知主要分工；如后续需要补充更精确的成员署名、代码归属或授权说明，应以团队共识为准。

除另有说明外，本仓库中由贡献者编写的代码和文档在贡献者有权授权的范围内采用 Apache License 2.0 许可。SDK、模型文件、库文件、开发板包和厂商资料请遵循其各自的许可协议。
