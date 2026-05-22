# Upstream Source Note

The local archive contains a Git repository with this remote:

```text
https://github.com/0Leeeezy0/IntelligentCar_Baidu.git
```

The user also reported that the teammate-side smart-car design code has been
published on Gitee and that the team has agreed to reuse it.

## Publication Recommendation

For the first GitHub portfolio release, use this page as a public project entry
and link the upstream source repository. If the code is to be imported into a
new Apache-2.0 repository under `TianBingzhuo`, confirm these items first:

- all contributors agree to the license;
- the upstream repository receives a `LICENSE` file, preferably Apache-2.0 if
  the team agrees;
- build outputs, videos, model binaries, private configs, and generated files
  are excluded;
- third-party libraries such as ONNX Runtime, json.hpp, board SDKs, and vendor
  examples are kept under their original licenses and not relicensed.

## Candidate Public Scope

- `README.md`, `self/README.md`, `race/README.md`;
- self-written `self/src`, `self/lib/swan`, `self/include/swan`;
- selected `race/lib` and `race/include` files after review;
- build instructions and architecture notes.

Avoid committing local `build/`, sample videos, large model files, `.so`
binaries, and generated IDE artifacts unless there is a clear public-release
reason.
