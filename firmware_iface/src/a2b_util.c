#include "a2b_regs.h"
#include "a2b_internal.h"
#include "i2c.h"
#include "time.h"
#include "printf.h"
#include <stdint.h>

int8_t a2b_write_reg(bool bus, uint8_t reg, uint8_t value) {
    uint8_t addr = bus ? BUS_ADDR(A2B_MASTER_ADDR) : BASE_ADDR(A2B_MASTER_ADDR);
    uint8_t data[] = {reg, value};

    //printf("WRITE 0x%02x -> 0x%02x (%s)\n", reg, value, bus ? "bus" : "base");
    return i2c_write(addr, data, 2, true);
}

uint8_t a2b_read_reg(bool bus, uint8_t reg) {
    uint8_t addr = bus ? BUS_ADDR(A2B_MASTER_ADDR) : BASE_ADDR(A2B_MASTER_ADDR);
    uint8_t data[] = {reg};

    i2c_write(addr, data, 1, true);
    *data = 0x55;
    i2c_read(addr, data, 1, true);

    //printf("READ  0x%02x -> 0x%02x (%s)\n", reg, *data, bus ? "bus" : "base");
    return *data;
}

int8_t a2b_set_bitmask_reg(bool bus, uint8_t reg, uint8_t bitmask) {
    return a2b_write_reg(bus, reg, a2b_read_reg(bus, reg) | bitmask);
}

int8_t a2b_clr_bitmask_reg(bool bus, uint8_t reg, uint8_t bitmask) {
    return a2b_write_reg(bus, reg, a2b_read_reg(bus, reg) | bitmask);
}

a2b_irqinfo_t a2b_wait_for_next_irq(uint16_t timeout_ms) {
    a2b_irqinfo_t inf;
    inf.timeout = true;

    timeout_t t;
    timeout_begin(&t, timeout_ms);

    while (!a2b_base_read_reg(A2B_INTSTAT)) if (timeout_ms == 0 || timeout_reached(&t)) return inf;

    inf.timeout = false;

    uint8_t inttype = a2b_base_read_reg(A2B_INTTYPE);
    uint8_t intsrc = a2b_base_read_reg(A2B_INTSRC);

    inf.irqtype = inttype;
    inf.was_master_irq = (intsrc & 0x80) != 0;
    inf.src_node_id = intsrc & 0x0f;

    return inf;
}

void a2b_select_subnode(uint8_t node) {
    a2b_base_write_reg(A2B_NODEADR, node);
}
