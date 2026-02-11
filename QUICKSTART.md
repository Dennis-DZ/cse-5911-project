# Quick Start Guide

## What You Have

A complete, professional medical device control system with:
- ✅ Hardware abstraction for platform independence
- ✅ Modular, testable architecture
- ✅ Comprehensive unit tests
- ✅ Full documentation
- ✅ Ready for Arduino deployment
- ✅ Easy to extend with new features

## First Steps

### 1. Install PlatformIO (5 minutes)

**In VSCode:**
1. Open VSCode
2. Go to Extensions (Ctrl+Shift+X)
3. Search for "PlatformIO IDE"
4. Click Install
5. Reload VSCode

### 2. Open the Project (1 minute)

1. File → Open Folder
2. Select the `medical-device-controller` folder
3. PlatformIO will initialize automatically

### 3. Run the Tests (2 minutes)

Make sure everything works:

```bash
pio test -e native
```

You should see all tests passing! This confirms:
- ✅ PotentiometerInput works correctly
- ✅ PWMOutput handles values properly
- ✅ Channels transfer input to output
- ✅ Display manager functions
- ✅ Complete system integration works

### 4. Build for Arduino (1 minute)

```bash
pio run -e uno
```

This compiles the code for Arduino. If it succeeds, you're ready to deploy!

### 5. Connect Hardware

**Required:**
- Arduino Uno
- 2 potentiometers (connect to A0 and A1)
- SSD1306 OLED display (I2C: SDA, SCL)

**Potentiometer wiring:**
- Left pin → GND
- Middle pin → A0 (first pot) or A1 (second pot)
- Right pin → 5V

**OLED wiring:**
- VCC → 5V
- GND → GND
- SDA → A4 (on Uno)
- SCL → A5 (on Uno)

**Outputs:**
- Pin 9 → Your first output device
- Pin 10 → Your second output device

### 6. Upload to Arduino

```bash
pio run -e uno -t upload
```

Or use the → (Upload) button in VSCode's PlatformIO toolbar.

### 7. Test It!

1. Turn the potentiometers
2. Watch the display update with percentages
3. Measure PWM output on pins 9 and 10
4. Verify outputs match potentiometer positions

## What Each File Does

### Core Application (Platform-Independent)
- `MedicalDeviceController.h/cpp` - Main controller orchestrating everything
- `control/Channel.h/cpp` - Links input to output, where control logic goes
- `inputs/PotentiometerInput.h/cpp` - Reads and normalizes potentiometer values
- `outputs/PWMOutput.h/cpp` - Controls PWM output pins
- `display/DisplayManager.h/cpp` - Manages the OLED display

### Hardware Abstraction
- `hal/IHAL.h` - Interface defining all hardware operations
- `hal/ArduinoHAL.h/cpp` - Arduino-specific implementation

### Platform-Specific
- `main.cpp` - Arduino setup() and loop() functions

### Testing
- `test/MockHAL.h` - Simulated hardware for testing
- `test/test_*/` - Unit tests for each component

## Common Tasks

### Run All Tests
```bash
pio test -e native
```

### Run Specific Test
```bash
pio test -e native -f test_channel
```

### Build Without Uploading
```bash
pio run -e uno
```

### Upload to Arduino
```bash
pio run -e uno -t upload
```

### Clean Build
```bash
pio run -t clean
```

### Monitor Serial Output
```bash
pio device monitor
```

## Adding Your First Feature: Temperature Display

Let's add temperature sensor support as an example:

### 1. Create TemperatureSensor class

Create `src/inputs/TemperatureSensor.h`:
```cpp
#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "IInputSource.h"
#include "../hal/IHAL.h"

class TemperatureSensor : public IInputSource {
private:
    IHAL* hal;
    int pin;
    float minTemp;
    float maxTemp;
    
public:
    TemperatureSensor(IHAL* hal, int pin, float minTemp, float maxTemp);
    float getValue() override;  // Returns 0.0-1.0 based on temp range
    float getTemperature();     // Returns actual temperature
};

#endif
```

Create `src/inputs/TemperatureSensor.cpp`:
```cpp
#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(IHAL* hal, int pin, float minTemp, float maxTemp)
    : hal(hal), pin(pin), minTemp(minTemp), maxTemp(maxTemp) {}

float TemperatureSensor::getTemperature() {
    int rawValue = hal->analogRead(pin);
    // Convert ADC reading to temperature (depends on your sensor)
    // Example for TMP36: (voltage - 0.5) * 100
    float voltage = (rawValue / 1024.0) * 5.0;
    return (voltage - 0.5) * 100.0;
}

float TemperatureSensor::getValue() {
    float temp = getTemperature();
    // Normalize to 0.0-1.0 range
    return (temp - minTemp) / (maxTemp - minTemp);
}
```

