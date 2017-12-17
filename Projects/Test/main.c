/**
 * 2017 Orange Pi One Tianocore
 * File: SerialPortLib.c
 * nhoangvubk@gmail.com
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

#include "helper.h"

#include "Library/SerialPortLib.h"
#include "Library/TimerLib.h"
#include "Library/GpioLib.h"
#include "Library/CcuLib.h"
#include "Library/I2cLib.h"
#include "Library/SpiLib.h"

/**
  * @brief  Definition
  */
#undef errno
extern int errno;
#define STACK_BUFFER 65536 /* Reserved stack space in bytes. */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
void *_sbrk (int nbytes)
{
    /* Symbol defined by linker map */
    extern int end; /* start of free memory (as symbol) */
    extern int HeapLimit; /* end of free memory */

    /* The statically held previous end of the heap, with its initialization. */
    static void *heap_ptr = (void *)&end; /* Previous end */
    static void *limit = (void *)&HeapLimit;

    if ((limit - (heap_ptr + nbytes)) > STACK_BUFFER )
    {
        void *base = heap_ptr;
        heap_ptr += nbytes;
        return base;
    }
    else
    {
        errno = ENOMEM;
        printf("=> ENOMEM\r\n");
        return (void *) - 1;
    }
} /* _sbrk () */

/*-------------------- SYSCALL DEFINITION ------------------*/
int fwrite (int file,
            char *buf,
            int nbytes)
{
    int i;
    SerialPortWrite((uint8_t *)buf, nbytes);
    return nbytes;
} /* _write () */

int _read(int file) { return -1; }
int _close(int file) { return -1; }
int _fstat(int file) { return -1; }
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }


/** MAIN FUNC 
*******************/
int main(int argc, char const *argv[])
{
    uint Temp;
    uint8_t value;
    uint8_t data[2];
    uint32_t i;

    int *pa, *pb;

    // extern int end;
    // extern int HeapLimit;
    char Text[25];
    WriteReg(GPIO_BASE, PA_CONFIG1_OFFSET, ReadReg(GPIO_BASE, PA_CONFIG1_OFFSET) & (~(6 << 28)));
    WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) | (1 << 15));
    for (i = 1000; i > 0; i--);
    WriteReg(APB0_CLKGATE, 0, 3);

    WriteReg(PL_BASE, PL_CONFIG1_OFFSET, ReadReg(PL_BASE, PL_CONFIG1_OFFSET) & (~(6 << 8)));
    WriteReg(PL_BASE, PL_DATA_OFFSET, ReadReg(PL_BASE, PL_DATA_OFFSET) | (1 << 10));
    char *s = "Hello  Everyone 24!!\r\n";
    SerialPortWrite((uint8_t *)s, strlen(s));
    // printf("=> end 0x%p\r\n", &end);
    // printf("=> limit 0x%p\r\n", &HeapLimit);
    pa = malloc(200);
    pb = malloc(100);
    printf("==== printf Worked: %x\r\n", pa);
    printf("==== printf Worked: %x\r\n", pb);
    Timer0Init();
    free(pa);
//    WriteReg(CCU_BASE, PLL_PERIPH0, 1<<31 | 5<<8 | 2<<4 | 2);
//    while (!(ReadReg(CCU_BASE, PLL_PERIPH0) & 1<<28));
    Temp = ReadReg(CCU_BASE, PLL_PERIPH0);
    sprintf(Text, "--- PLL0= 0x%x\r\n", Temp);
    SerialPortWrite((uint8_t *)Text, strlen(Text));

//    WriteReg(CCU_BASE, PLL_PERIPH1, 1<<31 | 5<<8 | 2<<4 | 2);
//    while (!(ReadReg(CCU_BASE, PLL_PERIPH1) & 1<<28));
    Temp = ReadReg(CCU_BASE, PLL_PERIPH1);
    sprintf(Text, "--- PLL1= 0x%x\r\n", Temp);
    SerialPortWrite((uint8_t *)Text, strlen(Text));

//    WriteReg(CCU_BASE, AHB1_APB1_CFG_REG, 2<<12 | 2<<8 | 2<<6);
    Temp = ReadReg(CCU_BASE, AHB1_APB1_CFG_REG);
    sprintf(Text, "--- AHB1= 0x%x\r\n", Temp);
    SerialPortWrite((uint8_t *)Text, strlen(Text));

////// I2C
#if 0
    I2cInitialize(0);
    I2cRead(0xD0, 0x75, data, 1);
    sprintf(Text, "Who I Am = %d\r\n", data[0]);
    SerialPortWrite((uint8_t *)Text, strlen(Text));
    data[0] = 0x23;
    I2cWrite(0xD0, 0x6B, data, 1);

    I2cRead(0xD0, 0x6B, data, 1);
    sprintf(Text, "MPU6050 Addr:0x6B = %d\r\n", data[0]);
    SerialPortWrite((uint8_t *)Text, strlen(Text));
#endif
#if 0
////// SPI
    SPI0_Init();
    Dummy_Byte(); // => to correct SPI_SCK polarity
    value = Transfer_One(0x00);
    sprintf(Text, "SPI Reg 0x0? = 0x%x\r\n", value);
    SerialPortWrite((uint8_t *)Text, strlen(Text));

    value = Transfer_One(0x01);
    sprintf(Text, "SPI Reg 0x1? = 0x%x\r\n", value);
    SerialPortWrite((uint8_t *)Text, strlen(Text));

    value = Transfer_One(0x02);
    sprintf(Text, "SPI Reg 0x2? = 0x%x\r\n", value);
    SerialPortWrite((uint8_t *)Text, strlen(Text));

    // Print APB2 Config Register
    SerialPortWrite((uint8_t *)"---\r\n", 5);
    Temp = ReaadReg(CCU_BASE, APB2_CFG_REG);
    sprintf(Text, "APB2 Reg = 0x%x\r\n", Temp);
    SerialPortWrite(Text, strlen(Text));
#endif
    while (1)
    {
        i = ReadReg(TIMER_BASE, TMR0_CUR_VALUE_REG_OFFSET);
        if (i == 0x2DC00)
            WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) | (1 << 10));
        if (i == 0x50000)
            WriteReg(GPIO_BASE, PA_DATA_OFFSET, ReadReg(GPIO_BASE, PA_DATA_OFFSET) & ~(1 << 10));
    }
    return 0;
}

void test_un(void)
{
    SerialPortWrite((uint8_t *)"U\r\n", 3);
}

void test_sw(void)
{
    SerialPortWrite((uint8_t *)"SW\r\n", 4);
}

void test_pr(void)
{
    SerialPortWrite((uint8_t *)"P\r\n", 3);
}

void test_da(void)
{
    SerialPortWrite((uint8_t *)"D\r\n", 3);
}

void test_fi(void)
{
    SerialPortWrite((uint8_t *)"F\r\n", 3);
}

void test_ir(void)
{
    SerialPortWrite((uint8_t *)"I\r\n", 3);
}
