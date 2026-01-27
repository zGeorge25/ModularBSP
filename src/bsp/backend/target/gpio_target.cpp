#include "gpio.h"
#include <cstdint>

// === Implementare pentru ATSAMV71 (Hardware Real) ===

namespace bsp {

// === PMC (Power Management Controller) ===
#define PMC_BASE_ADDR 0x400E0600

struct PmcRegs {
    uint32_t reserved[4];            // 0x00 - 0x0C
    volatile uint32_t PCER0;         // 0x10: Peripheral Clock Enable Register 0
};

// === MATRIX (System Configuration) ===
#define MATRIX_BASE_ADDR 0x40088000

struct MatrixRegs {
    uint32_t reserved[69];           // 0x000..0x113 (Padding)
    volatile uint32_t CCFG_SYSIO;    // 0x114: System I/O Configuration Register
};

// === PIO (Parallel Input/Output) ===
#define PIOA_BASE_ADDR 0x400E0E00 
#define PIOB_BASE_ADDR 0x400E1000
#define PIOC_BASE_ADDR 0x400E1200

struct PioPort {
    volatile uint32_t PER;           // 0x00: PIO Enable
    volatile uint32_t PDR;           // 0x04: PIO Disable
    uint32_t reserved1[2];           // 0x08 - 0x0C
    
    volatile uint32_t OER;           // 0x10: Output Enable
    volatile uint32_t ODR;           // 0x14: Output Disable
    uint32_t reserved2[6];           // 0x18 - 0x2C
    
    volatile uint32_t SODR;          // 0x30: Set Output Data
    volatile uint32_t CODR;          // 0x34: Clear Output Data
    uint32_t reserved3[1];           // 0x38 
    
    volatile uint32_t PDSR;          // 0x3C: Pin Data Status (Read)
    uint32_t reserved4[8];           // 0x40 - 0x5C 
    
    volatile uint32_t PUDR;          // 0x60: Pull-up Disable
    volatile uint32_t PUER;          // 0x64: Pull-up Enable
    uint32_t reserved5[2];           // 0x68 - 0x6C
    
    volatile uint32_t ABCDSR0;       // 0x70: Peripheral Select 0
    volatile uint32_t ABCDSR1;       // 0x74: Peripheral Select 1
    uint32_t reserved6[6];           // 0x78 - 0x8C
    
    volatile uint32_t PPDDR;         // 0x90: Pull-down Disable
    volatile uint32_t PPDER;         // 0x94: Pull-down Enable
    uint32_t reserved7[19];          // 0x98 - 0xE0
    
    volatile uint32_t WPMR;          // 0xE4: Write Protect Mode
};

// --- Helper Functions ---

static PioPort* getPort(Port port) {
    if (port == Port::A) return reinterpret_cast<PioPort*>(PIOA_BASE_ADDR);
    if (port == Port::B) return reinterpret_cast<PioPort*>(PIOB_BASE_ADDR);
    if (port == Port::C) return reinterpret_cast<PioPort*>(PIOC_BASE_ADDR);
    return nullptr;
}


void initSystem() {
    PmcRegs* pmc = reinterpret_cast<PmcRegs*>(PMC_BASE_ADDR);
    
    // Enable PIOA (ID 10), PIOB (ID 11), PIOC (ID 12)
    pmc->PCER0 = (1 << 10) | (1 << 11) | (1 << 12);

    // Matrix Configuration for PB12
    // SYSIO12 (Bit 12): 0 = ERASE, 1 = PB12
    MatrixRegs* matrix = reinterpret_cast<MatrixRegs*>(MATRIX_BASE_ADDR);
    matrix->CCFG_SYSIO |= (1 << 12);
}

void GpioDriver::configure(Pin pin, PinMode mode, PullMode pull) {
    PioPort* pio = getPort(pin.port);
    if (!pio) return;

    uint32_t mask = (1 << pin.index); 

    // 1. Configure Mode
    if (mode == PinMode::INPUT || mode == PinMode::OUTPUT) {
        // Enable PIO control
        pio->PER = mask; 
        
        if (mode == PinMode::OUTPUT) {
            pio->OER = mask;
        } else {
            pio->ODR = mask;
        }
    } else if (mode == PinMode::PERIPHERAL_A || mode == PinMode::PERIPHERAL_B) {
        // Disable PIO control (System Peripheral control)
        pio->PDR = mask; 
        
        // Multiplexing Logic:
        // Peripheral A: ABCDSR0=0, ABCDSR1=0
        // Peripheral B: ABCDSR0=1, ABCDSR1=0
        if (mode == PinMode::PERIPHERAL_A) {
             pio->ABCDSR0 &= ~mask;
             pio->ABCDSR1 &= ~mask;
        } else {
             pio->ABCDSR0 |= mask;
             pio->ABCDSR1 &= ~mask;
        }
    } else if (mode == PinMode::ANALOG_INPUT || mode == PinMode::ANALOG_OUTPUT) {
        // Analog Mode: Disable PIO, Disable Output , Disable Pull-ups/downs
        pio->PDR = mask;
        pio->ODR = mask;
        pio->PUDR = mask;
        pio->PPDDR = mask;
    }

    // 2. Configure Pull-Resistors
    if (pull == PullMode::UP) {
        pio->PUER = mask;
        pio->PPDDR = mask;
    } else if (pull == PullMode::DOWN) {
        pio->PPDER = mask;
        pio->PUDR = mask; 
    } else { // PullMode::NONE
        pio->PUDR = mask;
        pio->PPDDR = mask;
    }
}

void GpioDriver::set(Pin pin) {
    PioPort* pio = getPort(pin.port);
    if (pio) {
        pio->SODR = (1 << pin.index);
    }
}

void GpioDriver::clear(Pin pin) {
    PioPort* pio = getPort(pin.port);
    if (pio) {
        pio->CODR = (1 << pin.index);
    }
}

void GpioDriver::toggle(Pin pin) {
    bool state = read(pin);
    if (state) clear(pin);
    else set(pin);
}

bool GpioDriver::read(Pin pin) {
    PioPort* pio = getPort(pin.port);
    if (pio) {
        return (pio->PDSR & (1 << pin.index)) != 0;
    }
    return false;
}

}