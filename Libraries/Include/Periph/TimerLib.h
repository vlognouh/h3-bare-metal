/**
 * 2017 Orange Pi One BSP
 * File: TimerLib.h
 * nhoangvubk@gmail.com
 **/

#ifndef __TIMERLIB_H__
#define __TIMERLIB_H__

/**
  * @brief  Timer Number enumeration
  */
typedef enum
{
    Timer0 = 0,
    Timer1
} TimerNumTypeDef;

/**
  * @brief  Timer Mode enumeration
  */
typedef enum
{
    Continuous = 0,
    Single
} TimerModeTypeDef;

/**
  * @brief  Timer Clock Source enumeration
  */
typedef enum
{
    Internal = 0,
    OSC24M
} TimerClkTypeDef;

/**
  * @brief  Timer Prescaler enumeration
  */
typedef enum
{
    Prescaler_1 = 0,
    Prescaler_2,
    Prescaler_4,
    Prescaler_8,
    Prescaler_16,
    Prescaler_32,
    Prescaler_64,
    Prescaler_128,
} TimerPrescalerTypeDef;

/**
  * @brief  Allwinner H3 Registers
  */
#define TIMER_BASE 					0x01C20C00

#define TMR_IRQ_EN_REG_OFFSET		0x0
#define TMR_IRQ_STA_REG_OFFSET		0x4
#define TMR0_CTRL_REG_OFFSET		0x10
#define TMR0_INTV_VALUE_REG_OFFSET	0x14
#define TMR0_CUR_VALUE_REG_OFFSET	0x18
#define TMR1_CTRL_REG_OFFSET		0x20
#define TMR1_INTV_VALUE_REG_OFFSET	0x24
#define TMR1_CUR_VALUE_REG_OFFSET	0x28
#define AVS_CNT_CTL_REG_OFFSET		0x80
#define AVS_CNT0_REG_OFFSET			0x84
#define AVS_CNT1_REG_OFFSET			0x88
#define AVS_CNT_DIV_REG_OFFSET		0x8C
#define WDOG0_IRQ_EN_REG_OFFSET		0xA0
#define WDOG0_IRQ_STA_REG_OFFSET	0xA4
#define WDOG0_CTRL_REG_OFFSET		0xB0
#define WDOG0_CFG_REG_OFFSET		0xB4
#define WDOG0_MODE_REG_OFFSET		0xB8

/**
  * @brief  Timer Struct
  */
struct Timer
{
    TimerNumTypeDef 		TimerNum;
    TimerModeTypeDef 		TimerMode;
    TimerClkTypeDef 		TimerClk;
    TimerPrescalerTypeDef TimerPrescaler;
};

/**
  * @brief  Timer API Prototype
  */
void
Timer0Init (
    void
);

#endif /* __TIMERLIB_H__ */
