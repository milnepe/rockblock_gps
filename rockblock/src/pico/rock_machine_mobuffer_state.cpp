/*
    RockBLOCK Machine MO Buffer State Implementation

    Load MO buffer with ascii text message

*/

#include "rock_machine.hpp"

// Setup a singleton
rock_machine_state *rock_machine_mobuffer_state::_instance = 0;

rock_machine_state *rock_machine_mobuffer_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_mobuffer_state;
    }
    return _instance;
}

// Load MO buffer
void rock_machine_mobuffer_state::send(rock_machine *rock)
{
    puts(rock->get_state());

    rock->write("AT+SBDWT=");
    rock->write(rock->get_message());
    rock->write("\r");

    rock->_timeout_id = add_alarm_in_ms(1000, alarm_callback, NULL, false);
}

// Change to next state
void rock_machine_mobuffer_state::send_ok(rock_machine *rock, char *response)
{
    if (get_response(response) == ISBD_OK)
    {
        cancel_alarm(rock->_timeout_id);
        change_state(rock, rock_machine_send_state::instance());
    }
}

// Change to error state
void rock_machine_mobuffer_state::repeat(rock_machine *rock)
{
    puts("Cancel flowcontrol timeout");
    change_state(rock, rock_machine_sendbad_wait_state::instance());
}