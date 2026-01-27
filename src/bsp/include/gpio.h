#pragma once
#include <cstdint>

namespace bsp {

enum class Port { A, B, C };

struct Pin {
    Port port;
    int index;
};

enum class PinMode { 
    INPUT, 
    OUTPUT, 
    PERIPHERAL_A, 
    PERIPHERAL_B, 
    ANALOG_INPUT,
    ANALOG_OUTPUT
};

enum class PullMode {
    NONE,
    UP,
    DOWN
};

class GpioDriver {
public:
    void configure(Pin pin, PinMode mode, PullMode pull = PullMode::NONE);
    void set(Pin pin);
    void clear(Pin pin);
    void toggle(Pin pin);
    bool read(Pin pin);
};

void initSystem();

}