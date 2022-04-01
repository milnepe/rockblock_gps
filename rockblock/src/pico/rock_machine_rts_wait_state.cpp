/*
    RockBLOCK Machine Ready To Send Wait State Implementation

    Waits for AT command response - keeps trying until 
    RockBLOCK is ready to send
    
*/

#include "rock_machine_state.hpp"

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
    // if(get_response(response, "OK") == ISBD_OK) {
    if(get_response(response) == ISBD_OK) {        
        cancel_alarm(rock->_timeout_id);
        change_state(rock, rock_machine_flowctrl_state::instance());
    } 
}

// Return to RTS state on timeout - this is not necessarily an error
// the RockBLOCK may not be ready yet, so keep trying
void rock_machine_rts_wait_state::repeat(rock_machine* rock) {
    puts("RockBLOCK not ready");
    change_state(rock, rock_machine_rts_state::instance());
}
