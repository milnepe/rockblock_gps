/*
    RockBLOCK Machine Flow Control State Implementation

    Turn off flow control
*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state *rock_machine_flowctrl_state::_instance = 0;

rock_machine_state *rock_machine_flowctrl_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_flowctrl_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_flowctrl_state::send(rock_machine *rock)
{
    puts(rock->get_state());

    rock->write("AT&K0\r");

    rock->_timeout_id = add_alarm_in_ms(1000, alarm_callback, NULL, false);
}

// Change to next state
void rock_machine_flowctrl_state::send_ok(rock_machine *rock, uint8_t *response)
{
    if (get_response(response) == ISBD_OK)
    {
        cancel_alarm(rock->_timeout_id);
        change_state(rock, rock_machine_mobuffer_state::instance());
    }
}

// Change to error state
void rock_machine_flowctrl_state::repeat(rock_machine *rock)
{
    puts("Cancel flowcontrol timeout");
    change_state(rock, rock_machine_sendbad_wait_state::instance());
}
