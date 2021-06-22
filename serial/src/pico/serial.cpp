#include "serial.hpp"

// Initialise a serial port using defaults or
// values passed in from constructor
void serial::init(uint baudrate) {
    uart_init(baudrate);
    uart_set_pin(_tx_pin);
    uart_set_pin(_rx_pin);
    // Flow control off
    uart_set_hw_flow(false, false);
    // 8-bit, 1 stop-bit, no parity
    uart_set_format(8U, 1U, UART_PARITY_NONE);        
}