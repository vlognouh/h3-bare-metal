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

    // Setup PA2 as CE pin
    Temp = ReadReg(GPIO_BASE, PA_CONFIG0_OFFSET);
    WriteReg(GPIO_BASE, PA_CONFIG0_OFFSET, Temp & ~(6 << 8));
}

void CE_High(void)
{
    // PA2
    Temp = ReadReg(GPIO_BASE, PA_DATA_OFFSET);
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, Temp | 1 << 2);
}

void CE_Low(void)
{
    // PA2
    Temp = ReadReg(GPIO_BASE, PA_DATA_OFFSET);
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, Temp & ~(1 << 2));
}

void SS0_High(void)
{
    // PC3 High
//    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
//    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp | 1 << 7);
    Temp = ReadReg(GPIO_BASE, PC_DATA_OFFSET);
    WriteReg(GPIO_BASE, PC_DATA_OFFSET, Temp | 1 << 3);
}

void SS0_Low(void)
{
    // PC3 Low
//    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
//    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp & ~(1 << 7));
    Temp = ReadReg(GPIO_BASE, PC_DATA_OFFSET);
    WriteReg(GPIO_BASE, PC_DATA_OFFSET, Temp & ~(1 << 3));
}
void delay(void)
{
    int i;
    for(i = 0; i<100000; i++)
    {;}
}

void Dummy_Byte(void)
{
    SS0_High();

    // Clear all pending interrupt
    WriteReg(SPI0_BASE, SPI_INT_STA, ~0);

    // Reset FIFO
    WriteReg(SPI0_BASE, SPI_FIFO_CTL, 1 << 31 | 1 << 15);

    // Setup FIFO Control
    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
    Temp &= ~SPI_TFR_CTL_CPHA;
    Temp &= ~SPI_TFR_CTL_CPOL;
    Temp &= ~SPI_TFR_CTL_FBS;
    Temp &= ~SPI_TFR_CTL_DHB;
    Temp |= SPI_TFR_CTL_CS_MANUAL;
    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp);

    // Setup Clock
    WriteReg(SPI0_BASE, SPI_CLK_CTL, 1 << 12 | 14);

    // Enable TXFIFO Write Enable
    Temp = ReadReg(SPI0_BASE, SPI_FIFO_STA);
    WriteReg(SPI0_BASE, SPI_FIFO_STA, Temp | 1 << 31);

    // Setup counter
    WriteReg(SPI0_BASE, SPI_BURST_CNT, 1);
    WriteReg(SPI0_BASE, SPI_XMIT_CNT, 1);
    WriteReg(SPI0_BASE, SPI_BURST_CTL_CNT, 1);

    // Fill FIFO
    WriteByte(SPI0_BASE, SPI_TXDATA, 0x00);

    // Enable interrupt
    WriteReg(SPI0_BASE, SPI_INT_CTL, 1 << 12);

    // Start Transfer
    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp | 1 << 31);

    // Wait util TC Flag set
    while (!(ReadReg(SPI0_BASE, SPI_INT_STA) & (1<<12)));

    // Clear Flag by write 1 to this bit
    WriteReg(SPI0_BASE, SPI_INT_STA, 1 << 12);

    // Read RX FIFO
    Data = ReadByte(SPI0_BASE, SPI_RXDATA);

    //
    WriteReg(SPI0_BASE, SPI_INT_CTL, 0);
}

uint8_t Transfer_One(uint8_t Addr)
{
    SS0_Low();
    // Clear all pending interrupt
    WriteReg(SPI0_BASE, SPI_INT_STA, ~0);

    // Reset FIFO
    WriteReg(SPI0_BASE, SPI_FIFO_CTL, 1 << 31 | 1 << 15);

    // Setup FIFO Control
    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
    Temp &= ~SPI_TFR_CTL_CPHA;
    Temp &= ~SPI_TFR_CTL_CPOL;
    Temp &= ~SPI_TFR_CTL_FBS;
    Temp &= ~SPI_TFR_CTL_DHB;
    Temp |= SPI_TFR_CTL_CS_MANUAL;
    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp);

