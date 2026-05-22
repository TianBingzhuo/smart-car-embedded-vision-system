# Vision Smart Car Control Pipeline

Clean public archive for a vision-group smart-car competition project built around RT1064 firmware, OpenART visual recognition, IMU tuning, PCB iteration, and repeated vehicle bring-up.

The raw competition package is not dumped here. This folder keeps the public-safe parts that help a reader understand the engineering chain without exposing private certificates, teammate information, unreleased media, binary build products, or third-party model/vendor packages.

## Summary

This project represents a sensing-control-actuation pipeline:

```text
camera / vision input
  -> OpenART recognition / target detection
  -> RT1064 decision logic
  -> PID, steering, and motor commands
  -> motor driver and actuator output
  -> IMU feedback and field debugging
```

Competition context: 19th National Undergraduate Smart Car Competition, vision group, regional second prize.

## What Is Published Here

| Area | Path | Purpose |
|---|---|---|
| RT1064 firmware snapshot | `firmware/rt1064/project/code/` | Final public snapshot of application-level control, camera, motor, location, UART, filter, and logic modules. |
| Toolchain project metadata | `firmware/rt1064/project/mdk/` | Minimal MDK project files needed to understand the embedded build setup. Generated objects and listings are excluded. |
| OpenART scripts | `firmware/openart/` | Upper/lower OpenART scripts, serial command modes, labels, and deployment helpers. Model weights are intentionally excluded. |
| IMU tuning notes | `hardware/imu/` | IMU660RA configuration notes and attitude-filter tuning records. |
| PCB and schematic exports | `hardware/pcb/` | Public PCB/schematic screenshots and EasyEDA project exports. |
| Bring-up notes | `docs/raw-notes/bringup-log.zh-CN.txt` | Original short field log for vehicle debugging milestones. |
| Public demos | `src/` | Small synthetic demos for PID and line-center detection, independent of private competition data. |

## How To Read The Firmware Snapshot

The most relevant application modules are:

- `box_logic_code.c`: high-level mission and box/logic handling.
- `camera.c`: camera processing and visual input handling.
- `car_move.c`: vehicle movement, chassis-level command conversion, and motion modes.
- `motor.c`, `encoder.c`, `smotor.c`, `steer.c`: actuator and feedback control.
- `filter.c`, `Fuzzy.c`, `location.c`, `tracking.c`: filtering, steering assistance, localization, and path tracking.
- `u660.c`: IMU interface and attitude-related integration.
- `wifi.c`, `wir_usart.c`, `wirles_usart.c`: wireless and serial communication.

The firmware snapshot is preserved as a readable competition archive, not as a turnkey SDK distribution. Third-party SDK/vendor libraries, local IAR configuration, and build artifacts are excluded from this repository.

## Public Demos

Run:

```bash
python src/pid_demo.py
python src/line_tracker.py
```

These scripts do not depend on the competition hardware. They provide small public-safe examples of the control ideas used in the real project.

## Documents

- `docs/system-architecture.md`: system-chain diagram.
- `docs/hardware.md`: hardware and PCB overview.
- `docs/openart-pipeline.md`: OpenART command modes and deployment boundary.
- `docs/imu-tuning.md`: IMU tuning summary.
- `docs/bringup-log.md`: cleaned milestone log.
- `docs/release-boundary.md`: what was removed from the public version and why.
- `docs/upstream-source.md`: source-release and license-review note for related upstream/team repositories.

## Resume Connection

This project supports a robotics-system-software narrative: sensing, embedded control, actuator output, PCB iteration, serial/wireless communication, field debugging, and the path toward ROS 2 topic/control abstractions.
