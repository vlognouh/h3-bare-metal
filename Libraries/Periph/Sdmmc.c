#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Periph/sdmmc.h>

struct mmc_cmd {
    uint32_t cmdidx;
    uint32_t resp_type;
    uint32_t cmdarg;
    uint32_t response[4];
};

uint8_t send_cmd(struct mmc_cmd *cmd)
{
    uint32_t cmdval = SUNXI_MMC_CMD_START;
    uint8_t err = 0;

    if (cmd->resp_type & MMC_RSP_BUSY)
        printf("mmc cmd %ld check rsp busy\r\n", cmd->cmdidx);
    if (cmd->cmdidx == 12)
        return 0;

    if (!cmd->cmdidx)
        cmdval |= SUNXI_MMC_CMD_SEND_INIT_SEQ;
    if (cmd->resp_type & MMC_RSP_PRESENT)
        cmdval |= SUNXI_MMC_CMD_RESP_EXPIRE;
    if (cmd->resp_type & MMC_RSP_136)
        cmdval |= SUNXI_MMC_CMD_LONG_RESPONSE;
    if (cmd->resp_type & MMC_RSP_CRC)
        cmdval |= SUNXI_MMC_CMD_CHK_RESPONSE_CRC;

    // Write CMD ARG
    SDMMC0->SD_CAGR = cmd->cmdarg;

    // printf("SDMMC0 Raw interrupt status 0x%x\r\n", SDMMC0->SD_RISR);

    // Write CMD reg
    SDMMC0->SD_CMDR = cmdval | cmd->cmdidx;

    // Wait for CMD Done
    while (!((SDMMC0->SD_RISR) & SUNXI_MMC_RINT_COMMAND_DONE))
    {
        if ((SDMMC0->SD_RISR) & SUNXI_MMC_RINT_INTERRUPT_ERROR_BIT)
        {
            printf("-- ERR\r\n");
            printf("SDMMC0 Raw interrupt status 0x%x\r\n", SDMMC0->SD_RISR);
            err = 1;
            goto out;
        }
    }
    // printf("SDMMC0 Raw interrupt status 0x%x\r\n", SDMMC0->SD_RISR);

    //
    if (cmd->resp_type & MMC_RSP_BUSY) {
        while (SDMMC0->SD_STAR & SUNXI_MMC_STATUS_CARD_DATA_BUSY);
    }

    if (cmd->resp_type & MMC_RSP_136) {
        cmd->response[0] = SDMMC0->SD_RESP3;
        cmd->response[1] = SDMMC0->SD_RESP2;
        cmd->response[2] = SDMMC0->SD_RESP1;
        cmd->response[3] = SDMMC0->SD_RESP0;
        printf("mmc resp 0x%08lx 0x%08lx 0x%08lx 0x%08lx\r\n",
               cmd->response[3], cmd->response[2],
               cmd->response[1], cmd->response[0]);
    } else {
        cmd->response[0] = SDMMC0->SD_RESP0;
        printf("mmc resp 0x%08lx\r\n", cmd->response[0]);
    }

out:
    if (err)
    {
        SDMMC0->SD_GCTL = (1 << 2) | (1 << 1) | 1;
        SDMMC0->SD_CMDR = SUNXI_MMC_CMD_START |
                          SUNXI_MMC_CMD_UPCLK_ONLY |
                          SUNXI_MMC_CMD_WAIT_PRE_OVER;

        while (SDMMC0->SD_CMDR & SUNXI_MMC_CMD_START);
    }

    // Clear int
    SDMMC0->SD_RISR = 0xFFFFFFFF;
    SDMMC0->SD_GCTL |= 1 << 1;

    return err;
}

void SD_Test(void)
{
    struct mmc_cmd cmd;
    uint8_t err, addr;

    printf("----------------------------------\r\n");

    SDMMC0->SD_CMDR = SUNXI_MMC_CMD_START |
                      SUNXI_MMC_CMD_UPCLK_ONLY |
                      SUNXI_MMC_CMD_WAIT_PRE_OVER;

    SDMMC0->SD_RISR = 0xFFFFFFFF;

    // Send CMD0
    cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_NONE;
    if (send_cmd(&cmd))
    {
        printf("ERR: $d\n", cmd.cmdidx);
        return;
    }

    // Send CMD8
    cmd.cmdidx = SD_CMD_SEND_IF_COND;
    cmd.cmdarg = 0x1aa;
    cmd.resp_type = MMC_RSP_R7;
    if (send_cmd(&cmd))
    {
        printf("ERR: %d\n", cmd.cmdidx);
        return;
    };

    // Send ACMD41
    for (int i = 0; i < 10; ++i)
    {
        // Send CMD55
        cmd.cmdidx = MMC_CMD_APP_CMD;
        cmd.resp_type = MMC_RSP_R1;
        cmd.cmdarg = 0;
        if (send_cmd(&cmd))
        {
            printf("ERR: $d\n", cmd.cmdidx);
            return;
        }

        // Send CMD41
        cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
        cmd.resp_type = MMC_RSP_R3;
        cmd.cmdarg |= 0x40000000 | MMC_VDD_32_33 | MMC_VDD_33_34;
        if (send_cmd(&cmd))
        {
            printf("ERR: $d\n", cmd.cmdidx);
            return;
        }

        // reponse OCR (table 5-1 page 161 spec v6)
        if (cmd.response[0] & 0x80000000)
        {
            // MSB of reponse indicate sdcard capacity F => normal capacity, C => high cap
            printf("Break OP CMD loop\r\n");
            break;
        }
        for (int i = 0; i < 100000; ++i)
        {
            /* code */
        }
    }

    // CMD11
    // cmd.cmdidx = SD_CMD_SWITCH_UHS18V;
    // cmd.cmdarg = 0;
    // cmd.resp_type = MMC_RSP_R1;
    // send_cmd(&cmd);

    // CMD2 to read CID
    /* Put the Card in Identify Mode */
    cmd.cmdidx = MMC_CMD_ALL_SEND_CID; /* cmd not supported in spi */
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = 0;
    if (send_cmd(&cmd))
    {
        printf("ERR: $d\n", cmd.cmdidx);
        return;
    }

    // CMD3 get sd addr
    cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_R6;
    if (send_cmd(&cmd))
    {
        printf("ERR: $d\n", cmd.cmdidx);
        return;
    }

    addr = (cmd.response[0] >> 16) & 0xFFFF;
    printf("Relative addr is %d\r\n", addr);

    // Select card
    cmd.cmdidx = MMC_CMD_SELECT_CARD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = addr << 16;
    if (send_cmd(&cmd))
    {
        printf("ERR: $d\n", cmd.cmdidx);
        return;
    }

    // Init done, try to read a block

    // set block len
    cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 512;
    if (send_cmd(&cmd))
    {
        printf("ERR: $d\n", cmd.cmdidx);
        return;
    }

    cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;
    cmd.resp_type = MMC_RSP_R1;

    data.dest = dst;
    data.blocks = blkcnt;
    data.blocksize = mmc->read_bl_len;
    data.flags = MMC_DATA_READ;


}

void read_block(uint32_t block)
{
    
}