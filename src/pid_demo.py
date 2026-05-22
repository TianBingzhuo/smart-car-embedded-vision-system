"""Minimal PID demo for portfolio use.

This is a synthetic demo. It does not use raw competition code or data.
"""

from dataclasses import dataclass


@dataclass
class PID:
    kp: float
    ki: float
    kd: float
    integral: float = 0.0
    previous_error: float = 0.0

    def step(self, error: float, dt: float) -> float:
        self.integral += error * dt
        derivative = (error - self.previous_error) / dt if dt > 0 else 0.0
        self.previous_error = error
        return self.kp * error + self.ki * self.integral + self.kd * derivative


def simulate() -> None:
    pid = PID(kp=0.8, ki=0.02, kd=0.12)
    target = 0.0
    position = 1.0
    dt = 0.1

    for step in range(30):
        error = target - position
        control = pid.step(error, dt)
        position += control * dt
        print(f"{step:02d} error={error:+.4f} control={control:+.4f} position={position:+.4f}")


if __name__ == "__main__":
    simulate()

