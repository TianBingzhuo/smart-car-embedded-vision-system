# October 2026 PCB review: original boards and an Astra candidate

This is a later review of the RT1064 mainboard v2 and dual DRV8701E driver-board snapshots in this repository. It does not change the competition hardware or firmware.

## Who did what

- **Original competition boards:** Tian Bingzhuo / 田秉卓 handled component selection, schematic/PCB design based on Seekfree reference circuits, soldering, board iteration and bring-up support. The complete vehicle and competition result were team work.
- **Astra A1:** Astra generated the independent KiCad candidate schematics and PCB layouts supplied for this comparison in October 2026. A1 is a later AI-generated design candidate, not my original competition design or a board already tested on the vehicle.
- **Review and explanatory models:** subsequent AI-assisted file inspection, tool execution and numerical cross-checks are recorded as later review work. They are not evidence that I independently operated every tool during the competition.

OpenAI has demonstrated **GPT-6 Astra performing PCB placement and routing in KiCad from an existing schematic**. See the [official GPT-6 Astra demonstration](https://openai.com/index/gpt-6-astra/), released on 3 September 2026. That is useful background for trying this comparison; the checks on our A1 candidate are recorded separately below.

## What was checked

The originals are the preserved `ProPrj_driver-board_2026-05-21.epro2` and `ProPrj_main-board-v2_2026-05-21.epro2` exports in `hardware/pcb/`. Their export filenames do not establish the design or manufacturing date, or that they match the final assembled competition revision. Source configurations have four and six copper layers respectively; actual manufacturing stackups were not confirmed.

| Input and check | Recorded result | Meaning |
|---|---|---|
| Original boards, JLCEDA native checks on 6 October | Schematic fatal/error counts were zero, with supplier-attribute warnings. Each PCB reported one schematic/PCB netlist mismatch. | The detailed ECO differences were not inspected. This is not a complete pass or proof of a particular physical short/open. The cloud input bytes were not frozen for that earlier run. |
| A1 driver, native KiCad 10.0.1 | ERC, active DRC rules, unconnected items and schematic parity passed. | A rule-check result for the frozen candidate, not electrical or manufacturing acceptance. |
| A1 mainboard, native KiCad 10.0.1 | ERC/copper/unconnected checks passed; C26 had one MPN field-parity warning. A separate copy passed after synchronizing that field. | The first result and field-only diagnostic copy remain distinct. |
| Imported copies of the originals | Default KiCad ERC/DRC did not pass, even after source-backed conversion repairs. | Conversion losses, library definitions and rule differences prevent direct scoring against the native A1 reports. |

No boards were manufactured, assembled or electrically tested during this review. The candidate source packages and internal review workspace are not included in this lightweight documentation update.

## Useful findings

**Decoupling.** Original driver U6 DVDD (pin 8) lacked the specified independent bypass connection. A1 added C14, 1 µF, as well as U15 bypass C16 and bridge-side C17–C20. The bridge supply before the diode and the driver VM node after it are different nodes; capacitance at one cannot simply be counted as local bypass at the other. The [TI DRV8701 datasheet](https://www.ti.com/lit/ds/symlink/drv8701.pdf), pin descriptions and layout section, provides the component-specific reference.

**Geometry has tradeoffs.** The sum of straight copper-segment centerlines on eight selected gate nets fell from about **211.0 mm to 88.2 mm** in A1. That is not a measurement of the complete gate/return loop, inductance or EMI. Original U6 CPH/CPL connections were shorter, while A1 mainboard U2 FB increased from about **7.27 mm to 22.09 mm**. Gate/source return paths, feedback references, switching-node coupling and layer transitions matter alongside line length. See [TI's motor-driver layout guide](https://www.ti.com/lit/an/slva959b/slva959b.pdf).

**Purchasing identity needs its own check.** Mainboard U2/U3 instance values say SP1N28STER while the associated library title says SCT2450; a similar pinout does not establish identical supplied parts. A1 U15 changed from HC125 to LV125A but inherited the old supplier number. This illustrates why a clean geometric DRC does not ensure the intended chip will be purchased.

**Operating limits remain open.** Both versions retain the IDRIVE-to-AVDD configuration, grounded SP/SN and unexported nFAULT. These choices limit particular current-regulation/diagnostic paths; they do not disable every protection. The user confirmed a 3S battery and saved firmware requests 17 kHz PWM, but actual motor parameters, stall current, measured PWM, wiring and BMS behavior remain unknown. Component identity, working-point and thermal budgets must be resolved before changing the protection strategy.

**Assembly matters.** The originals concentrate more components on the front, which is useful for hand soldering, probing and repair. A1 uses more back-side assembly and includes vias in several pads. Factory SMT changes the tradeoff, but does not remove requirements for process selection, stencil/pad treatment, component height, access and inspection. No current factory quotation or production approval is claimed.

## What the models establish

Independent solver examples and explicitly assumed passive, ideal motor-load and finite-source circuits were cross-checked against equations or independent numerical solutions. The complete TI DRV8701 functional-model baseline was not accepted because its separate test circuits had convergence problems. Neither those failures nor agreement in simplified models establish the performance of either physical board.

Actual gate/supply waveforms, switching losses, temperature, EMI and reliability remain unmeasured in this comparison. The original boards have practical design and assembly merits, and A1 offers useful local improvements, but this review does not produce an overall electrical-performance ranking.

---

# 2026 年 10 月 PCB 复盘：本人原版与 Astra 候选

本次复盘对象是本仓保留的 RT1064 主板第二版、DRV8701E 双路驱动板，以及后来用于比较的 Astra A1；竞赛原始硬件和固件没有因此被改写。

**原版由田秉卓参考逐飞电路进行选型、原理图/PCB 设计、焊接、迭代和板级联调支持；A1 原理图和 PCB 由 Astra 绘制，是后来的独立 KiCad 候选。** 整车与竞赛成绩属于团队。后续 AI 辅助检查和解释性建模也单独记为复盘工作，不回填为本人当年的独立技能或实测成果。

OpenAI 在 **2026 年 9 月 3 日的 GPT-6 Astra 官方演示**中展示过 Astra 在 KiCad 内基于既有原理图完成元件放置和铜线布线：[官方说明](https://openai.com/index/gpt-6-astra/)。这为本次尝试提供了背景；我们 A1 候选的具体检查与实测状态另记如下。

原件以 `hardware/pcb/` 的两份 `.epro2` 导出为基线。文件名日期不等于设计/生产日期，源工程的四层/六层配置也不证明最终实板叠层。检查结果分别保留：

- **原版嘉立创 EDA 原生检查（10 月 6 日）：**原理图 fatal/error 为零，有供应商属性警告；两块 PCB 各有一条网表不一致，尚未展开 ECO 差异，不能称完整通过或已确认实板短路/断路。
- **A1 原生 KiCad 10.0.1：**驱动板通过实际启用的 ERC/DRC、未连接及一致性检查；主板首次有 C26 MPN 字段一致性警告，单独字段诊断副本同步后通过。首次结果与诊断副本不混为一版。
- **原版转换副本：**默认 KiCad ERC/DRC 未通过，其中混有转换损失、库定义和规则差异，不能用数量与 A1 给个人水平排名。

最有用的设计发现是：原版驱动 U6 DVDD 缺独立旁路，A1 增加 C14 1 µF，并补了缓冲器与桥侧旁路；桥侧和二极管后的 VM 是不同节点。八条门极网铜段中心线合计约 211.0→88.2 mm，但这不是完整回路/寄生电感或 EMI 的改善比例；原版部分电荷泵连接更短，A1 主板 U2 FB 反而约 7.27→22.09 mm。具体理解应回到 [DRV8701 手册](https://www.ti.com/lit/ds/symlink/drv8701.pdf)与[电机驱动布局指南](https://www.ti.com/lit/an/slva959b/slva959b.pdf)。

两版仍需闭合实际供料身份、工况和保护/诊断取舍。SP1N28STER 与库标题 SCT2450 的身份关系未确认；A1 更换 U15 后继承旧采购编号也是真实问题。原版较集中正面的装配便于手焊/返修，A1 则要结合双面装配、盘中孔和工厂工艺评估，未取得制造放行。

本人确认了 3S，源码请求 17 kHz；实际电机、堵转电流、线束/BMS及两版寄生仍未知。已运行的无源/理想负载/有限源模型是在明确假设下作公式或独立数值复核；完整 TI 功能模型测试台未通过。它们没有验证两版实板的振铃、效率、温升、EMI或可靠性。本轮没有制板、上电或实际电气测试，A1 仍是设计候选；轻量文档更新不包含候选源包和内部评审工作区。
