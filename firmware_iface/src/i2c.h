#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
#include <stdbool.h>

bool i2c_init(uint32_t freq);
int8_t i2c_write(uint8_t addr, uint8_t* data, uint8_t count, bool stop);
int8_t i2c_read(uint8_t addr, uint8_t* data, uint8_t count, bool stop);

#endif // I2C_H_
