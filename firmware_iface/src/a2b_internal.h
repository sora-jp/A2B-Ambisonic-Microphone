#ifndef A2B_INTERNAL_H_
#define A2B_INTERNAL_H_
#include <stdint.h>
#include <stdbool.h>

#define A2B_MASTER_ADDR 0x6e
#define A2B_CNF_RESPOFFS 248
#define A2B_CNF_RESPCYCS 0x99

#define BASE_ADDR(x) ((x) | 0)
#define BUS_ADDR(x) ((x) | 1)

int8_t a2b_write_reg(bool bus, uint8_t reg, uint8_t value);
uint8_t a2b_read_reg(bool bus, uint8_t reg);

int8_t a2b_set_bitmask_reg(bool bus, uint8_t reg, uint8_t bitmask);
int8_t a2b_clr_bitmask_reg(bool bus, uint8_t reg, uint8_t bitmask);

#define a2b_base_read_reg(...) a2b_read_reg(false, __VA_ARGS__)
#define a2b_bus_read_reg(...) a2b_read_reg(true, __VA_ARGS__)

#define a2b_base_write_reg(...) a2b_write_reg(false, __VA_ARGS__)
#define a2b_bus_write_reg(...) a2b_write_reg(true, __VA_ARGS__)

void a2b_select_subnode(uint8_t node);

typedef struct {
    bool timeout;
    uint8_t irqtype;
    bool was_master_irq;
    uint8_t src_node_id;
} a2b_irqinfo_t;

a2b_irqinfo_t a2b_wait_for_next_irq(uint16_t timeout_ms);

#endif // A2B_INTERNAL_H_
