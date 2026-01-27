#include "gpio.h"
#include <iostream>
#include <map>
#include <string>

namespace bsp {

static std::map<int,bool> pin_state;

int get_id(Pin pin){
    return static_cast<int>(pin.port)*100+pin.index;
}

std::string portToString(Port port) {
    if (port == Port::A) return "PA";
    if (port == Port::B) return "PB";
    if (port == Port::C) return "PC";
    return "UNKNOWN";
}

std::string modeToString(PinMode mode) {
    switch(mode) {
        case PinMode::INPUT: return "INPUT";
        case PinMode::OUTPUT: return "OUTPUT";
        case PinMode::PERIPHERAL_A: return "PERIPHERAL_A";
        case PinMode::PERIPHERAL_B: return "PERIPHERAL_B";
        case PinMode::ANALOG_INPUT: return "ANALOG_INPUT";
        case PinMode::ANALOG_OUTPUT: return "ANALOG_OUTPUT";
        default: return "UNKNOWN";
    }
}

std::string pullToString(PullMode pull) {
    switch(pull) {
        case PullMode::NONE: return "NONE";
        case PullMode::UP: return "PULL_UP";
        case PullMode::DOWN: return "PULL_DOWN";
        default: return "UNKNOWN";
    }
}

void initSystem() {
    std::cout << "(MOCK) Init System... Enabling Clocks..." << std::endl;
}

void GpioDriver::configure(Pin pin, PinMode mode, PullMode pull){
    std::cout << "(MOCK) Configuring " << portToString(pin.port) << pin.index 
              << " -> Mode: " << modeToString(mode) 
              << " | Pull: " << pullToString(pull) << std::endl;
}

void GpioDriver::set(Pin pin){
    pin_state[get_id(pin)] = true;
    std::cout << "(MOCK) " << portToString(pin.port) << pin.index << " = HIGH" << std::endl;
}

void GpioDriver::clear(Pin pin){
    pin_state[get_id(pin)] = false;
    std::cout << "(MOCK) " << portToString(pin.port) << pin.index << " = LOW" << std::endl;
}

void GpioDriver::toggle(Pin pin){
    bool current = pin_state[get_id(pin)];
    pin_state[get_id(pin)] = !current;
    std::cout << "(MOCK) Toggling " << portToString(pin.port) << pin.index 
              << " -> New State: " << (!current ? "HIGH" : "LOW") << std::endl;
}

bool GpioDriver::read(Pin pin){
    bool state = pin_state[get_id(pin)];
    std::cout << "(MOCK) Reading " << portToString(pin.port) << pin.index 
              << " -> State: " << (state ? "HIGH" : "LOW") << std::endl;
    return state;
}

}
