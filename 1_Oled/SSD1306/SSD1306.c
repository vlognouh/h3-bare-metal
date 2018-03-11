//-----------------
#include "SSD1306.h"
#include "font_oled.h"

#define Set_DC() GPIO_SetBits(GPIOD, GPIO_Pin_11)
#define Clear_DC() GPIO_ResetBits(GPIOD, GPIO_Pin_11)

#define Set_RST() GPIO_SetBits(GPIOD, GPIO_Pin_9)
#define Clear_RST() GPIO_ResetBits(GPIOD, GPIO_Pin_9)

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
SPI_InitTypeDef   SPI_InitStructure;

void write_data(uint8_t data)
{
		Set_DC();
		/* Send the byte */
		SPI_I2S_SendData(SPI2, data);
		/* Wait until the transmit done */
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
}

void write_command(uint8_t cmd)
{
		Clear_DC();
		/* Send the byte */
		SPI_I2S_SendData(SPI2, cmd);
		/* Wait until the transmit done */
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
}

void OLED_SetPos(uint8_t ucIdxX, uint8_t ucIdxY)
{   
		write_command(0xb0 + ucIdxY);		
		write_command(ucIdxX & 0x0f);
		write_command(0x10 | (ucIdxX >> 4));
}

void OLED_Fill(char ucData)
{
    uint8_t ucPage, ucColumn;

    for(ucPage = 0; ucPage < 8; ucPage++)
    {
				OLED_SetPos(0x00, ucPage);
				for(ucColumn = 0; ucColumn < 129; ucColumn++)
				{
						write_data(ucData);
				}
    }
}

void SetStartColumn(uint8_t ucData)
{
    write_command(0x00+ucData % 16);   	// Set Lower Column Start Address for Page Addressing Mode
																					// Default => 0x00
    write_command(0x10+ucData / 16);   	// Set Higher Column Start Address for Page Addressing Mode
																					// Default => 0x10
}

void SetAddressingMode(uint8_t ucData)
{
    write_command(0x20);        // Set Memory Addressing Mode
    write_command(ucData);      // Default => 0x02
																	// 0x00 => Horizontal Addressing Mode
																	// 0x01 => Vertical Addressing Mode
																	// 0x02 => Page Addressing Mode
}

void SetColumnAddress(uint8_t a, uint8_t b)
{
    write_command(0x21);        // Set Column Address
    write_command(a);           // Default => 0x00 (Column Start Address)
    write_command(b);           // Default => 0x7F (Column End Address)
}

void SetPageAddress(uint8_t a, uint8_t b)
{
    write_command(0x22);        // Set Page Address
    write_command(a);           // Default => 0x00 (Page Start Address)
    write_command(b);           // Default => 0x07 (Page End Address)
}

void SetStartLine(uint8_t ucData)
{
    write_command(0x40|ucData); // Set Display Start Line
																// Default => 0x40 (0x00)
}

void SetContrastControl(uint8_t ucData)
{
    write_command(0x81);        // Set Contrast Control
    write_command(ucData);      // Default => 0x7F
}

void SetChargePump(uint8_t ucData)
{
    write_command(0x8D);        // Set Charge Pump
    write_command(0x10|ucData); // Default => 0x10
                            // 0x10 (0x00) => Disable Charge Pump
                            // 0x14 (0x04) => Enable Charge Pump
}

