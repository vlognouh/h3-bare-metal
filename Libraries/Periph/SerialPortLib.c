/**
 * 2017 Orange Pi One BSP
 * File: SerialPortLib.c
 * nhoangvubk@gmail.com
 **/

#include <stdint.h>
#include "Periph/SerialPortLib.h"

struct NS16550 *com_port = (struct NS16550 *)UART0_BASE;
uint8_t baud_divisor = 13;  // 115200
extern uint8_t test;
void NS16550_setbrg (
    struct NS16550 *com_port2,
    uint8_t baud_divisor
)
{
    com_port2->lcr = UART_LCR_BKSE | UART_LCRVAL;
    com_port2->dll = baud_divisor & 0xff;
    com_port2->dlm = (baud_divisor >> 8) & 0xff;
    com_port2->lcr = UART_LCRVAL;
}

void
SerialPortInitialize (
    void
)
{
    while (!(com_port->lsr & UART_LSR_TEMT));
    com_port->ier = CONFIG_SYS_NS16550_IER;
    com_port->mcr = UART_MCRVAL;
    com_port->fcr = UART_FCR_DEFVAL;
    if (baud_divisor != -1)
        NS16550_setbrg(com_port, baud_divisor);
}

int
SerialPortPutC (
    uint8_t     Buffer
)
{
    com_port->thr = Buffer;
    while ((com_port->lsr & UART_LSR_THRE) == 0)
        ;
    return 0;
}

int
SerialPortWrite (
    uint8_t     *Buffer,
    uint32_t     NumberOfBytes
)
{
    baud_divisor = 9;
    for (int i = 0; i < NumberOfBytes; ++i)
    {
        while ((com_port->lsr & UART_LSR_THRE) == 0)
            ;
        com_port->thr = *(Buffer+i);
    }
    return 0;
}

int
SerialPortRead (
    uint8_t     *Buffer,
    uint32_t     NumberOfBytes
)
{
    return 0;
}

int
SerialPortPoll (
    void
)
{
    return 0;
}
