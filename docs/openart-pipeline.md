# OpenART Pipeline

The project used OpenART modules for visual recognition and target-detection tasks. The public folder keeps scripts, labels, command-mode notes, and deployment helpers, while model weights are excluded by default.

## Public Files

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

## Release Boundary

The original folder contained several `.tflite` model files. They are not included here because model provenance and redistribution rights should be checked before publishing. If a model is confirmed to be self-trained and safe to redistribute, publish only the final model with a short model card that explains dataset source, classes, input size, quantization mode, and intended hardware.
