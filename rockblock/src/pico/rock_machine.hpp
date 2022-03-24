/*
    RockBLOCK Machine Class Interface

    Uses the State pattern where each state is a Singleton
    implementing that states behaviour
  
*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef rock_machine_hpp
#define rock_machine_hpp

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/serial.hpp"
#include "rock_machine_state.hpp"

#define MAX_MESSAGE_SIZE 50
#define BAD_TIMEOUT 2000 // ms
#define GOOD_TIMEOUT 2000 // ms

int64_t alarm_callback(alarm_id_t id, void *user_data);

bool get_response(char* response_buf, const char* string);

class rock_machine_state;

class rock_machine {
    public:
        alarm_id_t _timeout_id;
        int message_count = 1;
        const char* message;
        rock_machine(serial& uart);
        void start();        
        void send();
        void send_ok(char*);
        void repeat(); 
        void run();
        uint get_state_id();
        void write(const char* s);         
    private:
        friend class rock_machine_state;
        void change_state(rock_machine_state*);       
    private:
        rock_machine_state* _state;
        serial& _serial;
};
#endif