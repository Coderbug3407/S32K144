#ifndef _HAL_NVIC_H_
#define _HAL_NVIC_H_
#include "S32K144.h"

#ifdef __MPU_PRESENT
#undef __MPU_PRESENT

#define __MPU_PRESENT             0



#endif


/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
void HAL_NVIC_EnableIRQ(IRQn_Type irq_number);

/**
  \brief   Disable Interrupt
  \details Disables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
void HAL_NVIC_DisableIRQ(IRQn_Type irq_number);

/**
  \brief   Set Interrupt Priority
  \details Sets the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]      IRQn  Interrupt number.
  \param [in]  priority  Priority to set.
  \note    The priority cannot be set for every processor exception.
 */
void HAL_NVIC_SetPriority(IRQn_Type irq_number, uint32_t priority);

/**
  \brief   Clear Pending Interrupt
  \details Clears the pending bit of a device specific interrupt in the NVIC pending register.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
void HAL_NVIC_ClearPending(IRQn_Type irq_number);

/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
void HAL_NVIC_SystemReset(void);

#endif /*_HAL_NVIC_H_*/




