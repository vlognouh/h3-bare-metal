#include <stdio.h>
#include "helper.h"
#include "Library/GicLib.h"



void GicInit(void)
{
	uint32_t priority_field;

    /** GIC setup
      */
    GIC_SetPriority((IRQn_Type)0, 0xff);
    priority_field = GIC_GetPriority((IRQn_Type)0);
    printf("priority implemented = 0x%x\r\n", priority_field);

#if 1
    /* Setup GIC CPU interface, these reg is banked for each cpu */
    WriteReg(GICC_BASE, GICC_CTLR, 1); // Enable CPU interface , expect the running core is 0
    WriteReg(GICC_BASE, GICC_PMR, 0xFF); // Setup GIC CPU interface priority
    WriteReg(GICC_BASE, GICC_BPR, 0);

    /* Setup GIC Distributer */
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
    printf("CPU id %d\r\n", cpu_id);

    __irq_enable();
}