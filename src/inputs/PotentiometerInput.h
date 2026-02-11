#ifndef POTENTIOMETER_INPUT_H
#define POTENTIOMETER_INPUT_H

#include "IInputSource.h"
#include "../hal/IHAL.h"

/**
 * Potentiometer Input Source
 * 
 * Reads an analog potentiometer and provides normalized values.
 * Arduino analog inputs range from 0-1023, which are normalized to 0.0-1.0.
 */
class PotentiometerInput : public IInputSource {
private:
    IHAL* hal;
    int pin;
    static const int ANALOG_MAX = 1023;  // Arduino ADC is 10-bit
    
public:
    /**
     * Constructor
     * @param hal Hardware abstraction layer for reading analog values
     * @param pin Analog pin number to read from
     */
    PotentiometerInput(IHAL* hal, int pin);
    
    /**
     * Read the potentiometer value
     * @return Normalized value between 0.0 and 1.0
     */
    float getValue() override;
};

#endif // POTENTIOMETER_INPUT_H
