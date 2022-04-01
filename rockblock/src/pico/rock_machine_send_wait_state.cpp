/*
    RockBLOCK Machine Send Wait State Implementation
    
    Wait for a MO transmission acknowledgement

*/

#include <stdlib.h>
#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state* rock_machine_send_wait_state::_instance = 0;

rock_machine_state* rock_machine_send_wait_state::instance() {
    if(_instance == 0) {
        _instance = new rock_machine_send_wait_state;
    }
    return _instance;
}

// Change to next state
void rock_machine_send_wait_state::send_ok(rock_machine* rock, char* response) {
    cancel_alarm(rock->_timeout_id); 
    uint16_t res = get_response(response);  
    char c[8];
    puts(itoa(res, c, 10));
    if(res == ISBD_SENT) {       
        // Increament message counter
        // rock->inc_message_count();                 
        rock->_timeout_id = add_alarm_in_ms(GOOD_TIMEOUT, alarm_callback, NULL, false);                       
        change_state(rock, rock_machine_sendgood_wait_state::instance());
    } else {
        if(res == ISBD_NOT_SENT) {    
            rock->_timeout_id = add_alarm_in_ms(BAD_TIMEOUT, alarm_callback, NULL, false);                                                 
            change_state(rock, rock_machine_sendbad_wait_state::instance());
        }
    }
}

// Go back to RTS state on timeout
void rock_machine_send_wait_state::repeat(rock_machine* rock) {
    // set a timeout
    rock->_timeout_id = add_alarm_in_ms(BAD_TIMEOUT, alarm_callback, NULL, false);
    puts("Transmission timed out");
    change_state(rock, rock_machine_sendbad_wait_state::instance());    
}
