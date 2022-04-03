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

// Send command, set timeout and change to next state
void rock_machine_sendgood_wait_state::send(rock_machine *rock)
{
    puts(state_str[this->_state_id]);

        puts("Message sent");
        // Increament message counter
        // rock->inc_message_count();
    // Let it time out to allow RockBLOCK to re-charge
    rock->_timeout_id = add_alarm_in_ms(2000, alarm_callback, NULL, false);
}

// Change to next state on timeout
void rock_machine_sendgood_wait_state::repeat(rock_machine* rock) {
    change_state(rock, rock_machine_idle_wait_state::instance()); 
}
