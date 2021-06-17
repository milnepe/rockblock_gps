/*
    RockBLOCK Machine Send State Implementation
    
    Send start extended session command with timeout
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine.hpp"

#define SBDIX "AT+SBDIX\r"
// #define SBDIX_TIMEOUT 10000  // session timeout (ms)
#define SBDIX_TIMEOUT 60000  // production session timeout (ms)


// Setup a singleton
rock_machine_state* rock_machine_send_state::_instance = 0;

rock_machine_state* rock_machine_send_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_send_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_send_state::send(rock_machine* rock) {
    
    // Set timeout
    rock->_timeout_id = add_alarm_in_ms(SBDIX_TIMEOUT, alarm_callback, NULL, false);

    // send the message
    uart_puts(RB_UART_ID, SBDIX);

    change_state(rock, rock_machine_send_wait_state::instance());
}