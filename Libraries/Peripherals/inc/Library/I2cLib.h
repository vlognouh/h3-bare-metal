#ifndef __I2CLIB_H__
#define __I2CLIB_H__

#define TWI0_BASE 0x01C2AC00
#define TWI1_BASE 0x01C2B000
#define TWI2_BASE 0x01C2B400

#define TWI_ADDR_OFFSET     0x0000
#define TWI_XADDR_OFFSET    0x0004
#define TWI_DATA_OFFSET     0x0008

#define TWI_CNTR_OFFSET     0x000C
#define TWI_CNTR_INT_EN     1<<7
#define TWI_CNTR_BUS_EN     1<<6
#define TWI_CNTR_M_START    1<<5
#define TWI_CNTR_M_STOP     1<<4
#define TWI_CNTR_INTF       1<<3
#define TWI_CNTR_A_ACK      1<<2

#define TWI_STAT_OFFSET     0x0010
#define	TWI_STAT_BUS_ERR			0x00
#define	TWI_STAT_START			    0x08
#define	TWI_STAT_REPEAT_START		0x10
#define	TWI_STAT_WR_ADDR_ACK		0x18
#define	TWI_STAT_WR_ADDR_NO_ACK		0x20
#define	TWI_STAT_WR_ACK			    0x28
#define	TWI_STAT_WR_NO_ACK		    0x30
#define	TWI_STAT_LOST_ARB		    0x38
#define	TWI_STAT_RD_ADDR_ACK		0x40
#define	TWI_STAT_RD_ADDR_NO_ACK		0x48
#define	TWI_STAT_RD_DATA_ACK		0x50
#define	TWI_STAT_RD_DATA_NO_ACK		0x58
#define	TWI_STAT_WR_ADDR_2_ACK		0xd0
#define	TWI_STAT_WR_ADDR_2_NO_ACK	0xd8
#define	TWI_STAT_RD_ADDR_2_ACK		0xe0
#define	TWI_STAT_RD_ADDR_2_NO_ACK	0xe8
#define	TWI_STAT_NO_STATUS			0xf8

#define TWI_CCR_OFFSET      0x0014
#define TWI_SRST_OFFSET     0x0018
#define TWI_EFR_OFFSET      0x001C
#define TWI_LCR_OFFSET      0x0020

void I2cInitialize(uint8_t I2cBus);
void I2cGenerateStart(void);
void I2cGenerateStop(void);
void I2cRead(uint8_t chip_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);
void I2cWrite(uint8_t chip_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);
#endif /*__I2CLIB_H___*/
