# Medical Device Controller

A robust, testable, and platform-independent control system for a medical device. This project demonstrates professional embedded software architecture with hardware abstraction, dependency injection, and comprehensive unit testing.

## Features

- **Hardware Abstraction Layer (HAL)**: Platform-independent code that can run on Arduino, Raspberry Pi, or any other platform with a HAL implementation
- **Two Independent Channels**: Each channel reads from a potentiometer and controls a PWM output
- **OLED Display**: Shows real-time output values for both channels
- **Unit Tested**: Comprehensive test suite that runs on your computer (no hardware required)
- **Modular Design**: Easy to extend with new sensors, control logic, or features

## Hardware Requirements

- Arduino Uno (or compatible board)
- Two potentiometers connected to A0 and A1
- Two PWM-capable outputs on pins 9 and 10
- SSD1306 OLED display (128x64) connected via I2C

## Software Requirements

- PlatformIO (installed as VSCode extension or CLI)
- VSCode (recommended) or any PlatformIO-compatible editor

## Project Structure

```
medical-device-controller/
├── platformio.ini          # Build configuration
├── src/
│   ├── main.cpp           # Arduino entry point
│   ├── MedicalDeviceController.h/cpp  # Main controller (platform-independent)
│   ├── hal/               # Hardware Abstraction Layer
│   │   ├── IHAL.h         # HAL interface
│   │   ├── ArduinoHAL.h/cpp  # Arduino implementation
│   ├── inputs/            # Input sources
│   │   ├── IInputSource.h    # Input interface
│   │   ├── PotentiometerInput.h/cpp
│   ├── outputs/           # Output handlers
│   │   ├── PWMOutput.h/cpp
│   ├── control/           # Control logic
│   │   ├── Channel.h/cpp
│   └── display/           # Display management
│       ├── DisplayManager.h/cpp
└── test/                  # Unit tests
    ├── MockHAL.h          # Mock hardware for testing
    ├── test_potentiometer/
    ├── test_pwm_output/
    ├── test_channel/
    ├── test_display/
    └── test_integration/
```

## Architecture Overview

### Component Hierarchy

1. **HAL (Hardware Abstraction Layer)**
   - Abstracts all hardware operations
   - Different platforms implement the same interface
   - Makes the rest of the code platform-independent

2. **Input Sources** (implements `IInputSource`)
   - `PotentiometerInput`: Reads analog potentiometer, normalizes to 0.0-1.0
   - Easy to add: `TemperatureSensor`, `NetworkInput`, etc.

3. **Outputs**
   - `PWMOutput`: Controls PWM output, accepts normalized values (0.0-1.0)

4. **Channel**
   - Combines an input source with an output
   - Where you add control logic, filtering, PID, etc.

5. **Display Manager**
   - Handles display updates
   - Easy to add more information (temperatures, status, etc.)

6. **Medical Device Controller**
   - Orchestrates all channels and the display
   - Platform-independent application logic

### Key Design Principles

- **Dependency Injection**: Components receive dependencies rather than creating them
- **Interface-Based Design**: Code to interfaces, not implementations
- **Single Responsibility**: Each class has one clear purpose
- **Open/Closed**: Open for extension, closed for modification

## Getting Started

### 1. Install PlatformIO

In VSCode:
1. Install the "PlatformIO IDE" extension
2. Reload VSCode

### 2. Open the Project

1. Open VSCode
2. File → Open Folder → Select `medical-device-controller`
3. PlatformIO will auto-detect the project

### 3. Build and Upload

For Arduino:
```bash
pio run -e uno              # Build
pio run -e uno -t upload    # Upload to Arduino
```

Or use the PlatformIO buttons in VSCode.

### 4. Run Tests

Run all tests on your computer (no hardware needed):
```bash
pio test -e native
```

Run specific test:
```bash
pio test -e native -f test_potentiometer
```

## Current Functionality

The system currently:
1. Reads two potentiometers (A0 and A1)
2. Outputs proportional PWM signals (pins 9 and 10)
3. Displays both output values on an OLED screen
4. Updates continuously in the main loop

**Example**: If potentiometer 1 is at 50%, output 1 will be at 50% PWM duty cycle, and the display will show "50%".

## Adding Features

### Adding a Temperature Sensor

1. Create `TemperatureSensor.h/cpp` in `src/inputs/` implementing `IInputSource`
2. Update `MedicalDeviceController` to use it:
   ```cpp
   // Replace or supplement potentiometer
   temp1 = new TemperatureSensor(hal, TEMP_PIN);
   channel1 = new Channel(temp1, output1);
   ```
3. Update `DisplayManager` to show temperature

### Adding Control Logic

Modify `Channel::update()` to add:
- **Filtering**: Smooth noisy inputs
  ```cpp
  float raw = inputSource->getValue();
  float filtered = (raw * 0.1) + (lastValue * 0.9);  // Low-pass filter
  output->setValue(filtered);
  ```
- **PID Control**: For temperature regulation
- **Safety Limits**: Min/max constraints
- **Non-linear Mapping**: Custom transfer functions

### Switching to Raspberry Pi

1. Create `RaspberryPiHAL.h/cpp` implementing `IHAL`
2. Create new main file (e.g., `main.py` or `main_rpi.cpp`)
3. Use `RaspberryPiHAL` instead of `ArduinoHAL`
4. All other code stays the same!

## Testing Philosophy

- **Unit Tests**: Test each component in isolation with mock dependencies
- **Integration Tests**: Test components working together
- **Native Execution**: Tests run on your computer for fast feedback
- **Mock HAL**: Simulates hardware without requiring physical devices

### Example Test

```cpp
void test_channel_transfers_value(void) {
    input->setValue(0.5);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, channel->getOutputValue());
}
```

## Git Workflow

The project is set up for git collaboration:

```bash
# Initial setup
git init
git add .
git commit -m "Initial medical device controller"

# Feature branch workflow
git checkout -b feature/temperature-sensor
# ... make changes ...
git commit -m "Add temperature sensor support"
git push origin feature/temperature-sensor
# ... create pull request ...
```

### .gitignore

Add this to your `.gitignore`:
```
.pio/
.vscode/
*.o
*.elf
*.hex
```

## Pin Configuration

Current pin assignments (defined in `MedicalDeviceController.h`):
- **A0**: Potentiometer 1 (analog input)
- **A1**: Potentiometer 2 (analog input)
- **Pin 9**: PWM Output 1
- **Pin 10**: PWM Output 2
- **SDA/SCL**: I2C for OLED display

To change pins, modify the constants in `MedicalDeviceController.h`.

## Troubleshooting

### Tests Fail to Compile
- Make sure you're using `pio test -e native` (not `-e uno`)
- Check that all test files are in `test/test_*/` directories

### Display Not Working
- Verify I2C connections (SDA/SCL)
- Check display address in `ArduinoHAL.cpp` (default: 0x3C)
- Some displays use 0x3D instead

### Upload Fails
- Check USB connection
- Verify correct board in `platformio.ini`
- Try different USB port or cable

## Future Enhancements

Ideas for expansion:
- [ ] Temperature sensors with PID control
- [ ] Safety interlocks and alarms
- [ ] Data logging to SD card
- [ ] Network connectivity (WiFi/Ethernet)
- [ ] Configuration via serial commands
- [ ] Watchdog timer for reliability
- [ ] EEPROM for persistent settings

## License

[Choose your license]

## Contributors

[Your team members]
