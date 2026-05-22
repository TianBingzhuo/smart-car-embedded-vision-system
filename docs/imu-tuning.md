# IMU Tuning Notes

The project includes IMU660RA-related configuration and field tuning notes. These records are useful because they show the vehicle was not only assembled, but repeatedly tuned for stable sensing and control feedback.

## Public Files

- `hardware/imu/imu660ra_config_array.txt`: configuration array note.
- `hardware/imu/imu-tuning-notes.zh-CN.txt`: original short tuning record.

## Recorded Stable Parameters

The tuning note records a stable attitude-filter setting:

```text
alpha = 0.75
Kp = 12.8
Ki = 0.001
halfT = 0.6564 to 0.658
dt = 0.005
```

Observed behavior:

- Static angle reading was stable with little visible drift.
- Slow left/right shaking produced relatively small error.
- Fast motion increased error to roughly plus/minus 2 degrees.

## Resume-Relevant Takeaway

This supports a concrete embedded-control claim: IMU integration required parameter tuning, field observation, and stability tradeoff checks, not just API-level sensor use.
