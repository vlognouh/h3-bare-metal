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

struct mmc_data {
    union {
        char *dest;
        const char *src; /* src buffers don't get written to */
    };
    uint flags;
    uint blocks;
    uint blocksize;
};

uint8_t send_cmd(struct mmc_cmd *cmd, struct mmc_data *data)
{
    uint32_t cmdval = SUNXI_MMC_CMD_START;
    uint8_t err = 0;

    if (cmd->resp_type & MMC_RSP_BUSY)
        printf("mmc cmd %ld check rsp busy\n", cmd->cmdidx);
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

    if (data)
    {
        if ((uint32_t)(long)data->dest & 0x3) {
            err = -1;
            goto out;
        }

        cmdval |= SUNXI_MMC_CMD_WITH_DATA | SUNXI_MMC_CMD_WAIT_PRE_OVER;
        if (data->flags & MMC_DATA_WRITE)
            cmdval |= SUNXI_MMC_CMD_WRITE;
        if (data->blocks > 1)
            cmdval |= SUNXI_MMC_CMD_AUTO_STOP;
        SDMMC0->SD_BKSR = data->blocksize;
        SDMMC0->SD_BYCR = data->blocksize * data->blocks;
    }

    // Write CMD ARG
    SDMMC0->SD_CAGR = cmd->cmdarg;

    // printf("SDMMC0 Raw interrupt status 0x%x\n", SDMMC0->SD_RISR);

    // Write CMD reg
    SDMMC0->SD_CMDR = cmdval | cmd->cmdidx;

    // Wait for CMD Done
    while (!((SDMMC0->SD_RISR) & SUNXI_MMC_RINT_COMMAND_DONE))
    {
        if ((SDMMC0->SD_RISR) & SUNXI_MMC_RINT_INTERRUPT_ERROR_BIT)
        {
            printf("-- ERR\n");
            printf("SDMMC0 Raw interrupt status 0x%lx\n", SDMMC0->SD_RISR);
            err = 1;
            goto out;
        }
    }
    // printf("SDMMC0 Raw interrupt status 0x%x\n", SDMMC0->SD_RISR);

    //
    if (cmd->resp_type & MMC_RSP_BUSY) {
        while (SDMMC0->SD_STAR & SUNXI_MMC_STATUS_CARD_DATA_BUSY);
    }

    if (cmd->resp_type & MMC_RSP_136) {
        cmd->response[0] = SDMMC0->SD_RESP3;
        cmd->response[1] = SDMMC0->SD_RESP2;
        cmd->response[2] = SDMMC0->SD_RESP1;
        cmd->response[3] = SDMMC0->SD_RESP0;
        printf("mmc resp 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n",
               cmd->response[3], cmd->response[2],
               cmd->response[1], cmd->response[0]);
    } else {
        cmd->response[0] = SDMMC0->SD_RESP0;
        printf("mmc resp 0x%08lx\n", cmd->response[0]);
    }

    if (data)
    {
        // send data to fifo
        const int reading = !!(data->flags & MMC_DATA_READ);
        const uint32_t status_bit = reading ? SUNXI_MMC_STATUS_FIFO_EMPTY :
                                    SUNXI_MMC_STATUS_FIFO_FULL;
        uint32_t i;
        uint32_t *buff = (uint32_t *)(reading ? data->dest : data->src);
        uint32_t byte_cnt = data->blocksize * data->blocks;
        uint32_t timeout = 1000000;

        /* Always read / write data through the CPU */
        SDMMC0->SD_GCTL |= (1 << 31);
        printf("Byte count %d\n", byte_cnt);
        printf("/// Status reg 0x%lx\n", SDMMC0->SD_STAR);
        for (i = 0; i < (byte_cnt >> 2); i++) {
            while ((SDMMC0->SD_STAR) & status_bit)
            {
                if (!(timeout--))
                {
                    err = 1;
                    goto out;
                }
            }

            if (reading)
            {
                // printf("%d\n", i);
                buff[i] = SDMMC0->SD_FIFO;
            }
            else
                SDMMC0->SD_FIFO = buff[i];
        }
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
    SDMMC0->SD_RISR = 0x0001FFFF;
    SDMMC0->SD_GCTL |= 1 << 1;
    while (SDMMC0->SD_GCTL & 2);
    return err;
}

void SD_Test(void)
{
    struct mmc_cmd cmd;
    struct mmc_data data;
    uint16_t addr;

    printf("----------------------------------\n");

    // Reset
    SDMMC0->SD_GCTL = 2;
    while (SDMMC0->SD_GCTL & 2);

    // Send CMD0
    cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_NONE;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    // Send CMD8
    cmd.cmdidx = SD_CMD_SEND_IF_COND;
    cmd.cmdarg = 0x1aa;
    cmd.resp_type = MMC_RSP_R7;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    };

    // Send ACMD41
    for (int i = 0; i < 10; ++i)
    {
        // Send CMD55
        cmd.cmdidx = MMC_CMD_APP_CMD;
        cmd.resp_type = MMC_RSP_R1;
        cmd.cmdarg = 0;
        if (send_cmd(&cmd, NULL))
        {
            printf("ERR: %ld\n", cmd.cmdidx);
            return;
        }

        // Send CMD41
        cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
        cmd.resp_type = MMC_RSP_R3;
        cmd.cmdarg |= 0x40000000 | MMC_VDD_32_33 | MMC_VDD_33_34;
        if (send_cmd(&cmd, NULL))
        {
            printf("ERR: %ld\n", cmd.cmdidx);
            return;
        }

        // reponse OCR (table 5-1 page 161 spec v6)
        if (cmd.response[0] & 0x80000000)
        {
            // MSB of reponse indicate sdcard capacity F => normal capacity, C => high cap
            printf("Break OP CMD loop\n");
            break;
        }
        for (int i = 0; i < 100000; ++i)
        {
            /* code */
        }
    }

// CMD2 to read CID
    /* Put the Card in Identify Mode */
    cmd.cmdidx = MMC_CMD_ALL_SEND_CID; /* cmd not supported in spi */
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = 0;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

// CMD3 get sd addr, CHANGE from ident -> stand-by mode
    cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
    cmd.cmdarg = 0;
    cmd.resp_type = MMC_RSP_R6;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    addr = (cmd.response[0] >> 16) & 0xFFFF;
    printf("Relative addr is %d\n", addr);

    /* Get the Card-Specific Data */
    cmd.cmdidx = MMC_CMD_SEND_CSD;
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = addr << 16;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    uint32_t read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);
    printf("Read block len = %ld\n", read_bl_len);

