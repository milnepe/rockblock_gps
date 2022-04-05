#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "pico/stdlib.h"
#include "string.h"
#include "hardware/uart.h"

class serial {
private:
    uart_inst* _uart;
    uint _tx_pin;
    uint _rx_pin;
public:
    // Default to uart0 wit TX on GP0 and RX on GP1
    serial(uart_inst* uart = uart0, uint tx = 0, uint rx = 1) {
        _uart = uart;
        _tx_pin = tx;
        _rx_pin = rx;
    }

    // Initialise UART with default baud rate
    // Must be called as first method after constructing serial object     
    void init(uint baudrate = 115200);

    // Return the uart in use
    uart_inst* get_uart() {
        return _uart;
    }

    /*** Wrappers round pico-sdk hardware uart functions ***/

    uint uart_init(uint baudrate) {
        return ::uart_init(_uart, baudrate);        
    }

    bool uart_is_readable() {
        return ::uart_is_readable(_uart);
    }
    uint8_t uart_getc() {
        return ::uart_getc(_uart);
    }
    bool uart_is_writable() {
        return ::uart_is_writable(_uart);        
    }

    // Set UART flow control CTS/RTS
    void uart_set_hw_flow(bool cts, bool rts) {
        ::uart_set_hw_flow(_uart, cts, rts);        
    }

    // Set data format    
    void uart_set_format(uint data_bits, uint stop_bits, uart_parity_t parity) {
        ::uart_set_format(_uart, data_bits, stop_bits, parity);        
    }
    
    void uart_set_fifo_enabled(bool enabled) {
        ::uart_set_fifo_enabled(_uart, enabled);        
    }
    
    void uart_set_pin(uint pin) {
        ::gpio_set_function(pin, GPIO_FUNC_UART);
    }

    // Return the actual baud rate selected, which will be as close as
    // possible to that requested    
    int uart_set_baudrate(uint baudrate) {
        return ::uart_set_baudrate(_uart, baudrate);  
    }

    void uart_putc_raw(char ch) {
        ::uart_putc_raw(_uart, ch);        
    }

    void uart_puts(const char* s) {
        ::uart_puts(_uart, s);
    }
        
    void uart_write_blocking(const uint8_t* src, size_t len) {
        ::uart_write_blocking(_uart, src, len);
    }                  
};
#endif