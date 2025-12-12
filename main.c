volatile unsigned int *UART0 = (unsigned int*)0x101f1000;

void uart_putc(char c) { *UART0 = c; }
void uart_puts(const char *s) { while (*s) uart_putc(*s++); }

int main() {
    uart_puts("Hello IM JONATHANNNN!\n");

    unsigned data = 0b10110101;
    unsigned parity = __builtin_parity(data);

    uart_puts("Data parity bit: ");
    uart_putc(parity ? '1' : '0');
    uart_putc('\n');
    return 0;
}

