#include "PWMOutput.h"

PWMOutput::PWMOutput(IHAL* hal, int pin) 
    : hal(hal), pin(pin), currentValue(0.0f) {
}

void PWMOutput::setValue(float value) {
    // Clamp value to valid range
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;
    
    currentValue = value;
    
    // Convert to PWM range (0-255)
    int pwmValue = (int)(value * PWM_MAX);
    hal->analogWrite(pin, pwmValue);
}

float PWMOutput::getValue() const {
    return currentValue;
}
