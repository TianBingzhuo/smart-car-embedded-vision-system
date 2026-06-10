# Vehicle Bring-Up Log

This page is a summary of the Chinese field log sitting in `docs/raw-notes/bringup-log.zh-CN.txt`. I kept the original notes pretty raw because that's what actual debugging looks like — not everything made sense at the time, and the log reflects that.

## Timeline

| Date | Milestone |
|---|---|
| Feb 28 | Speed-loop PID debugging was still failing; hardware board issues were suspected. |
| Feb 29 | Basic movement modes were completed and PID tuning started. Path tracking became the next blocker. |
| Mar 2 | Path tracking reached an initial working state. Remaining tasks included ring logic, obstacle avoidance, OpenART data, structure modification, and servo integration. |
| Mar 10 | Wi-Fi wireless tuning/control and path tracking were working. OpenART on-vehicle testing, structural changes, hardware board work, and servo work remained. |
| Mar 19 | Added servo code, zebra-crossing recognition and stop logic, and improved path tracking. |
| Apr 6 | Added ring tracking. Most components were mounted and usable except OpenART and some structure work. |
| Apr 21 | Added OpenART logic, card-detection code, filtering code, improved ring tracking, and simplified the main function. |

## Engineering Pattern

Looking back at the log, you can see the typical embedded-robotics loop playing out over and over:

```text
single module works
  -> vehicle integration exposes new instability
  -> tune PID / sensor / structure
  -> add perception or mission logic
  -> simplify main control flow
```

I think this log is worth keeping around because it shows what repeated field debugging actually looks like — the back and forth, the stuff that didn't work — instead of just showing the polished final code.

---

# 中文版本

# 整车调试日志

这个页面是 `docs/raw-notes/bringup-log.zh-CN.txt` 里那份中文现场笔记的摘要。原始记录写得比较粗糙，但调试过程本来就是这样的——当时很多事情还没想明白，日志也如实反映了那个状态。

## 时间线

| 日期 | 里程碑 |
|---|---|
| 2月28日 | 速度环 PID 调试仍然失败，怀疑是硬件板子的问题。 |
| 2月29日 | 基本运动模式完成，开始初调 PID。路径跟踪成为下一个卡点。 |
| 3月2日 | 路径跟踪初步跑通。剩余任务包括圆环逻辑、避障、OpenART 数据对接、结构改装和舵机集成。 |
| 3月10日 | Wi-Fi 无线调参/控制和路径跟踪已调好。OpenART 上车实测、结构改装、硬件板子和舵机还没搞定。 |
| 3月19日 | 加入了舵机代码、斑马线识别与停车逻辑，改善了循迹。 |
| 4月6日 | 加入圆环循迹。除 OpenART 和部分结构外，所有部件均已上车可用。 |
| 4月21日 | 加入 OpenART 逻辑、卡片检测代码、滤波代码，改善了圆环循迹，并简化了主函数。 |

## 工程节奏

回头看这份日志，能看到嵌入式机器人开发里很典型的一个循环在反复上演：

```text
single module works
  -> vehicle integration exposes new instability
  -> tune PID / sensor / structure
  -> add perception or mission logic
  -> simplify main control flow
```

我觉得这份日志值得留下来，因为它记录的是反复现场调试的真实过程——来来回回、各种不顺利——而不是只展示最后打磨好的代码。
