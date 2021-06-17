/*
    RockBLOCK Machine MO Buffer State Implementation

    Send load MO buffer command with message payload and set timeout
    
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine.hpp"

#define SBDWT "AT+SBDWT="
#define SBDWT_TIMEOUT 10000  // message timeout (ms)

// Setup a singleton
rock_machine_state* rock_machine_mobuffer_state::_instance = 0;

rock_machine_state* rock_machine_mobuffer_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_mobuffer_state;
    }
    return _instance;
}

// Send the message and return to Idle
void rock_machine_mobuffer_state::send(rock_machine* rock) {
    // Construct text message
    if(strlen(rock->message) < MAX_MESSAGE_SIZE) {
        char text_message[MAX_MESSAGE_SIZE + 10] = {0};
        strcpy(text_message, SBDWT);
        strcat(text_message, rock->message);
        strcat(text_message, "\r");
        puts(text_message);
        // Send the message
        uart_puts(RB_UART_ID, text_message);        
    }
    else {
        puts(rock->message);
        puts("Message length exceeded");  // Error condition STOP!!
    }

    // set a timeout
    rock->_timeout_id = add_alarm_in_ms(SBDWT_TIMEOUT, alarm_callback, NULL, false);

    // Change to next state
    change_state(rock, rock_machine_mobuffer_wait_state::instance()); 
}