# Contribution Scope

本文件用于说明当前公开仓库里的贡献边界、材料边界和署名原则。智能车项目是团队竞赛成果，本仓库只是把其中可公开复盘的工程材料整理成一个可读档案。

## Project Scope

This repository focuses on:

- RT1064 application-level firmware and project metadata useful for understanding the embedded control chain.
- OpenART-side scripts and deployment files that are part of the visual-recognition workflow.
- IMU tuning notes, wireless/debug notes, and field bring-up records.
- PCB schematic/board screenshots and EasyEDA exports from the competition-stage iteration.
- Rule-derived documentation that explains the 19th Vision Group task context.

The repository is a public engineering snapshot. It is not a complete reproducible vendor SDK package and is not claimed to be production-ready.

## Team Contribution Notes

| Member | Main contribution |
|---|---|
| 戴哲维 | Main program / RT1064 vehicle-control firmware. |
| 葛洪飞 | Main program / RT1064 vehicle-control firmware. |
| 黄得时 | Mechanical structure. |
| 么林坤 | Vision part, including OpenART-related recognition work. |
| 田秉卓 | PCB design, soldering, board iteration notes, and public repository maintenance. |

中文分工说明：

| 成员 | 主要贡献 |
|---|---|
| 戴哲维 | 主程序编写，RT1064 整车控制固件相关工作。 |
| 葛洪飞 | 主程序编写，RT1064 整车控制固件相关工作。 |
| 黄得时 | 机械结构制作与相关调试。 |
| 么林坤 | 视觉部分，包括 OpenART 识别相关工作。 |
| 田秉卓 | 电路板设计、焊接、板级迭代记录，以及本公开仓库维护。 |

## Public Repository Boundaries

Included:

- Public-safe source files and notes that explain the engineering chain.
- PCB and schematic snapshots useful for showing electrical iteration.
- Debugging and bring-up notes after removing private or oversized raw package context.

Not included:

- Full raw competition package.
- Private certificates, teammate-private files, or unreleased media.
- Binary build products, local IDE state, board-vendor SDK packages, third-party model/vendor packages, and unreviewed external materials.
- A claim that the PCB snapshots are final production-ready boards.

## PCB Note

The PCB files record competition-stage iteration. They are useful for reviewing design decisions and hardware integration, but follow-up work can still add connector labeling, board-review notes, schematic cleanup, and manufacturing checks.
