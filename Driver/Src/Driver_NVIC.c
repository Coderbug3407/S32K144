#include "Driver_NVIC.h"

void DRIVER_NVIC_Init(IRQn_Type irq_number, uint32_t priority){
    /*Disable Interrupt*/
    HAL_NVIC_DisableIRQ(irq_number);
    /*Clear Pending Interrupt Flag*/
    HAL_NVIC_ClearPending(irq_number);
    /*Set Priority*/
    HAL_NVIC_SetPriority(irq_number, priority);
    /*Enable Interrupt*/
    HAL_NVIC_EnableIRQ(irq_number);
}

void DRIVER_NVIC_DeInit(IRQn_Type irq_number){
    /*Disable Interrupt*/
    HAL_NVIC_DisableIRQ(irq_number);
    /*Clear Pending Interrupt Flag*/
    HAL_NVIC_ClearPending(irq_number);
}

void DRIVER_NVIC_Reset(void){
    /*Reset System*/
    HAL_NVIC_SystemReset();
}
