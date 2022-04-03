/*
    RockBLOCK Machine Send State Implementation

    Start an extended SBD session
*/

#include "rock_machine_state.hpp"

// Setup a singleton
rock_machine_state *rock_machine_send_state::_instance = 0;

rock_machine_state *rock_machine_send_state::instance()
{
    if (_instance == 0)
    {
        _instance = new rock_machine_send_state;
    }
    return _instance;
}

// Send command, set timeout and change to next state
void rock_machine_send_state::send(rock_machine *rock)
{
    puts(rock->get_state());

    // Longer timeout to allow for satellite transmission
    rock->_timeout_id = add_alarm_in_ms(60000, alarm_callback, NULL, false);

    rock->write("AT+SBDIX\r");
}

// Change to next state
void rock_machine_send_state::send_ok(rock_machine *rock, uint8_t *response)
{
    cancel_alarm(rock->_timeout_id);
    uint16_t res = get_response(response);
    printf("%u\n", res);
    if (res == ISBD_SENT)
    {
        change_state(rock, rock_machine_sendgood_wait_state::instance());
    }
    else
    {
        if (res == ISBD_NOT_SENT)
        {
            change_state(rock, rock_machine_sendbad_wait_state::instance());
        }
    }
}

// Change to error state
void rock_machine_send_state::repeat(rock_machine *rock)
{
    puts("Transmission timed out");
    change_state(rock, rock_machine_sendbad_wait_state::instance());
}