// Select card, CHANGE from stand-by to trans mode
    cmd.cmdidx = MMC_CMD_SELECT_CARD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = addr << 16;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

// Set bus width
    // Send CMD55
    printf("Set bus width\n");
    cmd.cmdidx = MMC_CMD_APP_CMD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = addr << 16;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 2; // 4-bit
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

// Get Card Status CMD13
    printf("SS Read Card Status\n");
    // Select card
    cmd.cmdidx = MMC_CMD_SEND_STATUS;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = addr << 16;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    // set block len
    cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 64;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

// Get SD Status ACMD13
    // Send CMD55
    cmd.cmdidx = MMC_CMD_APP_CMD;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = addr << 16;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    // Send CMD13
    printf("SS Read SD Status\n");
    cmd.cmdidx = SD_CMD_APP_SD_STATUS;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 0;

    uint32_t ssr[16];
    uint32_t timeout = 3;

retry:
    data.dest = (char *)ssr;
    data.blocksize = 64;
    data.blocks = 1;
    data.flags = MMC_DATA_READ;
    if (send_cmd(&cmd, &data))
    {
        if (timeout--)
        {
            printf("-- Retry %d\n", timeout);
            goto retry;
        }

        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    for (int i = 0; i < 16; ++i)
    {
        printf("-> %d\n", ssr[i]);
    }


//----
    // Init done, try to read a block

    // set block len
    cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 512;
    if (send_cmd(&cmd, NULL))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    printf("Send Read CMD\n");

    cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;
    cmd.resp_type = MMC_RSP_R1;
    cmd.cmdarg = 8192;

    uint32_t dst[256];
    data.dest = (char *)dst;
    data.blocks = 1;
    data.blocksize = 512;
    data.flags = MMC_DATA_READ;

    if (send_cmd(&cmd, &data))
    {
        printf("ERR: %ld\n", cmd.cmdidx);
        return;
    }

    for (int i = 0; i < 128; ++i)
    {
        printf("-> 0x%08x\n", dst[i]);
    }
}
