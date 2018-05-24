#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Periph/helper.h"
#include "Periph/CcuLib.h"
#include "Periph/SpiLib.h"
#include "Periph/GpioLib.h"
#include "Periph/SerialPortLib.h"

#define LEN 1
uint32_t Temp;
uint8_t Data;
char Text[25];

void SPI0_HW_Init(void)
{
    // Setup SPI0 PIN MUX PC0, PC1, PC2 | PC3 as output
    Temp = ReadReg(GPIO_BASE, PC_CONFIG0_OFFSET);
    WriteReg(GPIO_BASE, PC_CONFIG0_OFFSET, Temp & ~(6 << 12) & ~(4 << 8) & ~(4 << 4) & ~4);

    // Setup PA7 as CE pin
    Temp = ReadReg(GPIO_BASE, PA_CONFIG0_OFFSET);
    WriteReg(GPIO_BASE, PA_CONFIG0_OFFSET, Temp & ~(6 << 28));
}

void CE_High(void)
{
    // PA7
    Temp = ReadReg(GPIO_BASE, PA_DATA_OFFSET);
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, Temp | 1 << 7);
}

void CE_Low(void)
{
    // PA7
    Temp = ReadReg(GPIO_BASE, PA_DATA_OFFSET);
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, Temp & ~(1 << 7));
}

void CS0_High(void)
{
    // PC3 High
//    Temp = SPI0->SPI_TCR;
//    SPI0->SPI_TCR = Temp | 1 << 7;
    Temp = ReadReg(GPIO_BASE, PC_DATA_OFFSET);
    WriteReg(GPIO_BASE, PC_DATA_OFFSET, Temp | 1 << 3);
}

void CS0_Low(void)
{
    // PC3 Low
//    Temp = SPI0->SPI_TCR);
//    SPI0->SPI_TCR = Temp & ~(1 << 7);
    Temp = ReadReg(GPIO_BASE, PC_DATA_OFFSET);
    WriteReg(GPIO_BASE, PC_DATA_OFFSET, Temp & ~(1 << 3));
}
void delay(void)
{
    int i;
    for (i = 0; i < 100000; i++)
    {;}
}

void Dummy_Byte(void)
{
    CS0_Low();

    // Clear all pending interrupt
    SPI0->SPI_INT_STA = ~0;

    // Reset FIFO
    // SPI0->SPI_FIFO_CTL, 1 << 31 | 1 << 15);
    SPI0->SPI_FIFO_CTL = 1 << 31 | 1 << 15;

    // Enable TXFIFO Write
    SPI0->SPI_FIFO_STA |= 1 << 31;

    // Setup counter
    SPI0->SPI_BURST_CNT = 1;
    SPI0->SPI_XMIT_CNT = 1;
    SPI0->SPI_BURST_CTL_CNT = 1;

    // Fill FIFO
    SPI0->SPI_TXDATA = 0x00;

    // Start Transfer
    SPI0->SPI_TCR |= 1 << 31;

    // Wait until TC Flag set
    while (!(SPI0->SPI_INT_STA & (1 << 12)));

    // Clear Flag by write 1 to this bit
    SPI0->SPI_INT_STA = 1 << 12;

    // Read RX FIFO
    Data = SPI0->SPI_RXDATA;

    // SPI0->SPI_INT_CTL, 0);
    CS0_High();
}

uint8_t Transfer_One(uint8_t Addr)
{
    // Clear all pending interrupt
    SPI0->SPI_INT_STA = ~0;

    // Reset FIFO
    SPI0->SPI_FIFO_CTL = 1 << 31 | 1 << 15;

    // Enable TXFIFO Write
    SPI0->SPI_FIFO_STA |= 1 << 31;

    // Setup counter
    SPI0->SPI_BURST_CNT = 1;
    SPI0->SPI_XMIT_CNT = 1;
    SPI0->SPI_BURST_CTL_CNT = 1;

    // Fill FIFO
    SPI0->SPI_TXDATA = Addr;

    // Count TX FIFO
    Temp = SPI0->SPI_FIFO_STA >> 16 & 0xFF;
    printf("--- TX COUNT= 0x%lx\r\n", Temp);

    // Start Transfer
    SPI0->SPI_TCR |= 1 << 31;

    // Wait until TC Flag set
    while (!((SPI0->SPI_INT_STA) & (1 << 12)));

    // Clear Flag by write 1 to this bit
    SPI0->SPI_INT_STA = 1 << 12;

    // Count RX FIFO
    Temp = SPI0->SPI_FIFO_STA;
    printf("--- RX COUNT= 0x%lx\r\n", Temp);

    Data = SPI0->SPI_RXDATA;

    return Data;
}

void SPI0_Init(void)
{
    // PIN MUX
    SPI0_HW_Init();

    // Setup AHB Clock Gating (AHB 600MHz !!!)
    Temp = ReadReg(CCU_BASE, BUS_CLK_GATING_REG0);
    WriteReg(CCU_BASE, BUS_CLK_GATING_REG0, Temp | SPI0_GATING);

    // Setup BUS SPI0 SCLK 30Mhz=120Mhz/2^2/(4+1)
    //WriteReg(CCU_BASE, BUS_SPI0_CLK_REG, SPI_SCLK_GATING | 2 << 24 | 2 << 16 | 4 );
    WriteReg(CCU_BASE, BUS_SPI0_CLK_REG, SPI_SCLK_GATING);

    // De-assert SW Reset
    Temp = ReadReg(CCU_BASE, BUS_SW_RESET_REG0);
    WriteReg(CCU_BASE, BUS_SW_RESET_REG0, Temp | BUS_SPI0_RESET);

    // Setup Global Control: Enter Master mode
    SPI0->SPI_GBL = 1 << 31 | SPI_GBL_CTL_BUS_ENABLE | SPI_GBL_CTL_MASTER | SPI_GBL_CTL_TP;

    // Wait until Reset is done
    while (SPI0->SPI_GBL & 1 << 31);

    // Clear all pending interrupt
    SPI0->SPI_INT_STA = ~0;

    // Reset FIFO
    SPI0->SPI_FIFO_CTL = SPI_RXFIFO_RESET | SPI_TXFIFO_RESET;

    // Setup FIFO Control
    Temp = SPI0->SPI_TCR;
    Temp &= ~SPI_TFR_CTL_CPHA;
    Temp &= ~SPI_TFR_CTL_CPOL;
    Temp &= ~SPI_TFR_CTL_FBS;
    Temp &= ~SPI_TFR_CTL_DHB;
    Temp |= SPI_TFR_CTL_CS_MANUAL;
    SPI0->SPI_TCR = Temp;

    // Setup SCLK Clock
    SPI0->SPI_CLK_CTL = 1 << 12 | 14;

    // Enable interrupt
    SPI0->SPI_INT_CTL = 1 << 12;

    // Wait state
    SPI0->SPI_WCR = 20;
}