//    // Setup SPI0 PIN MUX PC0, PC1, PC2 | PC3 as output
//    Temp = ReadReg(GPIO_BASE, PC_CONFIG0_OFFSET);
//    WriteReg(GPIO_BASE, PC_CONFIG0_OFFSET, Temp & ~(6 << 12) & ~(4 << 8) & ~(4 << 4) & ~4);

    // Setup Clock
    WriteReg(SPI0_BASE, SPI_CLK_CTL, 1 << 12 | 14);

    // Enable TXFIFO Write Enable
    Temp = ReadReg(SPI0_BASE, SPI_FIFO_STA);
    WriteReg(SPI0_BASE, SPI_FIFO_STA, Temp | 1 << 31);

    // Setup counter
    WriteReg(SPI0_BASE, SPI_BURST_CNT, 2);
    WriteReg(SPI0_BASE, SPI_XMIT_CNT, 2);
    WriteReg(SPI0_BASE, SPI_BURST_CTL_CNT, 2);

    // Fill FIFO
    WriteByte(SPI0_BASE, SPI_TXDATA, Addr);
    WriteByte(SPI0_BASE, SPI_TXDATA, 0x00);

    // Count TX FIFO
    Temp = ReadReg(SPI0_BASE, SPI_FIFO_STA)>>16 & 0xFF;
    printf(Text, "--- TX COUNT= 0x%x\r\n", Temp);

    // Enable interrupt
    WriteReg(SPI0_BASE, SPI_INT_CTL, 1 << 12);

    // Start Transfer
    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp | 1 << 31);

    // Wait util TC Flag set
    while (!(ReadReg(SPI0_BASE, SPI_INT_STA) & (1<<12)));

    // Clear Flag by write 1 to this bit
    WriteReg(SPI0_BASE, SPI_INT_STA, 1 << 12);

    // Count RX FIFO
    Temp = ReadByte(SPI0_BASE, SPI_FIFO_STA);
    printf("--- RX COUNT= 0x%lx\r\n", Temp);

    // Read RX FIFO
    Data = ReadByte(SPI0_BASE, SPI_RXDATA);
    printf("--- RX DATA= 0x%x\r\n", Data);

    Data = ReadByte(SPI0_BASE, SPI_RXDATA);

    //
    WriteReg(SPI0_BASE, SPI_INT_CTL, 0);

    SS0_High();
    return Data;
}

uint8_t SPI0_Read_Byte(uint8_t Address)
{
    // Reset FIFO
    WriteReg(SPI0_BASE, SPI_FIFO_CTL, 1 << 15 | 1 << 31);

    // Setup Burst count
    WriteReg(SPI0_BASE, SPI_BURST_CNT, LEN);
    WriteReg(SPI0_BASE, SPI_XMIT_CNT, LEN);
    WriteReg(SPI0_BASE, SPI_BURST_CTL_CNT, LEN);

    // Wait
    while (ReadReg(SPI0_BASE, SPI_TFR_CTL) & (1 << 31));

    // Write TX FIFO
    WriteByte(SPI0_BASE, SPI_TXDATA, Address);
    delay();

    // Count TX FIFO
//    Temp = ReadReg(SPI0_BASE, SPI_FIFO_STA)>>16 & 0xFF;
//    printf("--- TX COUNT= 0x%x\r\n", Temp);


    // Transmit
    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp | (1 << 31));

    // Wait
    while (ReadReg(SPI0_BASE, SPI_TFR_CTL) & (1 << 31));

    // Count RX FIFO
//    Temp = ReadByte(SPI0_BASE, SPI_FIFO_STA);
//    printf("--- RX COUNT= 0x%x\r\n", Temp);
    delay();
    return ReadByte(SPI0_BASE, SPI_RXDATA);
}

uint8_t SPI0_Read_Reg(uint8_t Reg)
{
    uint8_t value;
    SS0_Low();
    SPI0_Read_Byte(Reg);
    value = SPI0_Read_Byte(0x00);
    SS0_High();
    return value;
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
    WriteReg(SPI0_BASE, SPI_GBL_CTL, 1 << 31 | SPI_GBL_CTL_BUS_ENABLE | SPI_GBL_CTL_MASTER | SPI_GBL_CTL_TP);

    // Wait until Reset is done
    while (ReadReg(SPI0_BASE, SPI_GBL_CTL) & 1 << 31);

//    // Setup transfer control: CPOL, CPHA, MSB_FIRST, SS_SEL = 0b00
//    Temp = ReadReg(SPI0_BASE, SPI_TFR_CTL);
//    Temp &= 0;
////    Temp |= SPI_TFR_CTL_CPHA;
////    Temp |= SPI_TFR_CTL_CPOL;
////    Temp |= SPI_TFR_CTL_FBS;
////    Temp |= SPI_TFR_CTL_DHB;
//    Temp |= 1 << 13;
//    Temp |= SPI_TFR_CTL_CS_MANUAL;
//    WriteReg(SPI0_BASE, SPI_TFR_CTL, Temp);
//
//    // Setup Clock control, 24MHz/2^5 = 938KHz
//    WriteReg(SPI0_BASE, SPI_CLK_CTL, 2 << 8);

    // Wait state
    WriteReg(SPI0_BASE, 0x20, 20);
}
