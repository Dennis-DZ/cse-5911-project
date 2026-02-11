#ifndef CHANNEL_H
#define CHANNEL_H

#include "../inputs/IInputSource.h"
#include "../outputs/PWMOutput.h"

/**
 * Channel Controller
 * 
 * Manages a single control channel that reads from an input source
 * and drives an output. This architecture makes it easy to add
 * processing, filtering, or control logic in the future.
 */
class Channel {
private:
    IInputSource* inputSource;
    PWMOutput* output;
    
public:
    /**
     * Constructor
     * @param inputSource Source of input values (potentiometer, sensor, etc.)
     * @param output PWM output to control
     */
    Channel(IInputSource* inputSource, PWMOutput* output);
    
    /**
     * Update the channel - read input and set output
     * This is where you would add control logic, transforms, etc.
     */
    void update();
    
    /**
     * Get the current output value
     * @return Current output value (0.0 - 1.0)
     */
    float getOutputValue() const;
};

#endif // CHANNEL_H
