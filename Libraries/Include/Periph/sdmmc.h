#ifndef __SDMMC_H__
#define __SDMMC_H__

typedef struct {
    volatile uint32_t SD_GCTL;          // 0x00
    volatile uint32_t SD_CKCR;          // 0x04
    volatile uint32_t SD_TMOR;          // 0x08
    volatile uint32_t SD_BWDR;          // 0x0c
    volatile uint32_t SD_BKSR;          // 0x10
    volatile uint32_t SD_BYCR;          // 0x14
    volatile uint32_t SD_CMDR;          // 0x18
    volatile uint32_t SD_CAGR;          // 0x1c
    volatile uint32_t SD_RESP0;         // 0x20
    volatile uint32_t SD_RESP1;         // 0x24
    volatile uint32_t SD_RESP2;         // 0x28
    volatile uint32_t SD_RESP3;         // 0x2c
    volatile uint32_t SD_IMKR;          // 0x30
    volatile uint32_t SD_MISR;          // 0x34
    volatile uint32_t SD_RISR;          // 0x38
    volatile uint32_t SD_STAR;          // 0x3c
    volatile uint32_t SD_FWLR;          // 0x40
    volatile uint32_t SD_FUNS;          // 0x44
    uint32_t RESERVED0[4];
    volatile uint32_t SD_A12A;          // 0x58
    volatile uint32_t SD_NTSR;          // 0x5c
    volatile uint32_t SD_SDBG;          // 0x60
    uint32_t RESERVED1[5];
    volatile uint32_t SD_HWRST;         // 0x78
    uint32_t RESERVED2;
    volatile uint32_t SD_DMAC;          // 0x80
    volatile uint32_t SD_DLBA;          // 0x84
    volatile uint32_t SD_IDST;          // 0x88
    volatile uint32_t SD_IDIE;          // 0x8c
    uint32_t RESERVED3[29];
    volatile uint32_t SD_THLDC;         // 0x100
    uint32_t RESERVED4[2];
    volatile uint32_t SD_DSBD;          // 0x10c
    volatile uint32_t SD_RES_CRC;       // 0x110
    volatile uint32_t SD_DATA7_CRC;     // 0x114
    volatile uint32_t SD_DATA6_CRC;     // 0x118
    volatile uint32_t SD_DATA5_CRC;     // 0x11c    
    volatile uint32_t SD_DATA4_CRC;     // 0x120
    volatile uint32_t SD_DATA3_CRC;     // 0x124
    volatile uint32_t SD_DATA2_CRC;     // 0x128
    volatile uint32_t SD_DATA1_CRC;     // 0x12c
    volatile uint32_t SD_DATA0_CRC;     // 0x130
    volatile uint32_t SD_CRC_STA;       // 0x134
    uint32_t RESERVED5[50];
    volatile uint32_t SD_FIFO;
} sdmmc_reg_t;

#define SDMMC0 ((sdmmc_reg_t *)(0x01C0F000))
#define SDMMC1 ((sdmmc_reg_t *)(0x01C10000))
#define SDMMC2 ((sdmmc_reg_t *)(0x01C11000))

#define MMC_DATA_READ       1
#define MMC_DATA_WRITE      2

#define MMC_CMD_GO_IDLE_STATE       0
#define MMC_CMD_SEND_OP_COND        1
#define MMC_CMD_ALL_SEND_CID        2
#define MMC_CMD_SET_RELATIVE_ADDR   3
#define MMC_CMD_SET_DSR         4
#define MMC_CMD_SWITCH          6
#define MMC_CMD_SELECT_CARD     7
#define MMC_CMD_SEND_EXT_CSD        8
#define MMC_CMD_SEND_CSD        9
#define MMC_CMD_SEND_CID        10
#define MMC_CMD_STOP_TRANSMISSION   12
#define MMC_CMD_SEND_STATUS     13
#define MMC_CMD_SET_BLOCKLEN        16
#define MMC_CMD_READ_SINGLE_BLOCK   17
#define MMC_CMD_READ_MULTIPLE_BLOCK 18
#define MMC_CMD_SEND_TUNING_BLOCK       19
#define MMC_CMD_SEND_TUNING_BLOCK_HS200 21
#define MMC_CMD_SET_BLOCK_COUNT         23
#define MMC_CMD_WRITE_SINGLE_BLOCK  24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK    25
#define MMC_CMD_ERASE_GROUP_START   35
#define MMC_CMD_ERASE_GROUP_END     36
#define MMC_CMD_ERASE           38
#define MMC_CMD_APP_CMD         55
#define MMC_CMD_SPI_READ_OCR        58
#define MMC_CMD_SPI_CRC_ON_OFF      59
#define MMC_CMD_RES_MAN         62

#define MMC_CMD62_ARG1          0xefac62ec
#define MMC_CMD62_ARG2          0xcbaea7

#define SD_CMD_SEND_RELATIVE_ADDR   3
#define SD_CMD_SWITCH_FUNC      6
#define SD_CMD_SEND_IF_COND     8
#define SD_CMD_SWITCH_UHS18V        11

#define SD_CMD_APP_SET_BUS_WIDTH    6
#define SD_CMD_APP_SD_STATUS        13
#define SD_CMD_ERASE_WR_BLK_START   32
#define SD_CMD_ERASE_WR_BLK_END     33
#define SD_CMD_APP_SEND_OP_COND     41
#define SD_CMD_APP_SEND_SCR     51

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136 (1 << 1)        /* 136 bit response */
#define MMC_RSP_CRC (1 << 2)        /* expect valid crc */
#define MMC_RSP_BUSY    (1 << 3)        /* card may send busy */
#define MMC_RSP_OPCODE  (1 << 4)        /* response contains opcode */

