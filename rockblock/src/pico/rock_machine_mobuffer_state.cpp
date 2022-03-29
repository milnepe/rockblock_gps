/*
    RockBLOCK Machine MO Buffer State Implementation

    Load MO buffer with ascii text message
    
*/

#include "rock_machine.hpp"

#define SBDWT_CMD "AT+SBDWT="
#define SBDWT_TIMEOUT 10000  // message timeout (ms)

// Setup a singleton
rock_machine_state* rock_machine_mobuffer_state::_instance = 0;

rock_machine_state* rock_machine_mobuffer_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_mobuffer_state;
    }
    return _instance;
}

// Load MO buffer
void rock_machine_mobuffer_state::send(rock_machine* rock) {

    puts(SBDWT_CMD);
    rock->write(SBDWT_CMD);
    rock->write(rock->get_message());
    rock->write("\r");

    // set a timeout
    rock->_timeout_id = add_alarm_in_ms(SBDWT_TIMEOUT, alarm_callback, NULL, false);

    // Change to next state
    change_state(rock, rock_machine_mobuffer_wait_state::instance()); 
}
