#ifndef __GICLIB_H__
#define __GICLIB_H__

#define GICD_BASE           0x01c81000
#define GICD_CTRL           0x000
#define GICD_IGROUPR_1      0x084
#define GICD_ISENABLER_1    0x104
#define GICD_ICENABLER_1    0x184
#define GICD_IPRIORITYR_12  0x430
#define GICD_ITARGETSR_12   0x830
#define GICD_ICPENDR_1      0x284
#define GICD_ICACTIVER_1    0x384

#define GICD_ICFGR_3        0xC0C
#define GICD_SPISR_2        0xD08

#define GICC_BASE           0x01c82000
#define GICC_CTLR           0x000
#define GICC_PMR            0x004
#define GICC_BPR            0x008
#define GICC_IAR            0x000C
#define GICC_EOIR           0x0010

typedef struct
{
    uint32_t D_CTLR;                 /*!< \brief  Offset: 0x000 (R/W) Distributor Control Register */
    uint32_t D_TYPER;                /*!< \brief  Offset: 0x004 (R/ ) Interrupt Controller Type Register */
    uint32_t D_IIDR;                 /*!< \brief  Offset: 0x008 (R/ ) Distributor Implementer Identification Register */
    uint32_t RESERVED0;
    uint32_t D_STATUSR;              /*!< \brief  Offset: 0x010 (R/W) Error Reporting Status Register, optional */
    uint32_t RESERVED1[11];
    uint32_t D_SETSPI_NSR;           /*!< \brief  Offset: 0x040 ( /W) Set SPI Register */
    uint32_t RESERVED2;
    uint32_t D_CLRSPI_NSR;           /*!< \brief  Offset: 0x048 ( /W) Clear SPI Register */
    uint32_t RESERVED3;
    uint32_t D_SETSPI_SR;            /*!< \brief  Offset: 0x050 ( /W) Set SPI, Secure Register */
    uint32_t RESERVED4;
    uint32_t D_CLRSPI_SR;            /*!< \brief  Offset: 0x058 ( /W) Clear SPI, Secure Register */
    uint32_t RESERVED5[9];
    uint32_t D_IGROUPR[32];          /*!< \brief  Offset: 0x080 (R/W) Interrupt Group Registers */
    uint32_t D_ISENABLER[32];        /*!< \brief  Offset: 0x100 (R/W) Interrupt Set-Enable Registers */
    uint32_t D_ICENABLER[32];        /*!< \brief  Offset: 0x180 (R/W) Interrupt Clear-Enable Registers */
    uint32_t D_ISPENDR[32];          /*!< \brief  Offset: 0x200 (R/W) Interrupt Set-Pending Registers */
    uint32_t D_ICPENDR[32];          /*!< \brief  Offset: 0x280 (R/W) Interrupt Clear-Pending Registers */
    uint32_t D_ISACTIVER[32];        /*!< \brief  Offset: 0x300 (R/W) Interrupt Set-Active Registers */
    uint32_t D_ICACTIVER[32];        /*!< \brief  Offset: 0x380 (R/W) Interrupt Clear-Active Registers */
    uint8_t  D_IPRIORITYR[1020];     /*!< \brief  Offset: 0x400 (R/W) Interrupt Priority Registers */
    uint32_t RESERVED9[1];
    uint8_t  D_ITARGETSR[1020];      /*!< \brief  Offset: 0x800 (R/W) Interrupt Targets Registers */
    uint32_t RESERVED10[1];
    uint32_t D_ICFGR[64];            /*!< \brief  Offset: 0xC00 (R/W) Interrupt Configuration Registers */
    uint32_t D_IGRPMODR[32];         /*!< \brief  Offset: 0xD00 (R/W) Interrupt Group Modifier Registers */
    uint32_t RESERVED11[32];
    uint32_t D_NSACR[64];            /*!< \brief  Offset: 0xE00 (R/W) Non-secure Access Control Registers */
    uint32_t D_SGIR;                 /*!< \brief  Offset: 0xF00 ( /W) Software Generated Interrupt Register */
    uint32_t RESERVED13[3];
    uint8_t  D_CPENDSGIR[16];        /*!< \brief  Offset: 0xF10 (R/W) SGI Clear-Pending Registers */
    uint8_t  D_SPENDSGIR[16];        /*!< \brief  Offset: 0xF20 (R/W) SGI Set-Pending Registers */
    uint32_t RESERVED14[5236];
    uint64_t D_IROUTER[988];         /*!< \brief  Offset: 0x6100(R/W) Interrupt Routing Registers */
}  GICDistributor_Type;

#define GICDistributor      ((GICDistributor_Type      *)     GICD_BASE ) /*!< GIC Distributor configuration struct */

