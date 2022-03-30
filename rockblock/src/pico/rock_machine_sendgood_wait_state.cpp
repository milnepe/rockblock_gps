/*
    RockBLOCK Machine Send Bad State Implementation

    Indicates successful transmission
    
*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state* rock_machine_sendgood_wait_state::_instance = 0;

rock_machine_state* rock_machine_sendgood_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_sendgood_wait_state;
    }
    return _instance;
}

// Change to next state on timeout
void rock_machine_sendgood_wait_state::repeat(rock_machine* rock) {
    // Increament message counter
    rock->inc_message_count();

    change_state(rock, rock_machine_idle_wait_state::instance()); 
}
