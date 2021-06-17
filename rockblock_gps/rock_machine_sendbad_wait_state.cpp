/*
    RockBLOCK Machine Send Bad State Implementation

    Indicates failure conditions
    
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine.hpp"

// Setup a singleton
rock_machine_state* rock_machine_sendbad_wait_state::_instance = 0;

rock_machine_state* rock_machine_sendbad_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_sendbad_wait_state;
    }
    return _instance;
}

// Change to next state on timeout
void rock_machine_sendbad_wait_state::repeat(rock_machine* rock) {
    change_state(rock, rock_machine_idle_wait_state::instance()); 
}
