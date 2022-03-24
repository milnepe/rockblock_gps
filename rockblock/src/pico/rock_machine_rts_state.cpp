/*
    RockBLOCK Machine Ready To Send State Implementation

    Send an AT command with timeout
    
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine_state.hpp"

#define RTS_CMD "AT\r"
#define RTS_TIMEOUT 10000  // RTS timeout (ms)

// Setup a singleton
rock_machine_state* rock_machine_rts_state::_instance = 0;

rock_machine_state* rock_machine_rts_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_rts_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_rts_state::send(rock_machine* rock) {

    // set a timeout
    rock->_timeout_id = add_alarm_in_ms(RTS_TIMEOUT, alarm_callback, NULL, false);

    // send the message
    // uart_puts(RB_UART_ID, RTS_CMD);
    rock->write(RTS_CMD);
    puts(RTS_CMD);

    // Change to next state    
    change_state(rock, rock_machine_rts_wait_state::instance()); 

}
