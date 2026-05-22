# System Architecture Draft

```mermaid
flowchart LR
  A["Camera / visual input"] --> B["Recognition output"]
  B --> C["Control decision"]
  C --> D["PID / steering command"]
  D --> E["Motor driver"]
  E --> F["Vehicle motion"]
  F --> G["Debugging and observation"]
  G --> C
```

## Notes

- This diagram is a public abstraction, not a copy of raw competition materials.
- The future ROS 2 mapping can treat recognition output as a topic, the controller as a node, and the motor driver as an actuator interface.