void SetSegmentRemap(uint8_t ucData)
{
    write_command(0xA0|ucData); // Set Segment Re-Map
                            // Default => 0xA0
                            // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
                            // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void SetEntireDisplay(uint8_t ucData)
{
    write_command(0xA4|ucData); // Set Entire Display On / Off
                            // Default => 0xA4
                            // 0xA4 (0x00) => Normal Display
                            // 0xA5 (0x01) => Entire Display On
}

void SetInverseDisplay(uint8_t ucData)
{
    write_command(0xA6|ucData); // Set Inverse Display On/Off
                            // Default => 0xA6
                            // 0xA6 (0x00) => Normal Display
                            // 0xA7 (0x01) => Inverse Display On
}

void SetMultiplexRatio(uint8_t ucData)
{
    write_command(0xA8);        // Set Multiplex Ratio
    write_command(ucData);      // Default => 0x3F (1/64 Duty)
}

void SetDisplayOnOff(uint8_t ucData)
{
    write_command(0xAE|ucData); // Set Display On/Off
                            // Default => 0xAE
                            // 0xAE (0x00) => Display Off
                            // 0xAF (0x01) => Display On
}

void SetStartPage(uint8_t ucData)
{
    write_command(0xB0|ucData); // Set Page Start Address for Page Addressing Mode
                            // Default => 0xB0 (0x00)
}

void SetCommonRemap(uint8_t ucData)
{
    write_command(0xC0|ucData); // Set COM Output Scan Direction
                            // Default => 0xC0
                            // 0xC0 (0x00) => Scan from COM0 to 63
                            // 0xC8 (0x08) => Scan from COM63 to 0
}

void SetDisplayOffset(uint8_t ucData)
{
    write_command(0xD3);        // Set Display Offset
    write_command(ucData);      // Default => 0x00
}

void SetDisplayClock(uint8_t ucData)
{
    write_command(0xD5);        // Set Display Clock Divide Ratio / Oscillator Frequency
    write_command(ucData);      // Default => 0x80
																	// D[3:0] => Display Clock Divider
																	// D[7:4] => Oscillator Frequency
}

void SetPrechargePeriod(uint8_t ucData)
{
    write_command(0xD9);        // Set Pre-Charge Period
    write_command(ucData);      // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                            // D[3:0] => Phase 1 Period in 1~15 Display Clocks
                            // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void SetCommonConfig(uint8_t ucData)
{
    write_command(0xDA);        // Set COM Pins Hardware Configuration
    write_command(0x02|ucData); // Default => 0x12 (0x10)
																	// Alternative COM Pin Configuration
																	// Disable COM Left/Right Re-Map
}

void SetVCOMH(uint8_t ucData)
{
    write_command(0xDB);        // Set VCOMH Deselect Level
    write_command(ucData);      // Default => 0x20 (0.77*VCC)
}

void SetNop(void)
{
    write_command(0xE3);        // Command for No Operation
}

void IO_Init(void)
{
	/* GPIOB Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN | SCL_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
		/* Configure DC pin */
	GPIO_InitStructure.GPIO_Pin = DC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DC_GPIO, &GPIO_InitStructure);

	/* Configure RST pin */
	GPIO_InitStructure.GPIO_Pin = RST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(RST_GPIO, &GPIO_InitStructure);
	
	/* SPI2 Config */
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_CalculateCRC(SPI2, DISABLE);

	/* SPI enable */
	SPI_Cmd(SPI2, ENABLE);
	
	/* drain SPI */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
}

/*****************************************************************************
OLED_Init
*****************************************************************************/
void OLED_Init(void)        
{
    uint16_t i;
    IO_Init();
	
    Clear_RST();
    for(i = 0; i <= 1000; i++);
		Set_RST();
	
    SetDisplayOnOff(0x00);    // Display Off (0x00/0x01)	@   
    SetMultiplexRatio(0x3F);  // 1/64 Duty (0x0F~0x3F)				@
    SetDisplayOffset(0x00);   // Shift Mapping RAM Counter (0x00~0x3F) @
    SetStartLine(0x00);       // Set Mapping RAM Display Start Line (0x00~0x3F)	@
		SetSegmentRemap(0x01);    // Set SEG/Column Mapping      @
		SetCommonRemap(0x08);     // Set COM/Row Scan Direction 	@
		SetCommonConfig(0x10);    // Set Sequential Configuration (0x00/0x10)  
		SetContrastControl(0x7f); // Set SEG Output Current
		SetEntireDisplay(0x00);   // Disable Entire Display On (0x00/0x01)		@
		SetInverseDisplay(0x00);  // Disable Inverse Display On (0x00/0x01)  @	
		SetDisplayClock(0x80);    // Set Clock as 100 Frames/Sec 	
		SetChargePump(0x04);      // Enable Embedded DC/DC Converter (0x00/0x04)			@	

		SetAddressingMode(0x02); 	// Set Page Addressing Mode (0x00/0x01/0x02) @
    SetPrechargePeriod(0xF1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    SetVCOMH(0x40);           // Set VCOM Deselect Level

    SetDisplayOnOff(0x01);    // Display On (0x00/0x01)	@
		
    OLED_Fill(0xff);          // Clear, 0xff = fill full
} 
 
/*****************************************************************************
OLED_P6x8Char
*****************************************************************************/
void OLED_P6x8Char(char ucData)
{
    uint8_t i, ucDataTmp;     
     
    ucDataTmp = ucData-32;
    for(i = 0; i < 6; i++)
    {
        write_data(F6x8[ucDataTmp][i]);
    }
}

/*****************************************************************************
OLED_PrintImage
*****************************************************************************/
void OLED_PrintImage(char ucData[], uint8_t page, uint8_t columm)
{
	uint16_t i,j;
	for(i = 0; i < page; i++)
	{
		OLED_SetPos(0x00, i);
		for(j = 0; j < columm; j++)
		{
			write_data(ucData[i*columm+j]);
		}
	}
}

void OLED_P8x16Str(uint8_t ucIdxX, uint8_t ucIdxY, char ucData[])
{
    uint8_t i, j, ucDataTmp;
    ucIdxY <<= 1;  //nhan 2
    for (j = 0; ucData[j] != '\0'; j++)
    {
        ucDataTmp = ucData[j] - 32;
        if(ucIdxX > 120)
        {
            ucIdxX = 0;
            ucIdxY += 2;
        }				
        OLED_SetPos(ucIdxX, ucIdxY);  
				
        for(i = 0; i < 8; i++) 
        {
            write_data(F8x16[ucDataTmp][i]);
        }
        OLED_SetPos(ucIdxX, ucIdxY + 1);   
				
        for(i = 0; i < 8; i++) 
        {
            write_data(F8x16[ucDataTmp][i + 8]);
        }
        ucIdxX += 8;
    }
    return;
}

/*-----------------------------------------Scroll------------------------------*/

// startscrollright
// Activate a right handed scroll for pages start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void startscrollright(uint8_t start, uint8_t stop)
{
  write_command(0x26);
  write_command(0x00);
  write_command(start);
  write_command(0x00);
  write_command(stop);
  write_command(0x00);
  write_command(0xFF);
  write_command(SSD1306_ACTIVATE_SCROLL);
}

// startscrollleft
// Activate a right handed scroll for pages start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void startscrollleft(uint8_t start, uint8_t stop)
{
  write_command(0x27);
  write_command(0x00);
  write_command(start);
  write_command(0x00);
  write_command(stop);
  write_command(0x00);
  write_command(0xFF);
  write_command(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagright
// Activate a diagonal scroll for pages start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void startscrolldiagright(uint8_t start, uint8_t stop)
{
  write_command(SSD1306_SET_VERTICAL_SCROLL_AREA);  
  write_command(0X00);
  write_command(SSD1306_LCDHEIGHT);
  write_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  write_command(0X00);
  write_command(start);
  write_command(0X00);
  write_command(stop);
  write_command(0X01);
  write_command(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagleft
// Activate a diagonal scroll for pages start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void startscrolldiagleft(uint8_t start, uint8_t stop){
  write_command(SSD1306_SET_VERTICAL_SCROLL_AREA);  
  write_command(0X00);
  write_command(SSD1306_LCDHEIGHT);
  write_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  write_command(0X00);
  write_command(start);
  write_command(0X00);
  write_command(stop);
  write_command(0X01);
  write_command(SSD1306_ACTIVATE_SCROLL);
}

void stopscroll(void)
{
  write_command(SSD1306_DEACTIVATE_SCROLL);
}
