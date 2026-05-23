# Hardware Overview

This project combines a vision smart-car control stack with custom board iteration and multiple sensing/actuation modules.

## Main Hardware Chain

```text
camera / OpenART modules
  -> recognition result over serial
  -> RT1064 control board
  -> motor, servo, electromagnet, and actuator drivers
  -> vehicle motion
  -> IMU / encoder / field debugging feedback
```

## Hardware Files

- `hardware/pcb/`: schematic and PCB screenshots plus EasyEDA project exports.
- `hardware/rt1064-pin-map.txt`: RT1064 pin mapping note.
- `hardware/imu/`: IMU660RA configuration and tuning notes.

## PCB Status

The PCB/schematic files are competition-stage snapshots. They record interface decisions, board iteration, and debugging history. Tian Bingzhuo was responsible for PCB design, soldering, and board-level iteration records in this team project.

These files are useful as evidence of the electrical integration process, but they should not be treated as final production-ready boards. Follow-up work can add clearer connector labels, board-review notes, design-rule checks, and a concise explanation of each board's role in the vehicle.
