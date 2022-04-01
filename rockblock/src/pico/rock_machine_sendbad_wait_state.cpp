/*
    RockBLOCK Machine Send Bad State Implementation

    Indicates failure conditions
    
*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state* rock_machine_sendbad_wait_state::_instance = 0;

rock_machine_state* rock_machine_sendbad_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_sendbad_wait_state;
    }
    return _instance;
}

void rock_machine_sendbad_wait_state::send_ok(rock_machine* rock, char* response) {
    if(get_response(response) == ISBD_OK) {
        // Let it timeout                 
    }
}

// Change to next state on timeout
void rock_machine_sendbad_wait_state::repeat(rock_machine* rock) {
    puts("Message Failure");      
    change_state(rock, rock_machine_idle_wait_state::instance()); 
}
