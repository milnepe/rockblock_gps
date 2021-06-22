/*
    RockBLOCK Machine Ready To Send Wait State Implementation

    Waits for AT command response - keeps trying until 
    RockBLOCK is ready to send
    
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine.hpp"

// Setup a singleton
rock_machine_state* rock_machine_rts_wait_state::_instance = 0;

rock_machine_state* rock_machine_rts_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_rts_wait_state;
    }
    return _instance;
}

// Change to next state
void rock_machine_rts_wait_state::send_ok(rock_machine* rock, char* response) {
    if(get_response(response, "OK")) {
        cancel_alarm(rock->_timeout_id);
        puts(response);                              
        change_state(rock, rock_machine_flowctrl_state::instance());
    } 
}

// Go back to state on timeout
void rock_machine_rts_wait_state::repeat(rock_machine* rock) {
    change_state(rock, rock_machine_rts_state::instance());
}
