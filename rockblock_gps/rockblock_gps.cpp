/*
    RockBLOCK GPS Client Implementation
    
    Reads GPS module and sends position over satellite link 

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "pico/serial.hpp"
#include "pico/rock_machine.hpp"

#define UART1_ID uart1
#define UART1_TX_PIN 4
#define UART1_RX_PIN 5

#define RB_BAUD_RATE 19200
#define GPS_BAUD_RATE 9600

#define OUTPUT_INTERVAL 5000 // ms

#define BUTTON_PIN 15
#define LED_AMBER 14
#define LED_RED 16
#define LED_GREEN 18
#define LED_PIN PICO_DEFAULT_LED_PIN

// GPS
#define SENTENCE_SIZE 128
#define RESPONSE_SIZE 32
#define GPRMC_CHARACTERS 44

// RockBLOCK
#define MAX_MESSAGE_SIZE 50  // 1 credit
#define MAX_MESSAGE_COUNT 10  // Limit number of messages

bool construct_message();

void led_init();

void switch_led(uint state);

bool get_GPRMC(char* sentence_buf, char* gprmc_buf);

static char message_buffer[MAX_MESSAGE_SIZE] = "Payload";
static char rb_response_buffer[RESPONSE_SIZE];
static int rd_chars_rxed = 0;

static char gps_sentence[SENTENCE_SIZE]; // Raw NMEA buffer
static char gps_buffer[GPRMC_CHARACTERS + 20];  // GPRMC buffer (64 bytes)
static int gps_chars_rxed = 0;
volatile bool gps_updated = false;

bool on = true;

void on_uart1_rx();
void gpio_callback(uint gpio, uint32_t events);

bool led_repeating_timer_callback(struct repeating_timer *t) {
    gpio_put(LED_PIN, on);
    on = !on;
    return true;
}

serial serial0;
serial serial1(UART1_ID, UART1_TX_PIN, UART1_RX_PIN);

// Create a RockBLOCK machine
rock_machine rock(serial1, message_buffer);

int main()
{
    stdio_init_all();

    // Initialise uart0 for use with GPS on default GPIO
    // TX GP0, RX GP1
    serial0.init(GPS_BAUD_RATE);

    // Ititialise RockBLOCK on uart1
    serial1.init(RB_BAUD_RATE);

    // Turn off FIFO's
    uart_set_fifo_enabled(UART1_ID, false);
    int UART_IRQ = UART1_ID == uart1 ? UART1_IRQ : UART0_IRQ; 

    // Set up a RX interrupt uart 1
    irq_set_exclusive_handler(UART_IRQ, on_uart1_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART1_ID, true, false); 
    // End RockBLOCK uart setup

    struct repeating_timer led_timer;  // On-board LED heartbeat
    add_repeating_timer_ms(500, led_repeating_timer_callback, NULL, &led_timer);

    static uint previous_state = MOBUFFER;

    led_init();

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    sleep_ms(10000);  // let GPS / RockBLOCK warm up

    puts("RockBLOCK GPS test\r\n");

    while(1) {
        static bool print_fg = false;

        // Read GPS stream             
        while (serial0.uart_is_readable()) {           
            char ch = serial0.uart_getc();
            gps_sentence[gps_chars_rxed++] = ch;
            if (ch == '\n' || gps_chars_rxed == SENTENCE_SIZE - 1) {
                gps_sentence[gps_chars_rxed] = '\0';
                gps_chars_rxed = 0;
                print_fg = get_GPRMC(gps_sentence, gps_buffer);
            }
            break;
        }

        if (print_fg) {
            print_fg = false;
            puts(gps_buffer);
        }      

        // Send a message
        rock.run();

        // Get state for LED's and USB debug output
        volatile uint current_state = rock.get_state_id();
        if (current_state != previous_state) {
            previous_state = current_state;
            puts(state_str[current_state]);
            switch_led(current_state);
        }


        // // Put anything else non-blocking to be run here
        // puts("Do other stuff...\r\n");


    }
}

// Button callback handler - starts a new message session
void gpio_callback(uint gpio, uint32_t events) {
    if (construct_message()) {
        rock.start();
    }
}

// RockBLOCK RX interrupt handler
void on_uart1_rx() {
    while (uart_is_readable(UART1_ID)) {
        char ch = uart_getc(UART1_ID);
        rb_response_buffer[rd_chars_rxed++] = ch;
        if (ch == '\r' || rd_chars_rxed == RESPONSE_SIZE - 1) {
            rd_chars_rxed = 0;
            rock.send_ok(rb_response_buffer);
            memset(rb_response_buffer, 0, sizeof rb_response_buffer);
        }
    }
}

// Construct message to be sent
bool construct_message() {
    uint i = rock.get_message_count();
    if (i < MAX_MESSAGE_COUNT) {
        // Start message with message count
        itoa(rock.get_message_count(), message_buffer, 10);
        // Add a GPS reading
        if (MAX_MESSAGE_SIZE - strlen(message_buffer) > strlen(gps_buffer) + 1) {
            strcat(message_buffer, ",");
            strcat(message_buffer, gps_buffer);
            return true;
        } else {
            puts("Message too long");
            return false;            
        }
    }
    puts("Maximum message count");
    return false;
}

// Put a valid partial NMEA $GPRMC sentence into the buffer
// Returns true if specific sentence is found
bool get_GPRMC(char* sentence_buf, char* gprmc_buf) {
    if(strstr(sentence_buf, "$GPRMC") != NULL) {
        // memcpy(gprmc_buf, sentence_buf, strlen(sentence_buf) + 1);
        memcpy(gprmc_buf, sentence_buf, GPRMC_CHARACTERS);        
        return true;    
    }
    return false;
}

void led_init() {
    gpio_init(LED_PIN);   
    gpio_init(LED_RED);
    gpio_init(LED_AMBER);
    gpio_init(LED_GREEN);

    gpio_set_dir(LED_PIN, GPIO_OUT); 
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_AMBER, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_pull_up(BUTTON_PIN);

}

void switch_led(uint state) {
    switch(state) {
        case IDLE:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case READYTOSEND:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case READYTOSENDWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;                        
        case FLOWCTRL:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case FLOWCTRLWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;            
        case MOBUFFER:
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 0);
            break;
        case MOBUFFERWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;
        case SENDGOODWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_AMBER, 1);
            break;
        case SENDBADWAIT:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 1);
            gpio_put(LED_AMBER, 0);
            break;                                         
    }
}
