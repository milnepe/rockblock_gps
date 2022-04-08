#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rock_machine.hpp"

const uint8_t error[32] = "\nfijdiej\r";
const uint8_t ok[32] = "\nOK\r";
const uint8_t sent[32] = "+SBDIX: 0, 16, 0, 0, 0, 0\r";
const uint8_t notsent[32] = "\n+SBDIX: 32, 16, 2, 0, 0, 0\r";
const uint8_t empty[32] = "\r123545";

int main() {
    stdio_init_all();

    sleep_ms(2000);


    printf("get_response test\n");

    printf("%u\n", get_response(error));
    printf("%u\n", get_response(ok));    
    printf("%u\n", get_response(sent));
    printf("%u\n", get_response(notsent));
    printf("%u\n", get_response(empty));

    puts("");

    return 0;
}
