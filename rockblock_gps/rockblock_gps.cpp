/*
    RockBLOCK GPS Client Implementation
    
    Reads GPS module and sends position over satellite link
    

*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "rockblock_gps.hpp"
#include "rock_machine.hpp"
#include "gps_machine.hpp"
#include "pico/serial.hpp"


#define SENTENCE_SIZE 128
#define RESPONSE_SIZE 32
#define MAX_MESSAGE_COUNT_SIZE 4
#define NMEA_CHARACTERS 44

static char rd_message_buf[MAX_MESSAGE_SIZE];
static char rb_response_buf[RESPONSE_SIZE];
static int rd_chars_rxed = 0;

static char gps_sentence[SENTENCE_SIZE];
static char gps_buffer[SENTENCE_SIZE];
static int gps_chars_rxed = 0;
volatile bool gps_updated = false;

bool on = true;

void on_uart1_rx();
void gpio_callback(uint gpio, uint32_t events);

bool led_repeating_timer_callback(struct repeating_timer *t) {
    gpio_put(LED_PIN, on);
    on = !on;
    return true;
}

// Create a RockBLOCK machine
rock_machine rock;

int main()
{
    stdio_init_all();

    serial serial0;

    // Initialise uart0 for use with GPS on default GPIO
    // TX GP0, RX GP1
    serial0.init(GPS_BAUD_RATE);

    // Setup RockBLOCK (uart 1)
    uart_init(UART1_ID, RB_BAUD_RATE);
    gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);

    // Turn off FIFO's
    uart_set_fifo_enabled(UART1_ID, false);
    int UART_IRQ = UART1_ID == uart1 ? UART1_IRQ : UART0_IRQ; 

    // Set up a RX interrupt uart 1
    irq_set_exclusive_handler(UART_IRQ, on_uart1_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART1_ID, true, false); 
    // End RockBLOCK uart setup

    struct repeating_timer led_timer;  // On-board LED heartbeat
    add_repeating_timer_ms(500, led_repeating_timer_callback, NULL, &led_timer);

    static uint previous_state = MOBUFFER;

    led_init(); 

    puts("RockBLOCK GPS test\r\n");

    sleep_ms(10000);  // let GPS / RockBLOCK warm up

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);  

    while(1) {

        // Read GPS stream
        while (serial0.uart_is_readable()) {
            char ch = serial0.uart_getc();
            gps_sentence[gps_chars_rxed++] = ch;
            if (ch == '\n' || gps_chars_rxed == SENTENCE_SIZE - 1) {
                gps_sentence[gps_chars_rxed] = '\0';
                gps_chars_rxed = 0;
                get_GPRMC(gps_sentence, gps_buffer);
            }
        }

        rock.message = construct_message(rd_message_buf, gps_buffer, rock.message_count);        

        // Send a message
        rock.run();

        // Get state for LED's and USB debug output
        volatile uint current_state = rock.get_state_id();
        if (current_state != previous_state) {
            puts(state_str[current_state]);
            switch_led(current_state);
        }
        previous_state = current_state;

        // // Put anything else non-blocking to be run here
        // puts("Do other stuff...\r\n");


    }
}

// Button callback handler - starts a new message session
void gpio_callback(uint gpio, uint32_t events) {
    rock.start();
}

// RockBLOCK RX interrupt handler
void on_uart1_rx() {
    while (uart_is_readable(UART1_ID)) {
        char ch = uart_getc(UART1_ID);
        rb_response_buf[rd_chars_rxed++] = ch;
        if (ch == '\r' || rd_chars_rxed == RESPONSE_SIZE - 1) {
            rd_chars_rxed = 0;
            rock.send_ok(rb_response_buf);
            memset(rb_response_buf, 0, sizeof rb_response_buf);
        }
    }
}

const char* construct_message(char *message_buf, char* message, int message_count) {
    char message_count_buf[MAX_MESSAGE_COUNT_SIZE] = {'\0'};
    itoa(message_count, message_count_buf, 10);  // base 10
    strcpy(message_buf, message_count_buf);
    strcat(message_buf, ",");
    size_t n = NMEA_CHARACTERS;
    if(n < MAX_MESSAGE_SIZE - strlen(message_buf)) {
        strncat(message_buf, message, n);  // must truncate message before \r\n
    }
    else {
        puts("Max message length exceeded!");
    }
}

void led_init() {
    gpio_init(LED_PIN);   
    gpio_init(LED_RED);
    gpio_init(LED_AMBER);
    gpio_init(LED_GREEN);

    gpio_set_dir(LED_PIN, GPIO_OUT); 
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_AMBER, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_pull_up(BUTTON_PIN);

}

void switch_led(uint state) {
    switch(state) {
        case IDLE:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case READYTOSEND:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case READYTOSENDWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;                        
        case FLOWCTRL:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case FLOWCTRLWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;            
        case MOBUFFER:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case MOBUFFERWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;
        case SENDGOODWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;
        case SENDBADWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 1);
            gpio_put(LED_AMBER, 0);
            break;                                         
    }
}
