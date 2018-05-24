/**
 * 2017 Orange Pi One Tianocore
 * File: SerialPortLib.c
 * nhoangvubk@gmail.com
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

#include "Periph/helper.h"

#include "Periph/SerialPortLib.h"
#include "Periph/TimerLib.h"
#include "Periph/GpioLib.h"
#include "Periph/CcuLib.h"
#include "Periph/I2cLib.h"
#include "Periph/SpiLib.h"
#include "Periph/GicLib.h"
#include "SSD1306/SSD1306.h"

/**
 * MAIN FUNC
 */
int main(int argc, char const *argv[])
{
    /* Configure PA10 as output */
    WriteReg(GPIO_BASE, PA_CONFIG1_OFFSET, ReadReg(GPIO_BASE, PA_CONFIG1_OFFSET) & (~(6 << 8)));

    /* Set PA10 */
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) | (1 << 10));

    /* Set PL Clock gating in APB0_CLKGATE register */
    WriteReg(APB0_CLKGATE, 0, 3);

    /* Configure PL10 as output */
    WriteReg(PL_BASE, PL_CONFIG1_OFFSET, ReadReg(PL_BASE, PL_CONFIG1_OFFSET) & (~(6 << 8)));

    /* clear PL10 */
    WriteReg(PL_BASE, PL_DATA_OFFSET, ReadReg(PL_BASE, PL_DATA_OFFSET) & ~(1 << 10));

    

////// SPI
    SPI0_Init();

    OLED_Init();
    printf("Oled Test !!\r\n");
    OLED_P8x16Str(0x00, 0x01, "TESTING ngon");

    while (1)
    {

    }
    return 0;
}

void test_un(void)
{
    SerialPortWrite((uint8_t *)"U\r\n", 3);
}

void test_sw(void)
{
    SerialPortWrite((uint8_t *)"SW\r\n", 4);
}

void test_pr(void)
{
    SerialPortWrite((uint8_t *)"P\r\n", 3);
}

void test_da(void)
{
    SerialPortWrite((uint8_t *)"D\r\n", 3);
}

void test_fi(void)
{
    SerialPortWrite((uint8_t *)"F\r\n", 3);
}

void test_ir(void)
{
    uint32_t val;

    SerialPortWrite((uint8_t *)"I\r\n", 3);
    // val = ReadReg(GICC_BASE, GICC_IAR);
    // printf("IRQ IAR : 0x%x\r\n", val);

    /* Clear active interrupt */
    val = ReadReg(GICD_BASE, GICD_ICPENDR_1);
    printf("IRQ IC pending : 0x%lx\r\n", val);

    val = ReadReg(GICD_BASE, GICD_ICACTIVER_1);
    printf("IRQ IC active : 0x%lx\r\n", val);

    // WriteReg(GICD_BASE, GICD_ICPENDR_1, 0xFFFFFFFF);

    /*
     * clear Timer pending
     */
    WriteReg(TIMER_BASE, TMR_IRQ_STA_REG_OFFSET, 1);
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) ^ (1 << 10));
    // WriteReg(GICD_BASE, GICD_ICACTIVER_1, 0xFFFFFFFF);

    val = ReadReg(GICD_BASE, GICD_ICPENDR_1);
    printf("IRQ IC pending : 0x%lx\r\n", val);
    // val = ReadReg(GICD_BASE, GICD_ICACTIVER_1);
    // printf("IRQ IC active : 0x%x\r\n", val);

    // WriteReg(GICC_BASE, GICC_EOIR, 0x32);
}