#define MMC_RSP_NONE    (0)
#define MMC_RSP_R1  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
            MMC_RSP_BUSY)
#define MMC_RSP_R2  (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3  (MMC_RSP_PRESENT)
#define MMC_RSP_R4  (MMC_RSP_PRESENT)
#define MMC_RSP_R5  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7  (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define MMC_VDD_32_33       0x00100000  /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34       0x00200000  /* VDD voltage 3.3 ~ 3.4 */

#define SUNXI_MMC_CMD_RESP_EXPIRE   (0x1 << 6)
#define SUNXI_MMC_CMD_LONG_RESPONSE (0x1 << 7)
#define SUNXI_MMC_CMD_CHK_RESPONSE_CRC  (0x1 << 8)
#define SUNXI_MMC_CMD_WITH_DATA   (0x1 << 9)
#define SUNXI_MMC_CMD_WRITE     (0x1 << 10)
#define SUNXI_MMC_CMD_AUTO_STOP     (0x1 << 12)
#define SUNXI_MMC_CMD_WAIT_PRE_OVER (0x1 << 13)
#define SUNXI_MMC_CMD_SEND_INIT_SEQ (0x1 << 15)
#define SUNXI_MMC_CMD_UPCLK_ONLY    (0x1 << 21)
#define SUNXI_MMC_CMD_START     (0x1 << 31)

#define SUNXI_MMC_RINT_RESP_ERROR       (0x1 << 1)
#define SUNXI_MMC_RINT_COMMAND_DONE     (0x1 << 2)
#define SUNXI_MMC_RINT_DATA_OVER        (0x1 << 3)
#define SUNXI_MMC_RINT_TX_DATA_REQUEST      (0x1 << 4)
#define SUNXI_MMC_RINT_RX_DATA_REQUEST      (0x1 << 5)
#define SUNXI_MMC_RINT_RESP_CRC_ERROR       (0x1 << 6)
#define SUNXI_MMC_RINT_DATA_CRC_ERROR       (0x1 << 7)
#define SUNXI_MMC_RINT_RESP_TIMEOUT     (0x1 << 8)
#define SUNXI_MMC_RINT_DATA_TIMEOUT     (0x1 << 9)
#define SUNXI_MMC_RINT_VOLTAGE_CHANGE_DONE  (0x1 << 10)
#define SUNXI_MMC_RINT_FIFO_RUN_ERROR       (0x1 << 11)
#define SUNXI_MMC_RINT_HARD_WARE_LOCKED     (0x1 << 12)
#define SUNXI_MMC_RINT_START_BIT_ERROR      (0x1 << 13)
#define SUNXI_MMC_RINT_AUTO_COMMAND_DONE    (0x1 << 14)
#define SUNXI_MMC_RINT_END_BIT_ERROR        (0x1 << 15)
#define SUNXI_MMC_RINT_SDIO_INTERRUPT       (0x1 << 16)
#define SUNXI_MMC_RINT_CARD_INSERT      (0x1 << 30)
#define SUNXI_MMC_RINT_CARD_REMOVE      (0x1 << 31)
#define SUNXI_MMC_RINT_INTERRUPT_ERROR_BIT      \
    (SUNXI_MMC_RINT_RESP_ERROR |        \
     SUNXI_MMC_RINT_RESP_CRC_ERROR |    \
     SUNXI_MMC_RINT_DATA_CRC_ERROR |    \
     SUNXI_MMC_RINT_RESP_TIMEOUT |      \
     SUNXI_MMC_RINT_DATA_TIMEOUT |      \
     SUNXI_MMC_RINT_VOLTAGE_CHANGE_DONE |   \
     SUNXI_MMC_RINT_FIFO_RUN_ERROR |    \
     SUNXI_MMC_RINT_HARD_WARE_LOCKED |  \
     SUNXI_MMC_RINT_START_BIT_ERROR |   \
     SUNXI_MMC_RINT_END_BIT_ERROR) /* 0xbfc2 */
#define SUNXI_MMC_RINT_INTERRUPT_DONE_BIT   \
    (SUNXI_MMC_RINT_AUTO_COMMAND_DONE | \
     SUNXI_MMC_RINT_DATA_OVER |     \
     SUNXI_MMC_RINT_COMMAND_DONE |      \
     SUNXI_MMC_RINT_VOLTAGE_CHANGE_DONE)

#define SUNXI_MMC_GCTRL_SOFT_RESET  (0x1 << 0)
#define SUNXI_MMC_GCTRL_FIFO_RESET  (0x1 << 1)
#define SUNXI_MMC_GCTRL_DMA_RESET   (0x1 << 2)
#define SUNXI_MMC_GCTRL_RESET       (SUNXI_MMC_GCTRL_SOFT_RESET|\
                     SUNXI_MMC_GCTRL_FIFO_RESET|\
                     SUNXI_MMC_GCTRL_DMA_RESET)
#define SUNXI_MMC_GCTRL_DMA_ENABLE  (0x1 << 5)
#define SUNXI_MMC_GCTRL_ACCESS_BY_AHB   (0x1 << 31)

#define SUNXI_MMC_STATUS_RXWL_FLAG      (0x1 << 0)
#define SUNXI_MMC_STATUS_TXWL_FLAG      (0x1 << 1)
#define SUNXI_MMC_STATUS_FIFO_EMPTY     (0x1 << 2)
#define SUNXI_MMC_STATUS_FIFO_FULL      (0x1 << 3)
#define SUNXI_MMC_STATUS_CARD_PRESENT       (0x1 << 8)
#define SUNXI_MMC_STATUS_CARD_DATA_BUSY     (0x1 << 9)
#define SUNXI_MMC_STATUS_DATA_FSM_BUSY      (0x1 << 10)

void SD_Test(void);

#endif //__SDMMC_H__