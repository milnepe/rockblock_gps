/*
    RockBLOCK GPS Client
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef rockblock_gps_hpp
#define rockblock_gps_hpp

#include "pico/stdlib.h"

#define UART0_ID uart0
#define UART1_ID uart1
#define RB_UART_ID UART1_ID
#define RB_BAUD_RATE 19200
#define GPS_BAUD_RATE 9600

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_TX_PIN 4
#define UART1_RX_PIN 5

#define OUTPUT_INTERVAL 5000 // ms

#define BUTTON_PIN 15
#define LED_AMBER 14
#define LED_RED 16
#define LED_GREEN 18
#define LED_PIN PICO_DEFAULT_LED_PIN


const char* construct_message(char *message_buf, char* message, int message_count);

void led_init();

void switch_led(uint state);

#endif
