/*
    RockBLOCK Machine Class Definition

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rock_machine.hpp"

volatile bool timeout_fired = false;

// Setup a timeout callback
int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    timeout_fired = true;
    return 0;
}

rock_machine::rock_machine(serial &uart, char *message) : _serial(uart)
{
    // Starting state
    _state = rock_machine_idle_wait_state::instance();
    _message = message;
}

void rock_machine::run()
{
    static uint previous_state = get_state_id();
    if (previous_state != get_state_id())
    {
        previous_state = get_state_id();
        send();
    }

    if (timeout_fired)
    {
        timeout_fired = false;
        repeat();
    }
}

void rock_machine::write(const char *s)
{
    _serial.uart_write_blocking((uint8_t *)s, strlen(s));
}

void rock_machine::start()
{
    _state->start(this);
}

void rock_machine::send()
{
    _state->send(this);
}
void rock_machine::send_ok(char *response)
{
    _state->send_ok(this, response);
}
void rock_machine::repeat()
{
    _state->repeat(this);
}

void rock_machine::change_state(rock_machine_state *state)
{
    _state = state;
}

uint rock_machine::get_state_id()
{
    return _state->get_state_id();
}

const char *rock_machine::get_state()
{
    return state_str[_state->get_state_id()];
}

const char *rock_machine::get_message()
{
    return this->_message;
}

void rock_machine::inc_mail_count()
{
    this->_mail_count++;
}

void rock_machine::dec_mail_count()
{
    this->_mail_count--;
}

uint rock_machine::get_mail_count()
{
    return this->_mail_count;
}

int get_response(const char *response)
{
    char buffer[128] = {'\0'};
    memcpy(buffer, response, sizeof(buffer));
    if (buffer[0] == '\r')
    {
        return ISBD_EMPTY;
    }
    puts(buffer);
    if (strstr(buffer, "OK\r") != NULL)
    {
        return ISBD_OK;
    }
    else if (strstr(buffer, "+SBDIX:") != NULL)
    {
        // Parse SDBIX response
        uint16_t sbdix_codes[6] = {0};
        for (int i = 0; i < 6; ++i)
        {
            char *p = strtok(i == 0 ? buffer : NULL, " ,");
            if (i > 0)
            {
                sbdix_codes[i - 1] = atol(p);
            }
        }
        switch (sbdix_codes[0])
        {
        case 0 ... 2:                // MO status sent
            if (sbdix_codes[2] == 1) // MT status
            {
                return ISBD_MAIL;
            }
            return ISBD_SENT;
        default:
            return ISBD_NOT_SENT; // MO status not sent
        }
    }
    else
    {
        return ISBD_ERROR;
    }
}