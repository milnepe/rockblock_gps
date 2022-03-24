/*
    RockBLOCK Machine Idle Wait State Implementation

    Waits for start trigger
    
*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state* rock_machine_idle_wait_state::_instance = 0;

rock_machine_state* rock_machine_idle_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_idle_wait_state;
    }
    return _instance;
}

// Change to next state on start signal
void rock_machine_idle_wait_state::start(rock_machine* rock) {
    change_state(rock, rock_machine_rts_state::instance()); 
}
