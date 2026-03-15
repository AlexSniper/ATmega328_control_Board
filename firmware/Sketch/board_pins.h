#pragma once
#include <Arduino.h>

// ------------------------------------------------------------
// Al_Robotics — ATmega328PB @ 16 MHz (MiniCore pin mapping)
// ------------------------------------------------------------
// UART0 (PD0/PD1) <-> ATmega16U2 USB bridge
constexpr uint8_t PIN_RX0 = 0;    // PD0 (USART0 RXD)
constexpr uint8_t PIN_TX0 = 1;    // PD1 (USART0 TXD)

// I2C bus (Wire)
constexpr uint8_t PIN_SDA = A4;   // PC4 (SDA)
constexpr uint8_t PIN_SCL = A5;   // PC5 (SCL)

// MCP23017 interrupt lines -> ATmega external interrupts
constexpr uint8_t PIN_INT_A = 2;  // PD2 / INT0  (MCP INTA)
constexpr uint8_t PIN_INT_B = 3;  // PD3 / INT1  (MCP INTB)

// RS-485 enable control (DE and /RE are tied together on the PCB)
constexpr uint8_t PIN_RS485_DE_RE = 4;  // PD4 drives MAX485 pins 2+3 (DE & /RE)

// ------------------------------------------------------------
// Notes (for reference, no defines needed here):
// * MAX485 DI  (pin 4)  -> ATmega328PB PE1 (USART1 TXD)  == Serial1 TX
// * MAX485 RO  (pin 1)  -> ATmega328PB PE0 (USART1 RXD)  == Serial1 RX
// * main.cpp selects UART at runtime (RS485UART 0|1), default = Serial1.
//
// Make sure MiniCore is set to ATmega328PB so Serial1 is available.
// ------------------------------------------------------------
