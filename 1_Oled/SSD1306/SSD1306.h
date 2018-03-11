/******************************************************************************
SSD1306 Driver OOLED 128x64
******************************************************************************/


#ifndef __OOLED_H__
#define __OOLED_H__

#include "stm32f4xx.h"

#define OLED_IMAGE_WHITE       1
#define OLED_IMAGE_BLACK       0

#define OLED_MAX_ROW_NUM      64
#define OLED_MAX_COLUMN_NUM  128

//-----------------------------------
#define SPI_SD SPI2;
#define SPI_CLK RCC_APB1Periph_SPI2;

#define SPI_GPIO GPIOB
#define SCL_PIN GPIO_Pin_13 	//SPI2 SCK
#define SDA_PIN	GPIO_Pin_15		//SPI2_MOSI

#define RST_GPIO GPIOD			
#define RST_PIN GPIO_Pin_9

#define DC_GPIO GPIOD
#define DC_PIN GPIO_Pin_11

#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

//-----------------------------------
void IO_Init(void);
void write_data(uint8_t data);
void write_command(uint8_t cmd);
void SetStartColumn(uint8_t ucData);
void SetAddressingMode(uint8_t ucData);
void SetColumnAddress(uint8_t a, uint8_t b);
void SetPageAddress(uint8_t a, uint8_t b);
void SetStartLine(uint8_t ucData);
void SetContrastControl(uint8_t ucData);
void SetChargePump(uint8_t ucData);
void SetSegmentRemap(uint8_t ucData);
void SetEntireDisplay(uint8_t ucData);
void SetInverseDisplay(uint8_t ucData);
void SetMultiplexRatio(uint8_t ucData);
void SetDisplayOnOff(uint8_t ucData);
void SetStartPage(uint8_t ucData);
void SetCommonRemap(uint8_t ucData);
void SetDisplayOffset(uint8_t ucData);
void SetDisplayClock(uint8_t ucData);
void SetPrechargePeriod(uint8_t ucData);
void SetCommonConfig(uint8_t ucData);
void SetVCOMH(uint8_t ucData);
void SetNop(void);

void OLED_Init(void);
void OLED_SetPos(uint8_t ucIdxX, uint8_t ucIdxY); 

void OLED_P6x8Char(char ucData);
void OLED_P12x8Char(char ucData);
void OLED_Fill(char ucData);
void OLED_PrintImage(char ucData[], uint8_t page, uint8_t columm);

void OLED_P8x16Str(uint8_t ucIdxX, uint8_t ucIdxY, char ucData[]);

void startscrollright(uint8_t start, uint8_t stop);
void startscrollleft(uint8_t start, uint8_t stop);
void startscrolldiagright(uint8_t start, uint8_t stop);
void startscrolldiagleft(uint8_t start, uint8_t stop);
void stopscroll(void);
#endif

