/**
 * 2017 Orange Pi One BSP
 * File: SerialPortLib.h
 * nhoangvubk@gmail.com
 **/

#ifndef _SERIALPORTLIB_H_
#define _SERIALPORTLIB_H_

#include <stdint.h>

#define UART0_BASE 0x01C28000
#define UART1_BASE  0x01C28400
#define UART2_BASE  0x01C28800
#define UART3_BASE  0x01C28C00
#define R_UART_BASE 0x01F02800

/* useful defaults for LCR */
#define UART_LCR_8N1	0x03

#define UART_LCRVAL UART_LCR_8N1		/* 8 data, 1 stop, no parity */
#define UART_MCRVAL (UART_MCR_DTR | UART_MCR_RTS)		/* RTS/DTR */

#define UART_REG(x) uint32_t x
#define UART_LSR_THRE	0x20		/* Xmit holding register empty */
#define CONFIG_SYS_NS16550_IER  0x00
#define UART_LSR_TEMT	0x40		/* Xmitter empty */

/*
 * These are the definitions for the Modem Control Register
 */
#define UART_MCR_DTR	0x01		/* DTR   */
#define UART_MCR_RTS	0x02		/* RTS   */
#define UART_MCR_OUT1	0x04		/* Out 1 */
#define UART_MCR_OUT2	0x08		/* Out 2 */
#define UART_MCR_LOOP	0x10		/* Enable loopback test mode */
#define UART_MCR_AFE	0x20		/* Enable auto-RTS/CTS */

#define UART_MCR_DMA_EN	0x04
#define UART_MCR_TX_DFR	0x08

/**
 * These are the definitions for the Line Control Register
 *
 * Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
 * UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
 **/
#define UART_LCR_WLS_MSK 0x03		/* character length select mask */
#define UART_LCR_WLS_5	0x00		/* 5 bit character length */
#define UART_LCR_WLS_6	0x01		/* 6 bit character length */
#define UART_LCR_WLS_7	0x02		/* 7 bit character length */
#define UART_LCR_WLS_8	0x03		/* 8 bit character length */
#define UART_LCR_STB	0x04		/* # stop Bits, off=1, on=1.5 or 2) */
#define UART_LCR_PEN	0x08		/* Parity eneble */
#define UART_LCR_EPS	0x10		/* Even Parity Select */
#define UART_LCR_STKP	0x20		/* Stick Parity */
#define UART_LCR_SBRK	0x40		/* Set Break */
#define UART_LCR_BKSE	0x80		/* Bank select enable */
#define UART_LCR_DLAB	0x80		/* Divisor latch access bit */

/*
 * These are the definitions for the FIFO Control Register
 */
#define UART_FCR_FIFO_EN	0x01 /* Fifo enable */
#define UART_FCR_CLEAR_RCVR	0x02 /* Clear the RCVR FIFO */
#define UART_FCR_CLEAR_XMIT	0x04 /* Clear the XMIT FIFO */
#define UART_FCR_DMA_SELECT	0x08 /* For DMA applications */
#define UART_FCR_TRIGGER_MASK	0xC0 /* Mask for the FIFO trigger range */
#define UART_FCR_TRIGGER_1	0x00 /* Mask for trigger set at 1 */
#define UART_FCR_TRIGGER_4	0x40 /* Mask for trigger set at 4 */
#define UART_FCR_TRIGGER_8	0x80 /* Mask for trigger set at 8 */
#define UART_FCR_TRIGGER_14	0xC0 /* Mask for trigger set at 14 */

#define UART_FCR_RXSR		0x02 /* Receiver soft reset */
#define UART_FCR_TXSR		0x04 /* Transmitter soft reset */

/* Ingenic JZ47xx specific UART-enable bit. */
#define UART_FCR_UME		0x10

/* Clear & enable FIFOs */
#define UART_FCR_DEFVAL (UART_FCR_FIFO_EN | \
			UART_FCR_RXSR |	\
			UART_FCR_TXSR)

#define thr rbr
#define iir fcr
#define dll rbr
#define dlm ier

struct NS16550
{
    UART_REG(rbr);		/* 0 */
    UART_REG(ier);		/* 1 */
    UART_REG(fcr);		/* 2 */
    UART_REG(lcr);		/* 3 */
    UART_REG(mcr);		/* 4 */
    UART_REG(lsr);		/* 5 */
    UART_REG(msr);		/* 6 */
    UART_REG(spr);		/* 7 */
    UART_REG(mdr1);		/* 8 */
    UART_REG(reg9);		/* 9 */
    UART_REG(regA);		/* A */
    UART_REG(regB);		/* B */
    UART_REG(regC);		/* C */
    UART_REG(regD);		/* D */
    UART_REG(regE);		/* E */
    UART_REG(uasr);		/* F */
    UART_REG(scr);		/* 10*/
    UART_REG(ssr);		/* 11*/
};

void
SerialPortInitialize (
    void
);

int
SerialPortWrite (
    uint8_t     *Buffer,
    uint32_t     NumberOfBytes
);

int
SerialPortPutC (
    uint8_t Buffer
);

int
SerialPortRead (
    uint8_t     *Buffer,
    uint32_t     NumberOfBytes
);

int
SerialPortPoll (
    void
);

#endif /* _SERIALPORTLIB_H_ */
