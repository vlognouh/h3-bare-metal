#ifndef __CCULIB_H__
#define __CCULIB_H__

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

#endif  /*__CCULIB_H__*/
