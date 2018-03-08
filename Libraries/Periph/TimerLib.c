/**
 * 2017 Orange Pi One BSP
 * File: TimerLib.c
 * nhoangvubk@gmail.com
 **/

#include <stdio.h>
#include <string.h>
#include "Periph/TimerLib.h"
#include "Periph/SerialPortLib.h"
#include "Periph/GpioLib.h"
#include "Periph/GicLib.h"
#include "Periph/helper.h"

void
Timer0Init (
    void
)
{
    /** Timer0 config
      */
    WriteReg(TIMER_BASE, TMR0_INTV_VALUE_REG_OFFSET, 0xA0000); //interval 1s
    WriteReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET, 0x74); // clock from 24Mhz, prescaler 128
    //reload timer
    WriteReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET, ReadReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET) |(1<<1));
    while ((ReadReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET)>>1)&1);   // Wait until reload complete
    // Enable timer0
    WriteReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET, ReadReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET) |(1<<0));
    // enable Timer0 interrupt
    WriteReg(TIMER_BASE, TMR_IRQ_EN_REG_OFFSET, 1);
}
