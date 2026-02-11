#include "hal/ArduinoHAL.h"
#include "MedicalDeviceController.h"

// Global instances
ArduinoHAL hal;
MedicalDeviceController* controller = nullptr;

void setup() {
    // Create and initialize the controller
    controller = new MedicalDeviceController(&hal);
    controller->initialize();
}

void loop() {
    // Update the controller
    controller->update();
    
    // Small delay to avoid overwhelming the display
    hal.delay(100);
}
