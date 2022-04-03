/*
    RockBLOCK Machine Class Interface

    State pattern where each state is a Singleton
    implementing that states behaviour
  
*/

#ifndef rock_machine_state_hpp
#define rock_machine_state_hpp

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/serial.hpp"
#include "rock_machine.hpp"

enum state_id {
    IDLE, READYTOSEND, FLOWCTRL, MOBUFFER,
    SENDBADWAIT, SENDGOODWAIT, SEND
};

static const char* state_str[] = {
    "IDLE", "READYTOSEND", "FLOWCTRL", "MOBUFFER",
    "SENDBADWAIT", "SENDGOODWAIT", "SEND"
};

class rock_machine;

class rock_machine_state {
    public:
        virtual void start(rock_machine*);
        virtual void send(rock_machine*);       
        virtual void send_ok(rock_machine*, uint8_t* response);
        virtual void repeat(rock_machine*);
        state_id get_state_id() {return _state_id;} 
    protected:
        void change_state(rock_machine*, rock_machine_state*);
        state_id _state_id = IDLE;
    };

class rock_machine_idle_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void send(rock_machine*);         
        virtual void start(rock_machine*);
    private:
        rock_machine_idle_wait_state() {_state_id = IDLE;}
        static rock_machine_state* _instance;
};

class rock_machine_rts_state : public rock_machine_state {
    public:    
        static rock_machine_state* instance();
        virtual void send(rock_machine*);
        virtual void send_ok(rock_machine*, uint8_t* response);
        virtual void repeat(rock_machine*);                
    private:
        rock_machine_rts_state() {_state_id = READYTOSEND;}
        static rock_machine_state* _instance;
};

class rock_machine_flowctrl_state : public rock_machine_state {
    public:
        static rock_machine_state* instance();
        virtual void send(rock_machine*);
        virtual void send_ok(rock_machine*, uint8_t* response);
        virtual void repeat(rock_machine*);                
    private:
        rock_machine_flowctrl_state() {_state_id = FLOWCTRL;}
        static rock_machine_state* _instance;
};

class rock_machine_mobuffer_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void send(rock_machine*);
        virtual void send_ok(rock_machine*, uint8_t* response);
        virtual void repeat(rock_machine*);        
    private:
        rock_machine_mobuffer_state() {_state_id = MOBUFFER;}
        static rock_machine_state* _instance;
};

class rock_machine_sendbad_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void send(rock_machine*);                   
        virtual void repeat(rock_machine*);
    private:
        rock_machine_sendbad_wait_state() {_state_id = SENDBADWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_sendgood_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void send(rock_machine*);       
        virtual void repeat(rock_machine*);
    private:
        rock_machine_sendgood_wait_state() {_state_id = SENDGOODWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_send_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void send(rock_machine*);
        virtual void send_ok(rock_machine*, uint8_t* response);
        virtual void repeat(rock_machine*);                
    private:
        rock_machine_send_state() {_state_id = SEND;}
        static rock_machine_state* _instance;
};
#endif
