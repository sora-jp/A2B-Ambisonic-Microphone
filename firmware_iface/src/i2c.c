#include "i2c.h"
#include <avr/io.h>
#include <avr/builtins.h>
#include <util/twi.h>
#include "printf.h"

static const uint8_t prescaler_lut[] = {1, 4, 16, 64};

#define BIT(x) (1 << (x))
#define TWCR_CLEAR (BIT(TWINT) | BIT(TWEA) | BIT(TWEN))
#define TWCR_START (TWCR_CLEAR | BIT(TWSTA))
#define TWCR_STOP  (TWCR_CLEAR | BIT(TWSTO))

bool i2c_init(uint32_t freq) {
    uint8_t prescaler = 1;
    uint8_t divider = F_CPU / (16 + 2 * freq * prescaler_lut[prescaler]);

    TWSR = 2;
    TWBR = 12;

    //TWSR = prescaler & 0x03;
    //TWBR = divider;
    TWCR |= BIT(TWEN);
    TWCR |= BIT(TWINT);

    return 0;
}

static void tw_wait_int(void) {
    while ((TWCR & BIT(TWINT)) == 0) __asm__ volatile("");
}

static uint8_t tw_write_raw(uint8_t val) {
    TWDR = val;
    TWCR = TWCR_CLEAR;
    tw_wait_int();

    return TW_STATUS;
}

static uint8_t tw_start(void) {
    TWCR = TWCR_START;
    tw_wait_int();

    return TW_STATUS;
}

static uint8_t tw_stop(void) {
    TWCR = TWCR_STOP;
    //tw_wait_int();

    return TW_STATUS;
}

static uint8_t tw_read(bool ack) {
    if (ack) {
        TWCR = TWCR_CLEAR;
    }
    else {
        TWCR = TWCR_CLEAR & ~BIT(TWEA);
    }

    tw_wait_int();
    uint8_t data = TWDR;
    return data;
}

static uint8_t twi_calc_addr(uint8_t addr, bool write) {
    return (addr << 1) | (write ? 0 : 1);
}

int8_t i2c_write(uint8_t addr, uint8_t* data, uint8_t count, bool stop) {
    uint8_t res = tw_start();
    if (res != TW_START && res != TW_REP_START) return -1;

    res = tw_write_raw(twi_calc_addr(addr, true));
    if (res != TW_MR_SLA_ACK && res != TW_MT_SLA_ACK) {
        tw_stop();
        return -1;
    }

    for (uint8_t i = 0; i < count; i++) {
        res = tw_write_raw(*data++);
        if (res != TW_MT_DATA_ACK) {
            tw_stop();
            return -1;
        }
    }

    if (stop) tw_stop();

    return count;
}

int8_t i2c_read(uint8_t addr, uint8_t* data, uint8_t count, bool stop) {
    uint8_t res = tw_start();
    if (res != TW_START && res != TW_REP_START) return -1;

    res = tw_write_raw(twi_calc_addr(addr, false));
    if (res != TW_MR_SLA_ACK && res != TW_MT_SLA_ACK) {
        return -1;
    }

    for (uint8_t i = 0; i < count; i++) {
        *data++ = tw_read(i != count - 1);
        res = TWSR & ~0x03;
        if (res != TW_MR_DATA_ACK && res != TW_MR_DATA_NACK) {
            return -1;
        }
    }

    if (stop) tw_stop();

    return count;
}
