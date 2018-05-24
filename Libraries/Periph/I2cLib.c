#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Periph/helper.h"
#include "Periph/I2cLib.h"
#include "Periph/SerialPortLib.h"
#include "Periph/CcuLib.h"
#include "Periph/GpioLib.h"

void I2cGenerateStart(void)
{
    uint32_t Temp;
    Temp = ReadReg(TWI0_BASE, TWI_CNTR_OFFSET);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, Temp | TWI_CNTR_M_START);
}

void I2cGenerateStop(void)
{
    uint32_t Temp;
    Temp = ReadReg(TWI0_BASE, TWI_CNTR_OFFSET);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, Temp | TWI_CNTR_M_STOP);
}

void I2cClearFlag(void)
{
    uint32_t Temp;
    Temp = ReadReg(TWI0_BASE, TWI_CNTR_OFFSET);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, Temp | TWI_CNTR_INTF);
}

uint8_t I2cCheckFlag(void)
{
    uint32_t CurStat;
    CurStat = ReadReg(TWI0_BASE, TWI_CNTR_OFFSET);
    return ((CurStat & TWI_CNTR_INTF) == TWI_CNTR_INTF);
}

uint8_t I2cCheckStatus(void)
{
    return ReadReg(TWI0_BASE, TWI_STAT_OFFSET);
}

void I2cWrite(uint8_t chip_addr, uint8_t reg_addr, uint8_t* data, uint8_t len)
{
    uint Temp;

    /* Start */
//    I2cGenerateStart();
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK | TWI_CNTR_M_START);
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Start Sent, Status 0x%x\r\n", Temp);

    /* Send Address + Write CMD */
    WriteReg(TWI0_BASE, TWI_DATA_OFFSET, chip_addr);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> ADDR+WR Sent, Status 0x%x\r\n", Temp);

    /* Write Reg Address */
    WriteReg(TWI0_BASE, TWI_DATA_OFFSET, reg_addr);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Reg ADDR Sent, Status 0x%x\r\n", Temp);

    // Write Reg Data
    WriteReg(TWI0_BASE, TWI_DATA_OFFSET, data[0]);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Reg Data Sent, Status 0x%x\r\n", Temp);

    // Stop
    I2cGenerateStop();
    I2cClearFlag();
    printf("==> STOP\r\n");
}

void I2cRead(uint8_t chip_addr, uint8_t reg_addr, uint8_t* data, uint8_t len)
{
    uint Temp;
    char Text[30];

    // Start
//    I2cGenerateStart();
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK | TWI_CNTR_M_START);
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Start Sent, Status 0x%x\r\n", Temp);

    // Send Address + Write CMD
    WriteReg(TWI0_BASE, TWI_DATA_OFFSET, chip_addr);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> ADDR+WR Sent, Status 0x%x\r\n", Temp);

    // Write Reg Address
    WriteReg(TWI0_BASE, TWI_DATA_OFFSET, reg_addr);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Reg ADDR Sent, Status 0x%x\r\n", Temp);

    // Restart
//    I2cGenerateStart();
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK | TWI_CNTR_M_START);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf(Text, "==> Restart Sent, Status 0x%x\r\n", Temp);

    // Send Address + Read CMD
    WriteReg(TWI0_BASE, TWI_DATA_OFFSET, chip_addr | 1);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Address+RE Sent, Status 0x%x\r\n", Temp);

    // Continue
    Temp = ReadReg(TWI0_BASE, TWI_CNTR_OFFSET);
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, Temp & ~TWI_CNTR_A_ACK);
    I2cClearFlag();
    while (!I2cCheckFlag());
    Temp = I2cCheckStatus();
    printf("==> Continue, Status 0x%x\r\n", Temp);

    data[0] = ReadReg(TWI0_BASE, TWI_DATA_OFFSET);
    I2cGenerateStop();
    I2cClearFlag();
    printf("==> STOP\r\n");
}

void I2cInitialize(uint8_t I2cBus)
{
    uint Temp;

    // De-assert TWI reset
    Temp = ReadReg(CCU_BASE, BUS_SW_RESET_REG4);
    WriteReg(CCU_BASE, BUS_SW_RESET_REG4, Temp | 1);

    printf("BUS SW RESET REG 4: 0x%x\r\n", Temp);

    // Setup Pin Mux
    Temp = ReadReg(GPIO_BASE, PA_CONFIG1_OFFSET);
    WriteReg(GPIO_BASE, PA_CONFIG1_OFFSET, ((((Temp & ~(7<<12)) & ~(7<<16)) | 2<<12) | 2<<16));

    Temp = ReadReg(GPIO_BASE, PA_CONFIG1_OFFSET);
    printf("GPIO PA CONFIG: 0x%x\r\n", Temp);

    // check bus clock setting
    Temp = ReadReg(CCU_BASE, APB2_CFG_REG);
    printf("APB2: 0x%x\r\n", Temp);

    // Enable TWI0 clock gating
    Temp = ReadReg(CCU_BASE, BUS_CLK_GATING_REG3);
    WriteReg(CCU_BASE, BUS_CLK_GATING_REG3, Temp | TWI0_GATING);

    Temp = ReadReg(CCU_BASE, BUS_CLK_GATING_REG3);
    printf("CLK GATING REG3: 0x%x\r\n", Temp);

///////////////

    // Clear TWI0 soft-reset
    WriteReg(TWI0_BASE, TWI_SRST_OFFSET, 1);
    WriteReg(TWI0_BASE, TWI_SRST_OFFSET, 0);

    // Config TWI0 Clock 100Khz, N=1, M=11, APB1 sourced from 24Mhz OSC
    WriteReg(TWI0_BASE, TWI_CCR_OFFSET, 0x59);

    Temp = ReadReg(TWI0_BASE, TWI_CCR_OFFSET);
    printf("TWI CCR CFG: 0x%x\r\n", Temp);

    // Clear slave address
    WriteReg(TWI0_BASE, TWI_ADDR_OFFSET, 0);
    WriteReg(TWI0_BASE, TWI_XADDR_OFFSET, 0);

    // Enable BUS | STOP
    WriteReg(TWI0_BASE, TWI_CNTR_OFFSET, TWI_CNTR_BUS_EN | TWI_CNTR_M_STOP);

    Temp = ReadReg(TWI0_BASE, TWI_CNTR_OFFSET);
    printf("TWI ENABLE BUS: 0x%x\r\n", Temp);
}
