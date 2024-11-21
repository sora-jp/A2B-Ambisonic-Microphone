#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>
#include <stdbool.h>

#define US (1)
#define MS (US * 1000)
#define SEC (MS * 1000)

typedef struct _timeout {
    uint16_t start_time;
    uint16_t delta;
} timeout_t;

void time_init(void);
uint16_t time_us_16(void);

void timeout_begin(timeout_t* timeout, uint16_t delta);
bool timeout_reached(timeout_t* timeout);

#endif // TIME_H_
