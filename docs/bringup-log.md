# Vehicle Bring-Up Log

This page summarizes the short Chinese field log in `docs/raw-notes/bringup-log.zh-CN.txt`.

## Timeline

| Date | Milestone |
|---|---|
| Feb 28 | Speed-loop PID debugging was still failing; hardware board issues were suspected. |
| Feb 29 | Basic movement modes were completed and PID tuning started. Path tracking became the next blocker. |
| Mar 2 | Path tracking reached an initial working state. Remaining tasks included ring logic, obstacle avoidance, OpenART data, structure modification, and servo integration. |
| Mar 10 | Wi-Fi wireless tuning/control and path tracking were working. OpenART on-vehicle testing, structural changes, hardware board work, and servo work remained. |
| Mar 19 | Added servo code, zebra-crossing recognition and stop logic, and improved path tracking. |
| Apr 6 | Added ring tracking. Most components were mounted and usable except OpenART and some structure work. |
| Apr 21 | Added OpenART logic, card-detection code, filtering code, improved ring tracking, and simplified the main function. |

## Engineering Pattern

The log shows a normal embedded-robotics iteration loop:

```text
single module works
  -> vehicle integration exposes new instability
  -> tune PID / sensor / structure
  -> add perception or mission logic
  -> simplify main control flow
```

This is the part of the project that matters most for a portfolio: it demonstrates repeated field debugging rather than a one-shot code upload.
