/*
    RockBLOCK Machine Send Bad State Implementation

    Indicates successful transmission

*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state *rock_machine_sendgood_wait_state::_instance = 0;

rock_machine_state *rock_machine_sendgood_wait_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_sendgood_wait_state;
    }
    return _instance;
}

// Set timeout to change state
void rock_machine_sendgood_wait_state::send(rock_machine *rock)
{
    puts(rock->get_state());
    rock->_timeout_id = add_alarm_in_ms(2000, alarm_callback, NULL, false);
}

// Get the OK
void rock_machine_sendgood_wait_state::send_ok(rock_machine *rock, char *response)
{
    if (get_response(response) == ISBD_OK)
    {
        ; // Just get the ack
    }
}

// Change to next state
void rock_machine_sendgood_wait_state::repeat(rock_machine *rock)
{
    change_state(rock, rock_machine_idle_wait_state::instance());
}
