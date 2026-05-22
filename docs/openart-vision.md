# OpenART Vision Notes

The project used OpenART modules for visual recognition and target-detection tasks. This note records the scripts, labels, serial command modes, and deployment conventions used by the smart-car project.

## Files

- `firmware/openart/art_up.py`: upper OpenART script.
- `firmware/openart/art_down.py`: lower OpenART script.
- `firmware/openart/down/main.py`: deployable lower-board script copy.
- `firmware/openart/down/cmm_load.py`: model/config loading helper.
- `firmware/openart/down/cmm_cfg.csv`: configuration table.
- `firmware/openart/down/labels*.txt`: label files.
- `firmware/openart/README.zh-CN.txt`: original deployment note.

## Runtime Command Modes

The original note records the following serial command convention:

| Command | Meaning |
|---|---|
| `0` | standby / disable active recognition |
| `1` | upper OpenART recognizes cards |
| `2` | upper OpenART recognizes letters |
| `3` | upper OpenART recognizes numbers `1`, `2`, `3` |
| `4` | lower OpenART runs target detection |

When switching functions, the system should send `0` first, then send the target mode command.

## Model Notes

Model weights are not included in this repository. If a self-trained model is added later, include a short model card covering dataset source, classes, input size, quantization mode, and intended hardware.
