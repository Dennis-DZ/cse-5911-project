#include "PotentiometerInput.h"

PotentiometerInput::PotentiometerInput(IHAL* hal, int pin) 
    : hal(hal), pin(pin) {
}

float PotentiometerInput::getValue() {
    int rawValue = hal->analogRead(pin);
    // Normalize to 0.0 - 1.0
    return (float)rawValue / (float)ANALOG_MAX;
}
