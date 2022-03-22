/*
    GPS Machine Definition
    
    Search for specific NMEA sentence types 
    
    $GPRMC,153029.000,A,5147.6360,N,00127.6627,W,0.00,262.90,220322,,,A*79

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

#define NMEA_CHARACTERS 44

// Parse a sentence looking for $GPRMC type
// Returns true if specific sentence is found
// bool get_GPRMC(char* sentence_buf, char* gprmc_buf) {
//     if(strstr(sentence_buf, "$GPRMC") != NULL) {
//         // memcpy(gprmc_buf, sentence_buf, strlen(sentence_buf) + 1);
//         memcpy(gprmc_buf, sentence_buf, NMEA_CHARACTERS);        
//         return true;    
//     }
//     return false;
// }
#endif