/*
    RockBLOCK Machine MO Buffer Wait State Implementation

    Wait for MO buffer load acknowledgement

    Return via failure state on error
    
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine.hpp"

// Setup a singleton
rock_machine_state* rock_machine_mobuffer_wait_state::_instance = 0;

rock_machine_state* rock_machine_mobuffer_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_mobuffer_wait_state;
    }
    return _instance;
}

// Change to next state
void rock_machine_mobuffer_wait_state::send_ok(rock_machine* rock, char* response) {
    if(get_response(response, "OK")) {
        cancel_alarm(rock->_timeout_id);        
        puts(response); 
        change_state(rock, rock_machine_send_state::instance());        
    }   
}

// Change to Bad Wait state on timeout
void rock_machine_mobuffer_wait_state::repeat(rock_machine* rock) {
    // set a timeout
    rock->_timeout_id = add_alarm_in_ms(BAD_TIMEOUT, alarm_callback, NULL, false);

    puts("Bad MO Buffer load opp");    

    change_state(rock, rock_machine_sendbad_wait_state::instance());    
}
