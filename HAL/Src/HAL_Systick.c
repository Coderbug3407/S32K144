#include "S32K144.h"
#include "HAL_Systick.h"
#include "core_cm4.h"


#define SYSTICK_CTRL_ENABLE         (1U << 0)
#define SYSTICK_CTRL_TICKINIT       (1U << 1)
#define SYSTICK_CTRl_CLKSOURCE      (1U << 2)

void HAL_SysTick_Init(uint32_t tick){
    /*Disable CTRL Register before congig*/
    SysTick->CTRL   = 0;
    /*Load Tick*/
    SysTick->LOAD   = tick - 1;
    /*Delete value on VAL Register*/
    SysTick->VAL    = 0;
    /*Congig Register CTRL with TICKINT and CLKSOURCE*/
    SysTick->CTRL =  SYSTICK_CTRL_TICKINIT | SYSTICK_CTRl_CLKSOURCE;
}


void HAL_SysTick_Enable(void){
    SysTick->CTRL |= SYSTICK_CTRL_ENABLE;
}

void HAL_SysTick_Disable(void){
    SysTick->CTRL &= ~SYSTICK_CTRL_ENABLE;
}
