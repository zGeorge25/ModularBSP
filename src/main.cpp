#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "bsp/include/gpio.h"

using namespace bsp;

void print_test_header(const std::string& name) {
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "TEST: " << name << "\n";
    std::cout << "--------------------------------------------------\n";
}

int main(){
    std::cout << "=== SYSTEM INITIALIZATION ===\n";
    bsp::initSystem();
    std::unique_ptr<GpioDriver> gpio = std::make_unique<GpioDriver>();
    std::cout << "-> System initialized successfully.\n";

    // --- 1. INPUT CONFIGURATION (PB12) ---
    print_test_header("1. INPUT CONFIGURATION");
    std::cout << "[] PB12: Input + Pull-Up\n";
    gpio->configure({Port::B, 12}, PinMode::INPUT, PullMode::UP);
    std::cout << "-> PB12 configured.\n";
    
    // Read Test
    bool val = gpio->read({Port::B, 12});
    std::cout << "-> Value read from PB12: " << (val ? "HIGH" : "LOW") << "\n";


    // --- 2. OUTPUT CONFIGURATION (PC9, PC10) ---
    print_test_header("2. OUTPUT CONFIGURATION");
    std::cout << "[] PC9: Output\n";
    std::cout << "[] PC10: Output\n";
    
    gpio->configure({Port::C, 9}, PinMode::OUTPUT);
    gpio->configure({Port::C, 10}, PinMode::OUTPUT);
    std::cout << "-> PC9 and PC10 configured as OUTPUT.\n";

    // Test TOGGLE PC9 (Manual Set/Clear)
    std::cout << "\n[Test] PC9 Manual Control (SET -> CLEAR):\n";
    std::cout << "-> Setting PC9 HIGH\n";
    gpio->set({Port::C, 9});
    std::cout << "-> Setting PC9 LOW\n";
    gpio->clear({Port::C, 9});

    // Test TOGGLE PC10 (Using toggle function)
    std::cout << "\n[Test] PC10 Toggle Function:\n";
    std::cout << "-> Toggling PC10\n";
    gpio->toggle({Port::C, 10});
    std::cout << "-> Toggling PC10 again\n";
    gpio->toggle({Port::C, 10});


    // --- 3. PERIPHERAL CONFIGURATION (UART) ---
    print_test_header("3. PERIPHERAL CONFIGURATION (UART0)");
    std::cout << "[] PA9: UART0 Rx (Peripheral A)\n";
    std::cout << "[] PA10: UART0 Tx (Peripheral A)\n";
    
    // UART0 is typically on Peripheral A for SAMV71
    gpio->configure({Port::A, 9}, PinMode::PERIPHERAL_A); 
    gpio->configure({Port::A, 10}, PinMode::PERIPHERAL_A);
    std::cout << "-> PA9 and PA10 mapped to UART0 (Peripheral A).\n";


    // --- 4. ANALOG CONFIGURATION (AFEC1) ---
    print_test_header("4. ANALOG CONFIGURATION (AFEC1)");
    std::cout << "[] PB1: AFEC1 Channel 0 (Analog Input)\n";
    
    // Configuring as Analog Input disables digital drivers
    gpio->configure({Port::B, 1}, PinMode::ANALOG_INPUT); 
    std::cout << "-> PB1 configured as ANALOG_INPUT (AFEC1_CH0).\n";

    return 0;
}