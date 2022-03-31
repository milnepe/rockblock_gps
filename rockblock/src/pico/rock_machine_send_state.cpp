/*
    RockBLOCK Machine Send State Implementation
    
    Start an extended SBD session
*/

#include "rock_machine_state.hpp"

#define SBDIX "AT+SBDIX\r"
// #define SBDIX_TIMEOUT 10000  // session timeout (ms)
#define SBDIX_TIMEOUT 60000  // production session timeout (ms)


// Setup a singleton
rock_machine_state* rock_machine_send_state::_instance = 0;

rock_machine_state* rock_machine_send_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_send_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_send_state::send(rock_machine* rock) {
    
    // Set timeout
    rock->_timeout_id = add_alarm_in_ms(SBDIX_TIMEOUT, alarm_callback, NULL, false);

    // send the message
    puts(SBDIX);
    rock->write(SBDIX);


    change_state(rock, rock_machine_send_wait_state::instance());
}
