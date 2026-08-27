#ifndef _DRIVER_NVIC_H_
#define _DRIVER_NVIC_H_

#include "HAL_NVIC.h"

void DRIVER_NVIC_Init(IRQn_Type irq_number, uint32_t priority);

void DRIVER_NVIC_DeInit(IRQn_Type irq_number);

void DRIVER_NVIC_Reset();



#endif/*_DRIVER_NVIC_H_*/
