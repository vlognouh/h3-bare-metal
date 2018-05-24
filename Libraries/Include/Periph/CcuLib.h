#ifndef __CCULIB_H__
#define __CCULIB_H__

#define APB0_CLKGATE            0x01F01428

#define CCU_BASE                0x01C20000

#define PLL_PERIPH0             0x0028
#define PLL_PERIPH1             0x0044
#define CPUX_AXI_CFG_REG        0x0050
#define AHB1_APB1_CFG_REG       0x0054
#define APB2_CFG_REG            0x0058
#define AHB2_CFG_REG            0x005C
#define BUS_CLK_GATING_REG0     0x0060
#define SPI0_GATING             1 << 20

#define BUS_CLK_GATING_REG1     0x0064
#define BUS_CLK_GATING_REG2     0x0068
#define BUS_CLK_GATING_REG3     0x006C
#define TWI0_GATING             1 << 0
#define TWI1_GATING             1 << 1

#define BUS_CLK_GATING_REG4     0x0070

#define BUS_SPI0_CLK_REG        0x00A0
#define SPI_SCLK_GATING         1 << 31

#define BUS_SW_RESET_REG0       0x02C0
#define BUS_SPI0_RESET          1 << 20
#define BUS_SW_RESET_REG4       0x02D8

#endif
