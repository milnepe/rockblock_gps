/*
    RockBLOCK Machine Send Bad State Implementation

    Catch-all for failure conditions

*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state *rock_machine_sendbad_wait_state::_instance = 0;

rock_machine_state *rock_machine_sendbad_wait_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_sendbad_wait_state;
    }
    return _instance;
}

// Set timeout
void rock_machine_sendbad_wait_state::send(rock_machine *rock)
{
    puts(rock->get_state());

    // Let it time out so we can see error state
    rock->_timeout_id = add_alarm_in_ms(2000, alarm_callback, NULL, false);
}

// Get the OK
void rock_machine_sendbad_wait_state::send_ok(rock_machine *rock, char *response)
{
    if (get_response(response) == ISBD_OK)
    {
        ; // Just get the ack
    }
}

// Change to idle
void rock_machine_sendbad_wait_state::repeat(rock_machine *rock)
{
    change_state(rock, rock_machine_idle_wait_state::instance());
}
