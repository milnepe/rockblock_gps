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

#define ISBD_ERROR 0
#define ISBD_OK 1
#define ISBD_SENT 2
#define ISBD_NOT_SENT 3
#define ISBD_MAIL 4
#define ISBD_EMPTY 5

// #define BAD_TIMEOUT 2000 // ms
// #define GOOD_TIMEOUT 2000 // ms

int64_t alarm_callback(alarm_id_t id, void *user_data);

int get_response(const uint8_t* response_buf);
// int get_response(char* response_buf, const char* string);

class rock_machine_state;

class rock_machine {
    public:
        alarm_id_t _timeout_id;
        // const char* message;

        rock_machine(serial& uart, char* message);
        void start();        
        void send();
        void send_ok(uint8_t* response);
        void repeat(); 
        void run();
        uint get_state_id();
        const char* get_state();
        void write(const char* s);
        const char* get_message();
        void inc_mail_count();
        void dec_mail_count();
        uint get_mail_count();
    private:
        friend class rock_machine_state;
        void change_state(rock_machine_state*);      
    private:
        rock_machine_state* _state;
        serial& _serial;
        char* _message;
        uint _mail_count = 1;
};
#endif