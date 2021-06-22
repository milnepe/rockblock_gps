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

#define MAX_MESSAGE_SIZE 50
#define BAD_TIMEOUT 2000 // ms
#define GOOD_TIMEOUT 2000 // ms

enum state {
    IDLE, READYTOSEND, READYTOSENDWAIT, FLOWCTRL, FLOWCTRLWAIT, MOBUFFER,
    MOBUFFERWAIT, SENDBADWAIT, SENDGOODWAIT, SEND, SENDWAIT
};

static const char* state_str[11] = {
    "IDLE", "READYTOSEND", "READYTOSENDWAIT", "FLOWCTRL", "FLOWCTRLWAIT", "MOBUFFER",
    "MOBUFFERWAIT", "SENDBADWAIT", "SENDGOODWAIT", "SEND", "SENDWAIT"
};


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

class rock_machine_state {

    public:
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*) {}       
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*) {}
        virtual uint get_state_id() {return _state_id;} 
    protected:
        uint _state_id = IDLE;
        void change_state(rock_machine*, rock_machine_state*);
    };

class rock_machine_idle_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void start(rock_machine*);
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*) {}
    private:
        rock_machine_idle_wait_state() {_state_id = IDLE;}
        static rock_machine_state* _instance;
};

class rock_machine_rts_state : public rock_machine_state {
    public:    
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*);        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*) {}
    private:
        rock_machine_rts_state() {_state_id = READYTOSEND;}
        static rock_machine_state* _instance;
};

class rock_machine_rts_wait_state : public rock_machine_state {
    public:
        virtual void start(rock_machine*) {}    
        static rock_machine_state* instance();
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_rts_wait_state() {_state_id = READYTOSENDWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_flowctrl_state : public rock_machine_state {
    public:
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*);        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*) {}
    private:
        rock_machine_flowctrl_state() {_state_id = FLOWCTRL;}
        static rock_machine_state* _instance;
};

class rock_machine_flowctrl_wait_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_flowctrl_wait_state() {_state_id = FLOWCTRLWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_mobuffer_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {} 
        virtual void send(rock_machine*);        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*) {}
    private:
        rock_machine_mobuffer_state() {_state_id = MOBUFFER;}
        static rock_machine_state* _instance;
};

class rock_machine_mobuffer_wait_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_mobuffer_wait_state() {_state_id = MOBUFFERWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_sendbad_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*);
    private:
        rock_machine_sendbad_wait_state() {_state_id = SENDBADWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_sendgood_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*);
    private:
        rock_machine_sendgood_wait_state() {_state_id = SENDGOODWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_send_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {} 
        virtual void send(rock_machine*);        
        virtual void send_ok(rock_machine*, char* response) {}
        virtual void repeat(rock_machine*) {}
    private:
        rock_machine_send_state() {_state_id = SEND;}
        static rock_machine_state* _instance;
};

class rock_machine_send_wait_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void start(rock_machine*) {}
        virtual void send(rock_machine*) {}        
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_send_wait_state() {_state_id = MOBUFFERWAIT;}
        static rock_machine_state* _instance;
};

#endif