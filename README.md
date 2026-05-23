# Smart Car Embedded Vision System

智能车嵌入式视觉控制系统

This repository records an embedded smart-car competition project built around RT1064 firmware, OpenART visual recognition, IMU feedback, motor/steering control, PCB iteration, and repeated vehicle bring-up.

本仓库保存的是智能车竞赛阶段的代码和调试记录，重点包括 RT1064 主控、OpenART 视觉识别、IMU 调参、PCB 迭代、车辆运动控制和现场调车过程。它更像一个工程快照和复盘记录，不是可以直接复制到任意电脑上编译烧录的完整环境。

## Project Overview

The core control flow is:

```text
camera / vision input
  -> OpenART recognition / target detection
  -> RT1064 decision logic
  -> PID, steering, and motor commands
  -> motor driver and actuator output
  -> IMU feedback and field debugging
```

Competition context: 19th National Undergraduate Smart Car Competition, vision group, regional second prize.

竞赛背景：第十九届全国大学生智能汽车竞赛，视觉组，赛区二等奖。

## Repository Layout

| Area | Path | Purpose |
|---|---|---|
| RT1064 firmware | `firmware/rt1064/project/code/` | Application-level modules for camera input, mission logic, motor control, steering, encoders, filters, IMU, UART, and wireless communication. |
| MDK project files | `firmware/rt1064/project/mdk/` | Minimal Keil/MDK project metadata for understanding the embedded build setup. |
| OpenART scripts | `firmware/openart/` | Upper/lower OpenART scripts, serial command modes, labels, and deployment helpers. |
| IMU notes | `hardware/imu/` | IMU660RA configuration and tuning notes. |
| PCB files | `hardware/pcb/` | Schematic/PCB screenshots and EasyEDA project exports from the competition iteration. |
| Bring-up notes | `docs/raw-notes/bringup-log.zh-CN.txt` | Field notes from vehicle debugging. |
| Desktop demos | `src/` | Small Python scripts for PID response and line-center extraction. |

## How To Read The Firmware Snapshot

The most relevant application modules are:

- `box_logic_code.c`: high-level mission and box/logic handling.
- `camera.c`: camera processing and visual input handling.
- `car_move.c`: vehicle movement, chassis-level command conversion, and motion modes.
- `motor.c`, `encoder.c`, `smotor.c`, `steer.c`: actuator and feedback control.
- `filter.c`, `Fuzzy.c`, `location.c`, `tracking.c`: filtering, steering assistance, localization, and path tracking.
- `u660.c`: IMU interface and attitude-related integration.
- `wifi.c`, `wir_usart.c`, `wirles_usart.c`: wireless and serial communication.

The firmware directory keeps the application-level code and the project metadata that help explain the embedded control chain. To rebuild the firmware on hardware, recreate the matching board SDK, startup files, compiler settings, and local toolchain configuration for the actual RT1064 development board.

这里保留的是便于阅读和复盘的应用层代码。若要真正编译上板，需要按所用开发板和工具链补齐对应 SDK、启动文件和本地编译配置。

## Demos

Run:

```bash
python src/pid_demo.py
python src/line_tracker.py
```

These scripts are desktop-friendly examples for understanding two small parts of the system: PID response and line-center extraction. They are not a replacement for the full embedded firmware.

## Documents

- `docs/system-architecture.md`: system-chain diagram.
- `docs/hardware.md`: hardware and PCB overview.
- `docs/openart-vision.md`: OpenART command modes and deployment notes.
- `docs/imu-tuning.md`: IMU tuning summary.
- `docs/bringup-log.md`: vehicle bring-up milestone log.

## PCB Status

The PCB files are snapshots from the competition-stage iteration. They are useful for showing the electrical design process, but they are not claimed to be final production-ready boards. Further schematic cleanup, interface labeling, and board-review notes can be added later.

PCB部分是竞赛阶段的迭代记录，可以体现设计过程，但后续仍有优化空间，例如接口标注、原理图说明、板级复盘和可制造性检查。

## Team And Copyright

This project was completed in a team competition setting. The competition result and engineering work should be understood as collaborative work; every teammate's contribution deserves proper credit. This repository is maintained by Tian Bingzhuo as a readable record of the code, hardware notes, and debugging process from the competition stage.

本项目来自团队竞赛，成果不是单人完成。队友在软件、硬件、调试、结构、文档和现场测试等方面均有相应贡献。本仓库由田秉卓维护，用于保存和说明竞赛阶段的代码、硬件资料与调车记录；如后续需要补充更精确的成员署名或贡献说明，应以团队共识为准。

Unless otherwise stated, code and documents authored in this repository use the Apache License 2.0. SDKs, model files, libraries, and vendor materials should follow their own licenses.
