/*
    RockBLOCK Machine MO Buffer State Implementation

    Send load MO buffer command with message payload and set timeout
    
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
    // Construct cmd string
    char mo_buffer[MO_BUFFER_SIZE];
    if (sizeof(mo_buffer) > strlen(rock->get_message()) + sizeof(SBDWT_CMD) + 1) {
        strcpy(mo_buffer, SBDWT_CMD);
        strcat(mo_buffer, rock->get_message());
        strcat(mo_buffer, "\r");
 
        // Send the message
        puts(mo_buffer); 
        rock->write(mo_buffer);
    } else {
        puts("Message too long");
    }

    // set a timeout
    rock->_timeout_id = add_alarm_in_ms(SBDWT_TIMEOUT, alarm_callback, NULL, false);

    // Change to next state
    change_state(rock, rock_machine_mobuffer_wait_state::instance()); 
}
