#include <stdint.h>

#define str(x) #x
#define BIT(nr)			(1UL << (nr))

static void inline
WriteReg(
    uint32_t Base,
    uint32_t Offset,
    uint32_t Data
)
{
    *(uint32_t *)(Base + Offset) = Data;
}

static void inline
WriteByte(
    uint32_t Base,
    uint32_t Offset,
    uint8_t Data
)
{
    *(uint8_t *)(Base + Offset) = Data;
}

static uint8_t inline
ReadByte(
    uint32_t Base,
    uint32_t Offset
)
{
    return *(uint8_t *)(Base + Offset);
}

static uint16_t inline
ReadHalf(
    uint32_t Base,
    uint32_t Offset
)
{
    return *(uint16_t *)(Base + Offset);
}

static uint32_t inline
ReadReg(
    uint32_t Base,
    uint32_t Offset
)
{
    return *(uint32_t *)(Base + Offset);
}

static void inline
WriteCheck(
    uint32_t Base,
    uint32_t Offset,
    uint32_t Data
)
{
    *(uint32_t *)(Base + Offset) = Data;
    while((ReadReg(Base, Offset) ^ Data));
}

static inline void __irq_enable(void)
{
    uint32_t tmp;

    __asm__ __volatile__(
        "mrs %0, cpsr\n"
        "bic %0, %0, #(1<<7)\n"
        "msr cpsr_cxsf, %0"
        : "=r" (tmp)
        :
        : "memory");
}

static inline uint32_t __read_cpu_id(void)
{
    uint32_t value;

    __asm__ __volatile__(
        "mrc p15, 0, %0, c0, c0, 5"
        : "=r" (value)
        :
        : "memory");

    return value;
}
