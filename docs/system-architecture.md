# System Architecture

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

- OpenART provides visual recognition results to the RT1064 control logic.
- The RT1064 firmware combines recognition output, encoder/IMU feedback, and mission state to generate steering and motor commands.
- For a future ROS 2 version, recognition output can be treated as a topic, the controller as a node, and the motor driver as an actuator interface.
