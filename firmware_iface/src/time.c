#include "time.h"
#include <avr/io.h>

void time_init(void) {
    TCCR1A = 0x00;
    TCCR1C = 0x00;
    TCCR1B = 0x01;
}

uint16_t time_us_16(void) {
    return TCNT1;
}

void timeout_begin(timeout_t* timeout, uint16_t delta) {
    timeout->delta = delta;
    timeout->start_time = time_us_16();
}

bool timeout_reached(timeout_t* timeout) {
    uint16_t d = time_us_16();
    d -= timeout->start_time;
    return d > timeout->delta;
}
