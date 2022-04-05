/*
    RockBLOCK Machine Ready To Send State Implementation

    Send an AT command to check if modem is ready

*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state *rock_machine_rts_state::_instance = 0;

rock_machine_state *rock_machine_rts_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_rts_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_rts_state::send(rock_machine *rock)
{
    puts(rock->get_state());

    // Output message to be sent
    puts("Message:");
    puts(rock->get_message());

    rock->write("AT\r");

    rock->_timeout_id = add_alarm_in_ms(1000, alarm_callback, NULL, false);
}

// Change to next state
void rock_machine_rts_state::send_ok(rock_machine *rock, char *response)
{
    if (get_response(response) == ISBD_OK)
    {
        cancel_alarm(rock->_timeout_id);
        change_state(rock, rock_machine_flowctrl_state::instance());
    }
}

// Return to idle state on timeout - this is not necessarily an error
// the RockBLOCK may not be ready yet, so keep trying
void rock_machine_rts_state::repeat(rock_machine *rock)
{
    puts("RockBLOCK not ready");
    change_state(rock, rock_machine_idle_wait_state::instance());
}