/** \brief  Structure type to access the Generic Interrupt Controller Interface (GICC)
*/
typedef struct
{
    uint32_t C_CTLR;                 /*!< \brief  Offset: 0x000 (R/W) CPU Interface Control Register */
    uint32_t C_PMR;                  /*!< \brief  Offset: 0x004 (R/W) Interrupt Priority Mask Register */
    uint32_t C_BPR;                  /*!< \brief  Offset: 0x008 (R/W) Binary Point Register */
    uint32_t C_IAR;                  /*!< \brief  Offset: 0x00C (R/ ) Interrupt Acknowledge Register */
    uint32_t C_EOIR;                 /*!< \brief  Offset: 0x010 ( /W) End Of Interrupt Register */
    uint32_t C_RPR;                  /*!< \brief  Offset: 0x014 (R/ ) Running Priority Register */
    uint32_t C_HPPIR;                /*!< \brief  Offset: 0x018 (R/ ) Highest Priority Pending Interrupt Register */
    uint32_t C_ABPR;                 /*!< \brief  Offset: 0x01C (R/W) Aliased Binary Point Register */
    uint32_t C_AIAR;                 /*!< \brief  Offset: 0x020 (R/ ) Aliased Interrupt Acknowledge Register */
    uint32_t C_AEOIR;                /*!< \brief  Offset: 0x024 ( /W) Aliased End Of Interrupt Register */
    uint32_t C_AHPPIR;               /*!< \brief  Offset: 0x028 (R/ ) Aliased Highest Priority Pending Interrupt Register */
    uint32_t C_STATUSR;              /*!< \brief  Offset: 0x02C (R/W) Error Reporting Status Register, optional */
    uint32_t RESERVED15[40];
    uint32_t C_APR[4];               /*!< \brief  Offset: 0x0D0 (R/W) Active Priority Register */
    uint32_t C_NSAPR[4];             /*!< \brief  Offset: 0x0E0 (R/W) Non-secure Active Priority Register */
    uint32_t RESERVED17[3];
    uint32_t C_IIDR;                 /*!< \brief  Offset: 0x0FC (R/ ) CPU Interface Identification Register */
    uint32_t RESERVED18[960];
    uint32_t C_DIR;                  /*!< \brief  Offset: 0x1000( /W) Deactivate Interrupt Register */
}  GICInterface_Type;

#define GICInterface        ((GICInterface_Type        *)     GICC_BASE )   /*!< GIC Interface configuration struct */

typedef enum IRQn
{
    /******  SGI Interrupts Numbers                 ****************************************/
    SGI0_IRQn           =  0,
    SGI1_IRQn           =  1,
    SGI2_IRQn           =  2,
    SGI3_IRQn           =  3,
    SGI4_IRQn           =  4,
    SGI5_IRQn           =  5,
    SGI6_IRQn           =  6,
    SGI7_IRQn           =  7,
    SGI8_IRQn           =  8,
    SGI9_IRQn           =  9,
    SGI10_IRQn          = 10,
    SGI11_IRQn          = 11,
    SGI12_IRQn          = 12,
    SGI13_IRQn          = 13,
    SGI14_IRQn          = 14,
    SGI15_IRQn          = 15,

    /******  Cortex-A7 Processor Exceptions Numbers ****************************************/
    GlobalTimer_IRQn    = 27,        /*!< Global Timer Interrupt                        */
    PrivTimer_IRQn      = 29,        /*!< Private Timer Interrupt                       */
    PrivWatchdog_IRQn   = 30,        /*!< Private Watchdog Interrupt                    */

    /******  Allwinner H3 Platform Exceptions Numbers **************************************/
    UART0_IRQn          = 32,
    UART1_IRQn          = 33,
    UART2_IRQn          = 34,
    UART3_IRQn          = 35,
    TWI0_IRQn           = 38,
    TWI1_IRQn           = 39,
    TWI2_IRQn           = 40,
    PA_EINT_IRQn        = 43,
    OWA_IRQn            = 44,
    I2S0_IRQn           = 45,
    I2S1_IRQn           = 46,
    I2S2_IRQn           = 47,
    PG_EINT             = 49,
    Timer0_IRQn         = 50,
    Timer1_IRQn         = 51,
} IRQn_Type;

//
static inline void GIC_EnableDistributor(void)
{
    GICDistributor->D_CTLR |= 1;
}

static inline void GIC_DisableDistributor(void)
{
    GICDistributor->D_CTLR &=~1;
}

static inline uint32_t GIC_DistributorInfo(void)
{
    return (uint32_t)(GICDistributor->D_TYPER);
}

static inline void GIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    GICDistributor->D_IPRIORITYR[((uint32_t)(int32_t)IRQn)] = (uint8_t)(priority);
}

static inline uint32_t GIC_GetPriority(IRQn_Type IRQn)
{
    return((uint32_t)GICDistributor->D_IPRIORITYR[((uint32_t)(int32_t)IRQn)]);
}

static inline void GIC_SetTarget(IRQn_Type IRQn, uint32_t cpu_target)
{
    GICDistributor->D_ITARGETSR[((uint32_t)(int32_t)IRQn)] = (uint8_t)(cpu_target & 0x0f);
}

static inline void GIC_EnableIRQ(IRQn_Type IRQn)
{
    /** Assign directly, without OR with current value ??
        Because write 0 has no effect
        In order to disable the IRQ, write 1 to ICENABLER
     */
    GICDistributor->D_ISENABLER[IRQn / 32] = 1 << (IRQn % 32);
}

static inline void GIC_DisableIRQ(IRQn_Type IRQn)
{
    GICDistributor->D_ICENABLER[IRQn / 32] = 1 << (IRQn % 32);
}

//
static inline void GIC_EnableInterface(void)
{
    GICInterface->C_CTLR |= 1; //enable interface
}

static inline void GIC_DisableInterface(void)
{
    GICInterface->C_CTLR &=~1; //disable distributor
}

static inline void GIC_SetBinaryPoint(uint32_t binary_point)
{
    GICInterface->C_BPR = binary_point & 0x07; //set binary point
}

static inline void GIC_SetInterfacePriorityMask(uint32_t priority)
{
    GICInterface->C_PMR = priority & 0xff; //set priority mask
}

void GIC_DistInit(void);
void GIC_CPUInterfaceInit(void);

void GicInit(void);

#endif /* __GICLIB_H__ */
