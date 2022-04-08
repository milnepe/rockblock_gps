/*
    RockBLOCK Machine State Implementation


*/

#include "rock_machine_state.hpp"

void rock_machine_state::start(rock_machine *) {}
void rock_machine_state::send(rock_machine *) {}
void rock_machine_state::send_ok(rock_machine *, char *response) {}
void rock_machine_state::repeat(rock_machine *) {}

// Change the machine state
void rock_machine_state::change_state(rock_machine *rock, rock_machine_state *state)
{
    rock->change_state(state);
}
