# SPDX-License-Identifier: Apache-2.0
"""Synthetic line-center extraction for a smart-car controller demo."""

from __future__ import annotations

import math
from dataclasses import dataclass


@dataclass(frozen=True)
class LineObservation:
    frame_index: int
    line_center: float
    image_center: float
    confidence: float

    @property
    def error(self) -> float:
        return self.line_center - self.image_center


def synthetic_scanline(width: int, line_center: float, line_width: float = 5.0) -> list[int]:
    """Return a grayscale scanline where the dark band is the lane line."""
    pixels = []
    for x in range(width):
        distance = abs(x - line_center)
        darkness = 210 * math.exp(-(distance * distance) / (2 * line_width * line_width))
        pixels.append(max(0, min(255, int(230 - darkness))))
    return pixels


def detect_dark_line(scanline: list[int]) -> tuple[float, float]:
    weights = [255 - pixel for pixel in scanline]
    total = sum(weights)
    if total == 0:
        return (len(scanline) / 2.0, 0.0)
    center = sum(index * weight for index, weight in enumerate(weights)) / total
    confidence = min(1.0, total / (len(scanline) * 120.0))
    return center, confidence


def steering_from_error(error_px: float, image_width: int, gain: float = 1.6) -> float:
    normalized = error_px / max(1.0, image_width / 2.0)
    return max(-1.0, min(1.0, gain * normalized))


def main() -> int:
    width = 96
    image_center = (width - 1) / 2.0
    for frame in range(8):
        true_center = image_center + math.sin(frame / 2.0) * 18.0
        scanline = synthetic_scanline(width, true_center)
        observed_center, confidence = detect_dark_line(scanline)
        observation = LineObservation(frame, observed_center, image_center, confidence)
        steering = steering_from_error(observation.error, width)
        print(
            f"{frame:02d} center={observation.line_center:5.2f} "
            f"error={observation.error:+5.2f}px confidence={confidence:.2f} steering={steering:+.3f}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
