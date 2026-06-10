# IMU Tuning Notes

We used an IMU660RA on the car, and it wasn't just a plug-and-play situation. These notes came from actual field tuning sessions where we were trying to get stable attitude readings under different motion conditions. The records here show that the IMU integration took real parameter tuning and observation, not just calling a sensor API.

## Public Files

- `hardware/imu/imu660ra_config_array.txt`: configuration array note.
- `hardware/imu/imu-tuning-notes.zh-CN.txt`: original short tuning record.

## Recorded Stable Parameters

Here's the attitude-filter setting that ended up working well for us:

```text
alpha = 0.75
Kp = 12.8
Ki = 0.001
halfT = 0.6564 to 0.658
dt = 0.005
```

What we observed in practice:

- Static angle reading was stable with little visible drift.
- Slow left/right shaking produced relatively small error.
- Fast motion increased error to roughly plus/minus 2 degrees.

Honestly, getting to these numbers took more trial and error than I expected. The "fast motion" case was always the hardest to keep under control.

## Engineering Takeaway

The point of keeping these notes is pretty simple: IMU integration on a moving vehicle required actual parameter tuning, field observation, and tradeoff checks between static accuracy and dynamic response. It wasn't something you could just configure once and forget about.

---

# 中文版本

# IMU 调参笔记

车上用的是 IMU660RA，这东西不是接上就能用的。这些笔记来自我们在实车调试时反复尝试的过程——目标是在不同运动条件下拿到稳定的姿态数据。这些记录说明 IMU 的集成确实需要认真调参和观察，不是调一下传感器 API 就完事了。

## 公开文件

- `hardware/imu/imu660ra_config_array.txt`：配置数组说明。
- `hardware/imu/imu-tuning-notes.zh-CN.txt`：原始简短调参记录。

## 记录下来的稳定参数

这是我们最终调出来效果比较好的一组姿态滤波参数：

```text
alpha = 0.75
Kp = 12.8
Ki = 0.001
halfT = 0.6564 to 0.658
dt = 0.005
```

实际观察到的表现：

- 静态角度读数稳定，基本没有明显零漂。
- 缓慢左右摇晃时误差比较小。
- 快速运动时误差会增大到大约正负 2 度。

说实话，调到这些参数比预想的要折腾。快速运动的情况一直是最难控制住的。

## 工程收获

保留这些笔记的意义很简单：在运动的小车上做 IMU 集成，确实需要反复调参、现场观察，以及在静态精度和动态响应之间做取舍。不是配置一次就能不管的东西。
