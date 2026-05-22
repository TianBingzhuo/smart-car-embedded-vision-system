# Public Release Boundary

This folder is a cleaned public release. The local evidence folder contains more raw material, but not everything belongs in a GitHub repository.

## Included

- Final RT1064 application-level firmware snapshot.
- Minimal MDK project metadata.
- OpenART scripts, labels, and command-mode notes.
- IMU configuration and tuning notes.
- PCB/schematic screenshots and EasyEDA project exports.
- Cleaned milestone log and synthetic public demos.

## Excluded

- Date-by-date duplicate firmware packages.
- Build outputs: `Objects/`, `Listings/`, `.o`, `.d`, `.map`, `.hex`, `.bin`, `.elf`.
- Local IDE/user files: `.vs/`, `.suo`, `.user`, `*.uvguix.*`.
- IAR project metadata that contained local absolute toolchain paths.
- Third-party SDK dumps and vendor package archives.
- Model weights such as `.tflite` until redistribution rights and provenance are confirmed.
- Raw `.docx` tuning summaries, raw certificates, teammate information, and unmasked photos/videos.
- Large datasheets and binary installers.

## Why

The goal is to show system engineering ability, not to upload a hard-drive backup. A clean public repository should be readable, small enough to browse, and clear about what is original work, what is third-party dependency, and what remains private evidence.
