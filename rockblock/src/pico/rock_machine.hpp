/*
    RockBLOCK Machine Class Interface

    Uses the State pattern where each state is a Singleton
    implementing that states behaviour
  
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


#define BAD_TIMEOUT 2000 // ms
#define GOOD_TIMEOUT 2000 // ms

#define MO_BUFFER_SIZE 64  // Absolute max 340 bytes

int64_t alarm_callback(alarm_id_t id, void *user_data);

bool get_response(char* response_buf, const char* string);

class rock_machine_state;

class rock_machine {
    public:
        alarm_id_t _timeout_id;
        // const char* message;

        rock_machine(serial& uart, char* message);
        void start();        
        void send();
        void send_ok(char*);
        void repeat(); 
        void run();
        uint get_state_id();
        void write(const char* s);
        const char* get_message();
        uint get_message_count();
    private:
        friend class rock_machine_state;
        void change_state(rock_machine_state*);      
    private:
        rock_machine_state* _state;
        serial& _serial;
        char* _message;
        uint _message_count = 1;
};
#endif