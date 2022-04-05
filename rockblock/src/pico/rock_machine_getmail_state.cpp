/*
    RockBLOCK Machine Getmail State Implementation

    Retreive MT Text session
*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state *rock_machine_getmail_state::_instance = 0;

rock_machine_state *rock_machine_getmail_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_getmail_state;
    }
    return _instance;
}

// Send command and set timeout
void rock_machine_getmail_state::send(rock_machine *rock)
{
    puts(rock->get_state());

    rock->_timeout_id = add_alarm_in_ms(2000, alarm_callback, NULL, false);

    rock->write("AT+SBDRT\r");
}

// Change to next state
void rock_machine_getmail_state::send_ok(rock_machine *rock, char *response)
{
    if (get_response(response) == ISBD_OK)
    {
        cancel_alarm(rock->_timeout_id);
        change_state(rock, rock_machine_sendgood_wait_state::instance());
    }
}

// Change to error state
void rock_machine_getmail_state::repeat(rock_machine *rock)
{
    puts("Getmail timed out");
    change_state(rock, rock_machine_sendbad_wait_state::instance());
}