### 2. Update MedicalDeviceController

In `MedicalDeviceController.h`, add:
```cpp
#include "inputs/TemperatureSensor.h"

// Add member variables:
TemperatureSensor* tempSensor1;
TemperatureSensor* tempSensor2;
```

In `MedicalDeviceController.cpp`, constructor:
```cpp
// Add after creating potentiometers:
tempSensor1 = new TemperatureSensor(hal, 2, 0.0, 100.0);  // A2, 0-100°C
tempSensor2 = new TemperatureSensor(hal, 3, 0.0, 100.0);  // A3, 0-100°C
```

### 3. Update Display

In `DisplayManager.h`, add:
```cpp
void updateDisplay(float ch1, float ch2, float temp1, float temp2);
```

In `DisplayManager.cpp`:
```cpp
void DisplayManager::updateDisplay(float ch1, float ch2, float temp1, float temp2) {
    hal->displayClear();
    
    // Channel 1
    hal->displaySetCursor(0, 0);
    hal->displayPrint("Ch1:");
    char buffer[16];
    sprintf(buffer, "%d%% %.1fC", (int)(ch1*100), temp1);
    hal->displayPrint(buffer);
    
    // Channel 2
    hal->displaySetCursor(0, 20);
    hal->displayPrint("Ch2:");
    sprintf(buffer, "%d%% %.1fC", (int)(ch2*100), temp2);
    hal->displayPrint(buffer);
    
    hal->displayShow();
}
```

### 4. Update Controller

In `MedicalDeviceController::update()`:
```cpp
void MedicalDeviceController::update() {
    channel1->update();
    channel2->update();
    
    display->updateDisplay(
        channel1->getOutputValue(),
        channel2->getOutputValue(),
        tempSensor1->getTemperature(),
        tempSensor2->getTemperature()
    );
}
```

### 5. Write Tests

Create `test/test_temperature/test_temperature.cpp`:
```cpp
#include <unity.h>
#include "../test/MockHAL.h"
#include "../src/inputs/TemperatureSensor.h"

MockHAL hal;
TemperatureSensor* sensor;

void setUp(void) {
    sensor = new TemperatureSensor(&hal, 2, 0.0, 100.0);
}

void tearDown(void) {
    delete sensor;
}

void test_temperature_normalizes_correctly(void) {
    // Simulate 50°C reading
    hal.setAnalogInput(2, 512);  // Adjust based on your sensor
    float normalized = sensor->getValue();
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.5, normalized);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_temperature_normalizes_correctly);
    return UNITY_END();
}
```

### 6. Test and Deploy

```bash
pio test -e native          # Run tests
pio run -e uno -t upload    # Upload to Arduino
```

Done! You've extended the system with temperature sensing.

## Troubleshooting

### "Command not found: pio"
- PlatformIO not installed or not in PATH
- Solution: Install PlatformIO extension in VSCode

### Tests won't compile
- Make sure using `-e native` not `-e uno`
- Tests only run on native platform

### Upload fails
- Check USB connection
- Try different USB port/cable
- Verify board is Arduino Uno in platformio.ini

### Display doesn't work
- Check I2C wiring (SDA=A4, SCL=A5 on Uno)
- Verify display address (0x3C or 0x3D)
- Some displays need different initialization

### Wrong PWM values
- Check that you're using PWM-capable pins (3, 5, 6, 9, 10, 11 on Uno)
- Verify wiring and grounds

## Next Steps

Now that you have the system working:

1. **Understand the architecture** - Read ARCHITECTURE.md
2. **Add your features** - Temperature sensors? Control loops?
3. **Write tests first** - TDD makes development faster
4. **Commit to git** - Track your changes
5. **Collaborate** - Use branches and pull requests

## Getting Help

- Read the detailed README.md
- Check ARCHITECTURE.md for design details
- Look at existing tests for examples
- All code is documented with comments

## Project Status

✅ Hardware abstraction complete
✅ Basic I/O working
✅ Display functional
✅ Tests passing
✅ Ready for feature development

You're all set to start building your medical device control system!
