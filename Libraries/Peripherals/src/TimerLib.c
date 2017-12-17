/**
 * 2017 Orange Pi One BSP
 * File: TimerLib.c
 * nhoangvubk@gmail.com
 **/

#include <stdio.h>
#include <string.h>
#include "Library/TimerLib.h"
#include "Library/SerialPortLib.h"
#include "Library/GpioLib.h"
#include "Library/GicLib.h"
#include "helper.h"

void
Timer0Init (
    void
)
{
    uint priority_field;
    char text[25];
    /** Timer0 config
      */
    WriteReg(TIMER_BASE, TMR0_INTV_VALUE_REG_OFFSET, 0xa0000); //interval 1s
    WriteReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET, 0x74); // clock from 24Mhz, prescaler 128
    //reload timer
    WriteReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET, ReadReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET) |(1<<1));
    while ((ReadReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET)>>1)&1);   // Wait until reload complete
    // Enable timer0
    WriteReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET, ReadReg(TIMER_BASE, TMR0_CTRL_REG_OFFSET) |(1<<0));
    // enable Timer0 interrupt
    WriteReg(TIMER_BASE, TMR_IRQ_EN_REG_OFFSET, 1);

#if 0
    /** GIC setup
      */
    GIC_SetPriority((IRQn_Type)0, 0xff);
    priority_field = GIC_GetPriority((IRQn_Type)0);
    sprintf(text, "priority implemented = 0x%x\r\n", priority_field);
    SerialPortWrite((uint8_t *)text, strlen(text));

#if 1
    // Setup GIC CPU interface
    WriteReg(GICC_BASE, GICC_CTLR, 1); // Enable CPU interface , i dont know which core is running
    WriteReg(GICC_BASE, GICC_PMR, 0xFF); // Setup GIC CPU interface priority
    WriteReg(GICC_BASE, GICC_BPR, 0);

    // Setup GIC distributer
    WriteReg(GICD_BASE, GICD_CTRL, 0); // Enable forward interrupt group 1
    WriteReg(GICD_BASE, GICD_ICENABLER_1, 1 << 18); // Disable irq 50
    WriteReg(GICD_BASE, GICD_ICFGR_3, 0x1<<4); // level sensitive
    WriteReg(GICD_BASE, GICD_IPRIORITYR_12, 0x80<<16); // Set priority of irq 50 = 1
    WriteReg(GICD_BASE, GICD_ITARGETSR_12, 0x1<<16); // Forward to core 0
    WriteReg(GICD_BASE, GICD_CTRL, 1); // Enable forward interrupt
    WriteReg(GICD_BASE, GICD_ISENABLER_1, 1<<18); // Enable irq 50 (timer0)

#else
    GIC_DistInit();
    GIC_CPUInterfaceInit();

    /* Determine number of implemented priority bits */
    GIC_SetPriority (Timer0_IRQn, 0xFFU);

    /* Set lowest priority -1 */
    GIC_SetPriority (Timer0_IRQn, GIC_GetPriority(Timer0_IRQn)-1);

    /* Enable IRQ */
    GIC_EnableIRQ (Timer0_IRQn);
#endif

    int cpu_id;
    cpu_id = __read_cpu_id() & 3;
    sprintf(text, "CPU id %d\r\n", cpu_id);
    SerialPortWrite((uint8_t *)text, strlen(text));

    __irq_enable();
#endif
}
