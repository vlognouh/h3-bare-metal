#ifndef __SPILIB_H__
#define __SPILIB_H__

#define BIT(nr)			(1UL << (nr))
#define SPI0_BASE   0x01C68000
#define SPI1_BASE   0x01C69000

#define SPI_GBL_CTL     0x04
#define SPI_GBL_CTL_BUS_ENABLE  BIT(0)
#define SPI_GBL_CTL_MASTER      BIT(1)
#define SPI_GBL_CTL_TP          BIT(7)

#define SPI_TFR_CTL     0x08
#define SPI_TFR_CTL_CPHA			BIT(0)
#define SPI_TFR_CTL_CPOL			BIT(1)
#define SPI_TFR_CTL_SPOL			BIT(2)
#define SPI_TFR_CTL_CS_MASK			0x30
#define SPI_TFR_CTL_CS(cs)			(((cs) << 4) & SPI_TFR_CTL_CS_MASK)
#define SPI_TFR_CTL_CS_MANUAL			BIT(6)
#define SPI_TFR_CTL_CS_LEVEL			BIT(7)
#define SPI_TFR_CTL_DHB			BIT(8)
#define SPI_TFR_CTL_FBS			BIT(12)
#define SPI_TFR_CTL_SDM			BIT(13)
#define SPI_TFR_CTL_XCH			BIT(31)

#define SPI_INT_CTL         0x10
#define SPI_INT_STA         0x14
#define SPI_FIFO_CTL        0x18
#define SPI_FIFO_STA        0x1C
#define SPI_WCR             0x20
#define SPI_CLK_CTL         0x24

#define SPI_BURST_CNT       0x30
#define SPI_XMIT_CNT        0x34
#define SPI_BURST_CTL_CNT   0x38
#define SPI_TXDATA     0x200
#define SPI_RXDATA     0x300

void SPI0_Init(void);
void     CE_Low(void);
void     CE_High(void);
void     SS0_Low(void);
void     SS0_High(void);
uint8_t SPI0_Read_Byte(uint8_t Address);
uint8_t SPI0_Read_Reg(uint8_t Reg);
void delay(void);
uint8_t Transfer_One(uint8_t Addr);
void Dummy_Byte(void);
#endif /* __SPILIB_H__*/
