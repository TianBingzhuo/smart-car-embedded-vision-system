# Hardware Overview

This project combines a vision smart-car control stack with custom board iteration and multiple sensing/actuation modules.

## Main Hardware Chain

```text
camera / OpenART modules
  -> recognition result over serial
  -> RT1064 control board
  -> motor, steering, and actuator drivers
  -> vehicle motion
  -> IMU / encoder / field debugging feedback
```

## Hardware Files

- `hardware/pcb/`: schematic and PCB screenshots plus EasyEDA project exports.
- `hardware/rt1064-pin-map.txt`: RT1064 pin mapping note.
- `hardware/imu/`: IMU660RA configuration and tuning notes.

## PCB Status

The PCB/schematic files are competition-stage snapshots. They are useful for documenting interface decisions, board iteration, and debugging history, but they still need later cleanup: clearer connector labels, review notes, and a concise explanation of each board's role in the vehicle.
