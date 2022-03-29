/*
    RockBLOCK Machine Flow Control State Implementation
    
    Turn off flow control
*/

#include "rock_machine_state.hpp"

#define ATK0_CMD "AT&K0\r"
#define ATK0_TIMEOUT 10000  // message timeout (ms)

// Setup a singleton
rock_machine_state* rock_machine_flowctrl_state::_instance = 0;

rock_machine_state* rock_machine_flowctrl_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_flowctrl_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_flowctrl_state::send(rock_machine* rock) {
    
    // Set timeout
    rock->_timeout_id = add_alarm_in_ms(ATK0_TIMEOUT, alarm_callback, NULL, false);

    // send the message
    puts(ATK0_CMD);
    rock->write(ATK0_CMD);

    change_state(rock, rock_machine_flowctrl_wait_state::instance());
}
