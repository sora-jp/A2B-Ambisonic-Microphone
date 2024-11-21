#ifndef A2B_H_
#define A2B_H_
#include <stdint.h>

void a2b_init_master(void);
void a2b_set_rc(uint32_t rc);
void a2b_discover_configure_bus(void);
void a2b_main_loop(void);

#endif // A2B_H_
