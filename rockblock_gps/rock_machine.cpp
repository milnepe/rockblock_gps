/*
    RockBLOCK Machine Class Definition
    
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "rock_machine.hpp"


volatile bool timeout_fired = false;

// Setup a timeout callback
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    puts("Timer!");
    timeout_fired = true;
    return 0;
}

rock_machine::rock_machine() {
    // Starting state
    _state = rock_machine_idle_wait_state::instance(); 
}

void rock_machine::run() {
    send();

    if(timeout_fired) {
        timeout_fired = false;
        repeat();
    }
}

void rock_machine::start() {
    _state->start(this);
}

void rock_machine::send() {
    _state->send(this);
}        
void rock_machine::send_ok(char* response) {
    _state->send_ok(this, response);
}
void rock_machine::repeat() {
    _state->repeat(this);
}

void rock_machine::change_state(rock_machine_state* state) {
    _state = state;
}

uint rock_machine::get_state_id() {
    return _state->get_state_id();
}

// Parse a response
bool get_response(char* response_buf, const char* string) {
    if(strstr(response_buf, string) != NULL) {
        return true;    
    }
    return false;
}