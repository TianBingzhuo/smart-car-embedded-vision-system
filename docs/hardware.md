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

## Public Hardware Files

- `hardware/pcb/`: schematic and PCB screenshots plus EasyEDA project exports.
- `hardware/rt1064-pin-map.txt`: RT1064 pin mapping note.
- `hardware/imu/`: IMU660RA configuration and tuning notes.

## Notes

The public repository keeps project-owned board exports and readable engineering notes. Large vendor datasheets, complete vendor SDK archives, binary tool installers, and raw competition media are intentionally excluded.
