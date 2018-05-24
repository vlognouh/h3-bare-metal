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


/**
 * MAIN FUNC
 */
int main(int argc, char const *argv[])
{
    uint32_t Temp;
    uint8_t value;
    uint8_t data[2];
    uint32_t i;

    float fval;

    int *pa, *pb;

    // extern int end;
    // extern int HeapLimit;

    Temp = ReadReg(CCU_BASE, BUS_CLK_GATING_REG2);
    printf("=== BUS_CLK_GATING_REG2 : %lx\r\n", Temp);

    /* Configure PA10 as output */
    WriteReg(GPIO_BASE, PA_CONFIG1_OFFSET, ReadReg(GPIO_BASE, PA_CONFIG1_OFFSET) & (~(6 << 8)));

    /* Set PA10 */
    //WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) | (1 << 10));

    /* Clear PA10 */
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) & ~(1 << 10));

    /* Set PL Clock gating in APB0_CLKGATE register */
    WriteReg(APB0_CLKGATE, 0, 3);

    /* Configure PL10 as output */
    WriteReg(PL_BASE, PL_CONFIG1_OFFSET, ReadReg(PL_BASE, PL_CONFIG1_OFFSET) & (~(6 << 8)));

    /* clear PL10 */
    WriteReg(PL_BASE, PL_DATA_OFFSET, ReadReg(PL_BASE, PL_DATA_OFFSET) & ~(1 << 10));

    printf("Hello Everyone 29  !!\r\n");

    pa = malloc(200);
    pb = malloc(100);
    printf("==== printf Worked: 0x%p\r\n", pa);
    printf("==== printf Worked: 0x%p\r\n", pb);

    GicInit();

    free(pa);
//    WriteReg(CCU_BASE, PLL_PERIPH0, 1<<31 | 5<<8 | 2<<4 | 2);
//    while (!(ReadReg(CCU_BASE, PLL_PERIPH0) & 1<<28));
    Temp = ReadReg(CCU_BASE, PLL_PERIPH0);
    printf("--- PLL0= 0x%lx\r\n", Temp);

//    WriteReg(CCU_BASE, PLL_PERIPH1, 1<<31 | 5<<8 | 2<<4 | 2);
//    while (!(ReadReg(CCU_BASE, PLL_PERIPH1) & 1<<28));
    Temp = ReadReg(CCU_BASE, PLL_PERIPH1);
    printf("--- PLL1= 0x%lx\r\n", Temp);

//    WriteReg(CCU_BASE, AHB1_APB1_CFG_REG, 2<<12 | 2<<8 | 2<<6);
    Temp = ReadReg(CCU_BASE, AHB1_APB1_CFG_REG);
    printf("--- AHB1= 0x%lx\r\n", Temp);

    Timer0Init();
////// I2C
#if 0
    I2cInitialize(0);
    I2cRead(0xD0, 0x75, data, 1);
    printf("Who I Am = %d\r\n", data[0]);

    data[0] = 0x23;
    I2cWrite(0xD0, 0x6B, data, 1);

    I2cRead(0xD0, 0x6B, data, 1);
    printf("MPU6050 Addr:0x6B = %d\r\n", data[0]);
#endif
#if 1
////// SPI
    SPI0_Init();
    Dummy_Byte(); // => to correct SPI_SCK polarity
    value = Transfer_One(0x00);
    printf("SPI Reg 0x0? = 0x%x\r\n", value);

    value = Transfer_One(0x01);
    printf("SPI Reg 0x1? = 0x%x\r\n", value);

    value = Transfer_One(0x02);
    printf("SPI Reg 0x2? = 0x%x\r\n", value);

    // Print APB2 Config Register
    SerialPortWrite((uint8_t *)"---\r\n", 5);
    Temp = ReadReg(CCU_BASE, APB2_CFG_REG);
    printf("APB2 Reg = 0x%lx\r\n", Temp);
#endif
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
