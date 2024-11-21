#include "a2b.h"
#include "a2b_internal.h"
#include "a2b_regs.h"
#include "time.h"
#include "log.h"
#include <util/delay.h>

uint32_t a2b_respcycs;

void a2b_set_rc(uint32_t rc) {
    a2b_respcycs = rc;
}

void a2b_init_master(void) {
    a2b_base_write_reg(A2B_CONTROL, 0x04);
    _delay_ms(16);

    a2b_base_write_reg(A2B_CONTROL, 0x90); // MSTR
    _delay_ms(16);

    a2b_base_write_reg(A2B_RESPCYCS, a2b_respcycs);
    a2b_set_bitmask_reg(false, A2B_CONTROL, 0x01); // NEWSTRCT

    // Listen for all interrupts
    a2b_base_write_reg(A2B_INTMSK0, 0xFF);
    a2b_base_write_reg(A2B_INTMSK1, 0xFF);
    a2b_base_write_reg(A2B_INTMSK2, 0xFF);

    a2b_base_write_reg(A2B_I2SGCFG, 0x02);
    a2b_base_write_reg(A2B_I2SCFG, 0x03);

    while (true) {
        a2b_irqinfo_t irq = a2b_wait_for_next_irq(35000);
        //dlog("Next irq: %d %d", irq.irqtype, irq.timeout);
        if (irq.irqtype == 0xFF) break;
    }
}

void a2b_discover_configure_bus(void) {
    dlog("Attempting discovery (master)");
    a2b_base_write_reg(A2B_SWCTL, 0x00); // Disable switch, to force reset of slave nodes
    _delay_ms(20);
    a2b_base_write_reg(A2B_SWCTL, 0x01); // ENSW

    uint8_t n;
    for (n = 0; n < 16; n++) {
        bool success = false;

        timeout_t time;
        timeout_begin(&time, 35000);

        while (!timeout_reached(&time)) {
            a2b_base_write_reg(A2B_DISCVRY, a2b_respcycs - n * 4);

            a2b_irqinfo_t irq = a2b_wait_for_next_irq(1000);
            //if (!irq.timeout) dlog("irq %d", irq.irqtype);
            if (!irq.timeout && irq.irqtype == 24) {
                success = true;
                break;
            }
        }

        if (!success) {
            a2b_write_reg(n != 0, A2B_SWCTL, 0x00); // Disable switch (protect)
            a2b_set_bitmask_reg(n != 0, A2B_CONTROL, 0x02); // Abort discovery
            dlog("Discovery of subnode %d failed", n);
            break;
        }

        a2b_write_reg(n != 0, A2B_SWCTL, 0x21); // Following the simple discovery flow
        a2b_select_subnode(n);

        //uint8_t vendor  = a2b_bus_read_reg(A2B_VENDOR);
        //uint8_t prod    = a2b_bus_read_reg(A2B_PRODUCT);
        //uint8_t version = a2b_bus_read_reg(A2B_VERSION);

        //dlog("Discovered subnode %d", n, vendor, prod, version);

        //a2b_bus_write_reg(A2B_PDMCTL,  0x11);  // PDM0EN | HPFEN
        //a2b_bus_write_reg(A2B_PDMCTL2, 0x10);  // PDMALTCLK, DST=Bus
        //a2b_bus_write_reg(A2B_LDNSLOTS, 0x00); // No local downstream slots
        //a2b_bus_write_reg(A2B_LUPSLOTS, 0x01); // 1 local upstream slot (sourced by this node)

        //a2b_set_bitmask_reg(true, A2B_CONTROL, 0x10); // XCVRBINV
        a2b_bus_write_reg(A2B_CONTROL, 0x10);
        a2b_bus_write_reg(A2B_SWCTL, 0x01); // Enable power switch (discover next node)
    }

    a2b_base_write_reg(A2B_UPSLOTS, n); // We have (n) upstream slots to receive (and send over PDM8)
    a2b_base_write_reg(A2B_DATCTL, 0x02); // Enable upstream slots, not downstream slots
    a2b_base_write_reg(A2B_SLOTFMT, 0x40); // 24 bit slots, normal format

    for (uint8_t n2 = 0; n2 < n; n2++) {
        a2b_select_subnode(n2);
        _delay_ms(10);

        dlog("Configuring subnode %d", n2);

        a2b_bus_write_reg(A2B_PDMCTL,  0x11);  // PDM0EN | HPFEN
        a2b_bus_write_reg(A2B_PDMCTL2, 0x10);  // PDMALTCLK, DST=Bus
        a2b_bus_write_reg(A2B_LDNSLOTS, 0x00); // No local downstream slots
        a2b_bus_write_reg(A2B_LUPSLOTS, 0x01); // 1 local upstream slot (sourced by this node)
        a2b_bus_write_reg(A2B_UPSLOTS, (n-1) - n2); // How many upstream slots to pass to prev node
        a2b_bus_write_reg(A2B_DNSLOTS, 0x00); // ¯\_(ツ)_/¯
    }

    a2b_set_bitmask_reg(false, A2B_CONTROL, 0x01); // Apply new bus structure
}

void a2b_main_loop(void) {
    uint32_t rc = 0x60;
    while (true) {
        a2b_irqinfo_t irq = a2b_wait_for_next_irq(1000);
        if (irq.timeout) continue;
        dlog("IRQ: %d (%d %d)", irq.irqtype, irq.was_master_irq, irq.src_node_id);

        _delay_ms(20);
        dlog("Using RESPCYCS = 0x%x", ++rc);
        a2b_set_rc(rc);
        a2b_init_master();
        a2b_discover_configure_bus();
    }
}
