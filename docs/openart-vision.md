# OpenART Vision Notes

We used OpenART modules for the visual recognition and target-detection parts of the competition. This note documents the scripts, labels, serial command modes, and deployment conventions that the vision side of our car relied on.

The vision work in our team was mainly handled by 么林坤. The Python files in this directory are OpenART-side scripts and deployment helpers — they were written to run on the actual OpenART hardware on the car, not as standalone desktop examples.

## Files

- `firmware/openart/art_up.py`: upper OpenART script.
- `firmware/openart/art_down.py`: lower OpenART script.
- `firmware/openart/down/main.py`: deployable lower-board script copy.
- `firmware/openart/down/cmm_load.py`: model/config loading helper.
- `firmware/openart/down/cmm_cfg.csv`: configuration table.
- `firmware/openart/down/labels*.txt`: label files.
- `firmware/openart/README.zh-CN.txt`: original deployment note.

## Runtime Command Modes

The RT1064 communicates with the OpenART modules over serial, using a simple command convention. Here's what each command does:

| Command | Meaning |
|---|---|
| `0` | standby / disable active recognition |
| `1` | upper OpenART recognizes cards |
| `2` | upper OpenART recognizes letters |
| `3` | upper OpenART recognizes numbers `1`, `2`, `3` |
| `4` | lower OpenART runs target detection |

One thing worth mentioning: when switching between functions, you need to send `0` first to put everything in standby, then send the target mode command. We learned this the hard way after some confusing behavior during early testing.

## Model Notes

This repo keeps the OpenART scripts and labels that explain how the vision side worked. The actual trained model files aren't included here. If you add a trained model later, it'd be a good idea to document the dataset source, classes, input size, quantization mode, and intended hardware right next to the model file — your future self will thank you.

---

# 中文版本

# OpenART 视觉笔记

竞赛里的视觉识别和目标检测用的是 OpenART 模块。这个文件记录了视觉部分用到的脚本、标签、串口命令模式和部署约定。

团队里的视觉工作主要是么林坤负责的。这个目录下的 Python 文件是 OpenART 端的脚本和部署辅助——它们是写来在车上实际运行的 OpenART 硬件上跑的，不是独立的桌面示例。

## 文件列表

- `firmware/openart/art_up.py`：上位 OpenART 脚本。
- `firmware/openart/art_down.py`：下位 OpenART 脚本。
- `firmware/openart/down/main.py`：可部署的下位板脚本副本。
- `firmware/openart/down/cmm_load.py`：模型/配置加载辅助。
- `firmware/openart/down/cmm_cfg.csv`：配置表。
- `firmware/openart/down/labels*.txt`：标签文件。
- `firmware/openart/README.zh-CN.txt`：原始部署说明。

## 运行时命令模式

RT1064 通过串口和 OpenART 模块通信，用的是一个简单的命令约定。每个命令的含义如下：

| 命令 | 含义 |
|---|---|
| `0` | 待机 / 关闭主动识别 |
| `1` | 上位 OpenART 识别卡片 |
| `2` | 上位 OpenART 识别字母 |
| `3` | 上位 OpenART 识别数字 `1`、`2`、`3` |
| `4` | 下位 OpenART 执行目标检测 |

有一点值得说一下：切换功能的时候，要先发 `0` 让所有模块进入待机状态，然后再发目标模式的命令。这是我们在早期测试时踩过的坑，当时出现了一些莫名其妙的行为，后来才发现是这个顺序问题。

## 模型说明

仓库里保留了 OpenART 的脚本和标签文件，能看出视觉端是怎么工作的。实际的训练模型文件没有包含在这里。如果后续要加入训练好的模型，建议在模型文件旁边记录一下数据集来源、类别、输入尺寸、量化方式和目标硬件——以后的自己会感谢现在的自己。
