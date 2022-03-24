/*
    RockBLOCK Machine Flow Control State Implementation
    
    Send a cancel flow control command with timeout
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine_state.hpp"

#define ATK0 "AT&K0\r"
#define ATK0_TIMEOUT 10000  // message timeout (ms)

// Setup a singleton
rock_machine_state* rock_machine_flowctrl_state::_instance = 0;

rock_machine_state* rock_machine_flowctrl_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_flowctrl_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_flowctrl_state::send(rock_machine* rock) {
    
    // Set timeout
    rock->_timeout_id = add_alarm_in_ms(ATK0_TIMEOUT, alarm_callback, NULL, false);

    // send the message
    // uart_puts(RB_UART_ID, ATK0);
    rock->write(ATK0);
    puts(ATK0);

    change_state(rock, rock_machine_flowctrl_wait_state::instance());
}
