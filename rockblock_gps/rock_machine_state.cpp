/*
    RockBLOCK Machine State Implementation

  
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rock_machine.hpp"

// Change the machine state
void rock_machine_state::change_state(rock_machine* rock, rock_machine_state* state) {
    rock->change_state(state);
}
