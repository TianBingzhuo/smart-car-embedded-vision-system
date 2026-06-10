# Contribution Scope

This file explains what's in this public repo, what's deliberately left out, and how credit is assigned. The smart-car project is a team competition effort — this repo is just the publicly shareable engineering archive, not the whole story.

本文件用于说明当前公开仓库里的贡献边界、材料边界和署名原则。智能车项目是团队竞赛成果，本仓库只是把其中可公开复盘的工程材料整理成一个可读档案。

## Project Scope

This repository focuses on:

- RT1064 application-level firmware and project metadata useful for understanding the embedded control chain.
- OpenART-side scripts and deployment files that are part of the visual-recognition workflow.
- IMU tuning notes, wireless/debug notes, and field bring-up records.
- PCB schematic/board screenshots and EasyEDA exports from the competition-stage iteration.
- Rule-derived documentation that explains the 19th Vision Group task context.

The repo is a public engineering snapshot — not a complete reproducible vendor SDK package, and definitely not something I'd call production-ready. It's here so people can see what the work actually looked like.

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

What's in here:

- Public-safe source files and notes that explain the engineering chain.
- PCB and schematic snapshots useful for showing electrical iteration.
- Debugging and bring-up notes after removing private or oversized raw package context.

What's not in here:

- Full raw competition package.
- Private certificates, teammate-private files, or unreleased media.
- Binary build products, local IDE state, board-vendor SDK packages, third-party model/vendor packages, and unreviewed external materials.
- A claim that the PCB snapshots are final production-ready boards.

## PCB Note

The PCB files are a record of the competition-stage iteration. They're handy for reviewing what design decisions we made and how the hardware came together, but there's still follow-up work that could be done — connector labeling, board-review notes, schematic cleanup, manufacturing checks. It's a snapshot of where things stood, not the final word.

---

# 中文版本

# 贡献范围

这个文件想说明三件事：这个公开仓库里有什么、刻意没放什么、以及功劳怎么算。智能车项目是团队竞赛成果——这个仓库只是其中可以公开分享的工程档案，不是全貌。

## 项目范围

本仓库关注以下内容：

- RT1064 应用层固件和工程元数据，便于理解嵌入式控制链路。
- OpenART 端脚本和部署文件，属于视觉识别工作流程的一部分。
- IMU 调参笔记、无线/调试笔记和现场调车记录。
- 竞赛阶段迭代的 PCB 原理图/板卡截图和嘉立创 EDA 导出。
- 规则衍生的文档，用于说明第十九届视觉组的任务背景。

这个仓库是一份公开的工程快照——不是完整的可复现厂商 SDK 包，更算不上什么量产级成品。放在这里是希望大家能看到我们的工作实际是什么样子的。

## 团队贡献说明

| 成员 | 主要贡献 |
|---|---|
| 戴哲维 | 主程序编写，RT1064 整车控制固件相关工作。 |
| 葛洪飞 | 主程序编写，RT1064 整车控制固件相关工作。 |
| 黄得时 | 机械结构制作与相关调试。 |
| 么林坤 | 视觉部分，包括 OpenART 识别相关工作。 |
| 田秉卓 | 电路板设计、焊接、板级迭代记录，以及本公开仓库维护。 |

## 公开仓库边界

包含的内容：

- 可公开分享的源文件和笔记，用于说明工程链路。
- PCB 和原理图快照，便于了解电路迭代过程。
- 移除了私人或超大体积的原始资料包之后保留的调试和调车笔记。

不包含的内容：

- 完整竞赛原始资料包。
- 私人证书、队友私人文件或未公开的媒体资料。
- 二进制编译产物、本地 IDE 状态、开发板厂商 SDK 包、第三方模型/厂商包和未经审核的外部材料。
- 不声称 PCB 快照是最终量产级成品。

## PCB 说明

PCB 文件是竞赛阶段迭代的记录。它们对回顾我们做了哪些设计决策、硬件是怎么搭起来的挺有参考价值，但后续还有可以做的事——连接器标注、板级复盘笔记、原理图清理、可制造性检查。这是一个阶段性的快照，不是最终定论。
