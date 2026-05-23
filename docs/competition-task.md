# 19th Vision Group Task Context

This repository records a project from the 19th National Undergraduate Smart Car Competition, Vision Group.

本文件用于说明第十九届智能车视觉组的任务背景，帮助读者理解本仓库中的 RT1064 固件、OpenART 脚本、IMU 调参、PCB 文件和调车记录。它不是官方规则替代文本；正式参赛要求仍以竞赛组委会发布的规则和现场说明为准。

Referenced local materials:

- `第十九届视觉组准备流程及思路介绍-逐飞科技.pdf`
- `19th培训大纲.xlsx`
- `智能视觉组.xlsx`
- `视觉组文件/规则类/` and related visual-group preparation notes

## Task Summary

The 19th Vision Group task is a mixed autonomous-driving and visual-handling task. A smart car starts from the starting area, uses the track as a ground-guidance map, searches for target boards near the track, detects and classifies them, and transports them to the required placement areas.

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

The preparation material breaks the car into five core parts:

1. Vehicle motion control.
2. Track following.
3. Target-board detection.
4. Target-board classification.
5. Target-board handling.

The hardware chain used for this project follows that structure:

```text
grayscale track camera / OpenART vision modules
  -> RT1064 main control firmware
  -> mission state and target-board logic
  -> motor, steering, servo, and electromagnet control
  -> encoder and IMU feedback
  -> field debugging and repeated bring-up
```

## Vision And Handling Strategy

The referenced 19th Vision Group preparation document describes two broad kinds of target-board situations:

- scattered target boards near the sides of the track;
- stacked target boards near ring and cross-loop centers.

This leads to two connected control problems. The car first needs to find a target board and approach it. Then it needs to classify the board and decide how to store or release it. The preparation material discusses several handling strategies, from simple one-by-one transport to multi-slot classification boxes. The repository code reflects this competition pressure through card/box logic, servo motor control, OpenART serial modes, and repeated field debugging notes.

## How This Maps To The Repository

| Repository part | Competition role |
|---|---|
| `firmware/rt1064/project/code/` | Main embedded control modules: camera input, mission state, motion control, encoders, motor/servo control, filtering, IMU, UART, Wi-Fi/debug interfaces, and card/box logic. |
| `firmware/openart/` | OpenART-side scripts and label files for target detection and recognition communication. |
| `hardware/pcb/` | Competition-stage custom board snapshots and EasyEDA exports, including board iterations used to support the vehicle. |
| `hardware/imu/` | IMU660RA configuration and tuning notes for attitude and motion feedback. |
| `docs/bringup-log.md` | Field debugging timeline showing PID, path tracking, OpenART integration, ring logic, servo logic, and hardware-board work. |
| `docs/openart-vision.md` | Notes on OpenART command modes, deployment, labels, and model-side workflow. |

There are no standalone desktop simulation scripts in this repository. The Python files kept here are OpenART firmware-side or deployment scripts, because those are part of the original visual-recognition chain.
