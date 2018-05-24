#ifndef __SPILIB_H__
#define __SPILIB_H__

#define BIT(nr)         (1UL << (nr))
#define SPI0_BASE   0x01C68000
#define SPI1_BASE   0x01C69000

// #define SPI_GBL_CTL     0x04
#define SPI_GBL_CTL_BUS_ENABLE  BIT(0)
#define SPI_GBL_CTL_MASTER      BIT(1)
#define SPI_GBL_CTL_TP          BIT(7)

#define SPI_TFR_CTL     0x08
#define SPI_TFR_CTL_CPHA         BIT(0)
#define SPI_TFR_CTL_CPOL         BIT(1)
#define SPI_TFR_CTL_SPOL         BIT(2)
#define SPI_TFR_CTL_CS_MASK         0x30
#define SPI_TFR_CTL_CS(cs)       (((cs) << 4) & SPI_TFR_CTL_CS_MASK)
#define SPI_TFR_CTL_CS_MANUAL       BIT(6)
#define SPI_TFR_CTL_CS_LEVEL        BIT(7)
#define SPI_TFR_CTL_DHB       BIT(8)
#define SPI_TFR_CTL_FBS       BIT(12)
#define SPI_TFR_CTL_SDM       BIT(13)
#define SPI_TFR_CTL_XCH       BIT(31)

#define SPI_RXFIFO_RESET   BIT(15)
#define SPI_TXFIFO_RESET   BIT(31)

typedef struct {
   uint32_t RESERVED0;
   volatile uint32_t SPI_GBL;          // 0x04
   volatile uint32_t SPI_TCR;          // 0x08
   uint32_t RESERVED1;
   volatile uint32_t SPI_INT_CTL;      // 0x10
   volatile uint32_t SPI_INT_STA;      // 0x14
   volatile uint32_t SPI_FIFO_CTL;     // 0x18
   volatile uint32_t SPI_FIFO_STA;     // 0x1C
   volatile uint32_t SPI_WCR;          // 0x20
   volatile uint32_t SPI_CLK_CTL;      // 0x24
   uint32_t RESERVED2[2];
   volatile uint32_t SPI_BURST_CNT;
   volatile uint32_t SPI_XMIT_CNT;
   volatile uint32_t SPI_BURST_CTL_CNT;
   uint32_t RESERVED3[113];
   volatile uint8_t SPI_TXDATA;
   uint8_t  RESERVED4[3];
   volatile uint32_t SPI_RXDATA;
} spi_reg_t;

#define SPI0 ((spi_reg_t *)(0x01C68000))
#define SPI1 ((spi_reg_t *)0x01C69000)

void SPI0_Init(void);
void     CE_Low(void);
void     CE_High(void);
void     CS0_Low(void);
void     CS0_High(void);
uint8_t SPI0_Read_Byte(uint8_t Address);
uint8_t SPI0_Read_Reg(uint8_t Reg);
void delay(void);
uint8_t Transfer_One(uint8_t Addr);
void Dummy_Byte(void);
#endif /* __SPILIB_H__*/
