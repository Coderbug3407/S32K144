#ifndef _HAL_SYSTICK_H_
#define _HAL_SYSTICK_H_

#ifdef __MPU_PRESENT
#undef __MPU_PRESENT

#define __MPU_PRESENT             0



#endif


void HAL_SysTick_Init(uint32_t tick);

void HAL_SysTick_Enable(void);

void HAL_SysTick_Disable(void);

#endif /*_HAL_SYSTICK_H_*/
