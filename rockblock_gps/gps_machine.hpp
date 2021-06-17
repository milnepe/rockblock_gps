/*
    GPS Machine Definition
    
    Search for specific NMEA sentence types 
    

*/

/**
 * Copyright (c) 2020 Peter Milne.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef gps_machine_hpp
#define gps_machine_hpp

#include <stdio.h>
#include <string.h>

// Parse a sentence looking for $GPRMC type
// Returns true if specific sentence is found
bool get_GPRMC(char* sentence_buf, char* gprmc_buf) {
    if(strstr(sentence_buf, "$GPRMC") != NULL) {
        memcpy(gprmc_buf, sentence_buf, strlen(sentence_buf) + 1);
        return true;    
    }
    return false;
}
#endif