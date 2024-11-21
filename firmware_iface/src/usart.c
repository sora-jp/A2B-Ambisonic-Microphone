#include "usart.h"

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdio.h>

#define BAUD_PRESCALE (F_CPU/16/BAUD-1)

void usart_initialize(void) {
    UBRR0 = 12;

    //UBRR0H = (BAUD_PRESCALE >> 8);
    //UBRR0L = BAUD_PRESCALE;
    UCSR0A = (1<<U2X0);
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void usart_send_byte(char byte) {
    if (byte == '\n') {
        usart_send_byte('\r');
    }
    loop_until_bit_is_set(UCSR0A,UDRE0);
    UDR0 = byte;
}

char usart_get_byte() {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

void _putchar(char c) {
    usart_send_byte(c);
}
