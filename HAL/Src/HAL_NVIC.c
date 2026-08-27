#include "HAL_NVIC.h"


#include "core_cm4.h"

void HAL_NVIC_EnableIRQ(IRQn_Type irq_number)
{
    NVIC_EnableIRQ(irq_number);
}

void HAL_NVIC_DisableIRQ(IRQn_Type irq_number)
{
    NVIC_DisableIRQ(irq_number);
}

void HAL_NVIC_SetPriority(IRQn_Type irq_number, uint32_t priority)
{
    NVIC_SetPriority(irq_number, priority);
}

void HAL_NVIC_ClearPending(IRQn_Type irq_number)
{
    NVIC_ClearPendingIRQ(irq_number);
}

void HAL_NVIC_SystemReset(void){
    NVIC_SystemReset();
}
