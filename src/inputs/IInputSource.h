#ifndef IINPUT_SOURCE_H
#define IINPUT_SOURCE_H

/**
 * Input Source Interface
 * 
 * This interface represents any source of input values for the control system.
 * Implementations include potentiometers, sensors, network inputs, etc.
 * 
 * All values are normalized to the range [0.0, 1.0] for consistency.
 */
class IInputSource {
public:
    virtual ~IInputSource() {}
    
    /**
     * Get the current input value
     * @return Normalized value between 0.0 and 1.0
     */
    virtual float getValue() = 0;
};

#endif // IINPUT_SOURCE_H
