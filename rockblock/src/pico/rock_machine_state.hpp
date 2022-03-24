/*
    RockBLOCK Machine Class Interface

    Uses the State pattern where each state is a Singleton
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

// #define MAX_MESSAGE_SIZE 50
// #define BAD_TIMEOUT 2000 // ms
// #define GOOD_TIMEOUT 2000 // ms

enum state {
    IDLE, READYTOSEND, READYTOSENDWAIT, FLOWCTRL, FLOWCTRLWAIT, MOBUFFER,
    MOBUFFERWAIT, SENDBADWAIT, SENDGOODWAIT, SEND, SENDWAIT
};

static const char* state_str[] = {
    "IDLE", "READYTOSEND", "READYTOSENDWAIT", "FLOWCTRL", "FLOWCTRLWAIT", "MOBUFFER",
    "MOBUFFERWAIT", "SENDBADWAIT", "SENDGOODWAIT", "SEND", "SENDWAIT"
};

class rock_machine;

class rock_machine_state {
    public:
        virtual void start(rock_machine*);
        virtual void send(rock_machine*);       
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
        uint get_state_id() {return _state_id;} 
    protected:
        uint _state_id = IDLE;
        void change_state(rock_machine*, rock_machine_state*);
    };

class rock_machine_idle_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void start(rock_machine*);
    private:
        rock_machine_idle_wait_state() {_state_id = IDLE;}
        static rock_machine_state* _instance;
};

class rock_machine_rts_state : public rock_machine_state {
    public:    
        static rock_machine_state* instance();
        virtual void send(rock_machine*);        
    private:
        rock_machine_rts_state() {_state_id = READYTOSEND;}
        static rock_machine_state* _instance;
};

class rock_machine_rts_wait_state : public rock_machine_state {
    public: 
        static rock_machine_state* instance();     
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_rts_wait_state() {_state_id = READYTOSENDWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_flowctrl_state : public rock_machine_state {
    public:
        static rock_machine_state* instance();
        virtual void send(rock_machine*);        
    private:
        rock_machine_flowctrl_state() {_state_id = FLOWCTRL;}
        static rock_machine_state* _instance;
};

class rock_machine_flowctrl_wait_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();     
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_flowctrl_wait_state() {_state_id = FLOWCTRLWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_mobuffer_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void send(rock_machine*);
    private:
        rock_machine_mobuffer_state() {_state_id = MOBUFFER;}
        static rock_machine_state* _instance;
};

class rock_machine_mobuffer_wait_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();        
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_mobuffer_wait_state() {_state_id = MOBUFFERWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_sendbad_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();       
        virtual void repeat(rock_machine*);
    private:
        rock_machine_sendbad_wait_state() {_state_id = SENDBADWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_sendgood_wait_state : public rock_machine_state {
    public:  
        static rock_machine_state* instance();
        virtual void repeat(rock_machine*);
    private:
        rock_machine_sendgood_wait_state() {_state_id = SENDGOODWAIT;}
        static rock_machine_state* _instance;
};

class rock_machine_send_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();
        virtual void send(rock_machine*);        
    private:
        rock_machine_send_state() {_state_id = SEND;}
        static rock_machine_state* _instance;
};

class rock_machine_send_wait_state : public rock_machine_state {
    public:   
        static rock_machine_state* instance();       
        virtual void send_ok(rock_machine*, char* response);
        virtual void repeat(rock_machine*);
    private:
        rock_machine_send_wait_state() {_state_id = MOBUFFERWAIT;}
        static rock_machine_state* _instance;
};
#endif