#include "usart.h"
#include "time.h"
#include "printf.h"
#include "a2b.h"
#include "i2c.h"
#include "log.h"

int main(void) {
    usart_initialize();
    dlog("BOOT!");

    time_init();
    i2c_init(100000);

    dlog("Discovering bus");
    a2b_set_rc(0x6c);
    a2b_init_master();
    a2b_discover_configure_bus();
    a2b_main_loop();
}
