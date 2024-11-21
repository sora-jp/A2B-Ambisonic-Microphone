#ifndef USART_H_
#define USART_H_

void usart_initialize(void);
void usart_send_byte(char byte);
char usart_get_byte(void);

#endif // USART_H_
