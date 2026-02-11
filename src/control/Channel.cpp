#include "Channel.h"

Channel::Channel(IInputSource* inputSource, PWMOutput* output) 
    : inputSource(inputSource), output(output) {
}

void Channel::update() {
    // Read from input and directly set output
    // Future enhancements could add:
    // - Filtering/smoothing
    // - Min/max limits
    // - Non-linear transforms
    // - PID control
    float inputValue = inputSource->getValue();
    output->setValue(inputValue);
}

float Channel::getOutputValue() const {
    return output->getValue();
}
