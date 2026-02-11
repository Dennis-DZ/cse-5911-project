# Software Architecture

## Component Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                      main.cpp (Arduino)                      │
│  - Creates ArduinoHAL                                       │
│  - Instantiates MedicalDeviceController                     │
│  - Calls setup() and loop()                                 │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│              MedicalDeviceController                         │
│  - Platform-independent orchestration                       │
│  - Owns channels and display                                │
│  - initialize() and update() methods                        │
└─────────┬───────────────────────────────────────┬───────────┘
          │                                       │
          ▼                                       ▼
┌──────────────────────┐              ┌──────────────────────┐
│  Channel 1           │              │  Channel 2           │
│  ┌────────────────┐  │              │  ┌────────────────┐  │
│  │ Input Source   │  │              │  │ Input Source   │  │
│  │ (Potentiometer)│  │              │  │ (Potentiometer)│  │
│  └───────┬────────┘  │              │  └───────┬────────┘  │
│          │           │              │          │           │
│          ▼           │              │          ▼           │
│  ┌────────────────┐  │              │  ┌────────────────┐  │
│  │  PWM Output    │  │              │  │  PWM Output    │  │
│  └────────────────┘  │              │  └────────────────┘  │
└──────────────────────┘              └──────────────────────┘
          │                                       │
          └───────────────┬───────────────────────┘
                          ▼
                ┌──────────────────────┐
                │  DisplayManager      │
                │  - Shows values      │
                └──────────────────────┘
                          │
                          ▼
          ┌───────────────────────────────┐
          │     IHAL (Interface)          │
          │  - analogRead()               │
          │  - analogWrite()              │
          │  - display operations         │
          └───────────────┬───────────────┘
                          │
          ┌───────────────┴────────────────┐
          │                                │
          ▼                                ▼
┌──────────────────┐            ┌──────────────────┐
│  ArduinoHAL      │            │  RaspberryPiHAL  │
│  (Actual HW)     │            │  (Future)        │
└──────────────────┘            └──────────────────┘
```

## Data Flow

```
Potentiometer → analogRead() → PotentiometerInput → Channel → PWMOutput → analogWrite() → Physical Output
                    ↑                                   ↓
                    │                                   │
                  IHAL ←──────── DisplayManager ←───────┘
                    ↓
              Arduino Hardware
```

## Dependency Graph

```
main.cpp
  └─→ MedicalDeviceController
        ├─→ PotentiometerInput (x2)
        │     └─→ IHAL
        ├─→ PWMOutput (x2)
        │     └─→ IHAL
        ├─→ Channel (x2)
        │     ├─→ IInputSource
        │     └─→ PWMOutput
        └─→ DisplayManager
              └─→ IHAL
```

## Interface Hierarchy

```
IHAL (Interface)
  └─→ ArduinoHAL (Implementation)
  └─→ MockHAL (Test Implementation)
  └─→ RaspberryPiHAL (Future)

IInputSource (Interface)
  └─→ PotentiometerInput (Implementation)
  └─→ TemperatureSensor (Future)
  └─→ MockInputSource (Test)
```

## Testing Architecture

```
Test Files (test/*)
  └─→ Use MockHAL instead of ArduinoHAL
  └─→ Use MockInputSource for testing Channel
  └─→ Run on native platform (your computer)
  └─→ Unity test framework
```

## Execution Flow

### Initialization (setup())
1. main.cpp creates ArduinoHAL
2. main.cpp creates MedicalDeviceController with HAL
3. MedicalDeviceController creates:
   - 2 PotentiometerInputs
   - 2 PWMOutputs
   - 2 Channels (connecting inputs to outputs)
   - 1 DisplayManager
4. main.cpp calls controller->initialize()
5. DisplayManager initializes the OLED

### Main Loop (loop())
1. main.cpp calls controller->update()
2. MedicalDeviceController calls channel1->update()
   - Channel reads from PotentiometerInput
   - PotentiometerInput calls hal->analogRead()
   - Channel sets value on PWMOutput
   - PWMOutput calls hal->analogWrite()
3. MedicalDeviceController calls channel2->update()
   - (Same as channel1)
4. MedicalDeviceController calls display->updateDisplay()
   - DisplayManager calls various hal->display*() methods
5. Small delay
6. Repeat

## Extension Points

### To Add a New Input Type

1. Create class implementing `IInputSource`
2. Implement `getValue()` method
3. Use it in Channel instead of PotentiometerInput

Example:
```cpp
class TemperatureSensor : public IInputSource {
    float getValue() override {
        // Read temperature, normalize to 0.0-1.0
    }
};
```

### To Add Control Logic

Modify `Channel::update()`:
```cpp
void Channel::update() {
    float raw = inputSource->getValue();
    
    // Add your logic here:
    // - Filtering
    // - PID control
    // - Safety limits
    // - Non-linear mapping
    
    output->setValue(processed);
}
```

### To Add Display Information

Modify `DisplayManager::updateDisplay()`:
```cpp
void DisplayManager::updateDisplay(float ch1, float ch2, float temp1, float temp2) {
    // Show more information
    hal->displayPrint("Temp 1: ");
    // ...
}
```

### To Switch Platforms

1. Implement new HAL (e.g., RaspberryPiHAL)
2. Create new main file using new HAL
3. All other code stays identical!

```cpp
// main_rpi.cpp
RaspberryPiHAL hal;
MedicalDeviceController controller(&hal);
controller.initialize();
while(true) {
    controller.update();
}
```

## Memory Management

- All components use dynamic allocation (new/delete)
- MedicalDeviceController owns all components
- Destructor cleans up in reverse order of creation
- No memory leaks if controller is properly destroyed

## Thread Safety

Current implementation is single-threaded. For multi-threading:
- Add mutexes around shared state
- Consider using RTOS (FreeRTOS, etc.)
- Protect HAL calls if accessed from multiple threads
