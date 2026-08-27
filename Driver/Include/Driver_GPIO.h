#ifndef _DRIVER_GPIO_H_
#define _DRIVER_GPIO_H_

#include "Driver_Common.h"


/********************************************************************************************
* Definitions                                                                               *
*********************************************************************************************/

/**
 * @brief Specified Pin not available.
 * The pin specified is not available.
 */
#define ARM_GPIO_ERROR_PIN   (ARM_DRIVER_ERROR_SPECIFIC - 1)

#define ARM_GPIO_EVENT_RISING_EDGE    (1UL << 0)             /*Rising-edge detected*/
#define ARM_GPIO_EVENT_FALLING_EDGE   (1UL << 1)			 /*Falling-edge detected*/
#define ARM_GPIO_EVENT_EITHER_EDGE    (1UL << 2)             /*Either edge detected (only when hardware cannot distinguish between rising and falling edge)*/

/********************************************************************************************
* Typedefs & Enumerations                                                                   *
*********************************************************************************************/

/* GPIO pin */
typedef uint32_t ARM_GPIO_Pin_t;

/*Pointer to ARM_GPIO_SignalEvent*/
typedef void(*ARM_GPIO_SignalEvent_t) (ARM_GPIO_Pin_t pin, uint32_t event);

/**
* @brief GPIO Direction
* Specifies values for setting the direction.
*/
typedef enum{
    ARM_GPIO_INPUT,              /*Input (default)*/
    ARM_GPIO_OUTPUT              /*Output*/
}ARM_GPIO_DIRECTION;

/**
 * @brief GPIO Output Mode
 * Specifies values for setting the output mode.
 */
typedef enum {
    ARM_GPIO_PUSH_PULL,                       /*Push-pull (default)*/
    ARM_GPIO_OPEN_DRAIN                       /*Open-drain*/
} ARM_GPIO_OUTPUT_MODE;

/**
 * @brief GPIO Pull Register
 */
typedef enum {
    ARM_GPIO_PULL_NONE,                       /*None (default)*/
    ARM_GPIO_PULL_UP ,                        /*Pull-up*/
    ARM_GPIO_PULL_DOWN                        /*Pull-down*/
}ARM_GPIO_PULL_RESISTOR;

/**
 * @brief GPIO Event Trigger.
 * Specifies values for setting the event trigger.
 */

typedef enum {
    ARM_GPIO_TRIGGER_NONE,                   /*None (default*/
    ARM_GPIO_TRIGGER_RISING_EDGE,            /*Rising-edge*/
    ARM_GPIO_TRIGGER_FALLING_EDGE,           /*Falling-edge*/
    ARM_GPIO_TRIGGER_EITHER_EDGE             /*Either edge (rising and falling)*/
}ARM_GPIO_EVENT_TRIGGER;


/*Function Documentation */
/**
 * @fn           int32_t(*Setup)(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
 * @brief        Setup GPIO Interface.
 * @param[in]    pin        GPIO Pin
 * @param[in]    cb_event   Pointer to ARM_GPIO_SignalEvent
 * @return       Status Error Codes
 */

/**
 * @fn           int32_t(*SetDirection)(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
 * @brief        Set GPIO Direction
 * @param[in]    pin        GPIO Pin
 * @param[in]    direction  ARM_GPIO_DIRECTION
 * @return       Status Error Codes
 */

 /**
  * @fn         int32_t(*SetOutputMode)(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
  * @brief      Set GPIO Output Mode
  * @param[in]  pin     GPIO Pin
  * @param[in]  mode    ARM_GPIO_OUTPUT_MODE
  * @return     Status Error Codes
  */

 /**
  * @fn        int32_t(*SetPullResistor)(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
  * @brief     Set GPIO Pull Resistor
  * @param[in] pin         GPIO Pin
  * @param[in] resistor    ARM_GPIO_PULL_RESISTOR
  * @return    Status Error Codes
  */

/**
 * @fn         int32_t(*SetEventTrigger)(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
 * @brief      Set GPIO Event Trigger
 * @param[in]  pin      GPIO Pin
 * @param[in]  trigger  ARM_GPIO_EVENT_TRIGGER
 * @return     Status Error Codes
 */

/**
 * @fn         void(*SetOutput )(ARM_GPIO_Pin_t pin, uint32_t val)
 * @brief      Set GPIO Output Level
 * @param[in]  pin  GPIO Pin
 * @param[in]  val  GPIO Pin Level (0 or 1)
 * @return     Status Error Codes
 */

/**
 * @fn         uint32_t(*GetInput )(ARM_GPIO_Pin_t pin)
 * @brief      Get GPIO Input Level
 * @param[in]  pin GPIO Pin
 * @return     GPIO Pin Level (0 or 1)
 */

/**
 * @fn         uint32_t(*GetInterruptStatus)(ARM_GPIO_Pin_t pin);
 * @brief      Get the Interrupt Status
 * @param[in]  pin GPIO Pin
 * @return     Level (0 or 1)
 */

/**
 * @fn         void (*ClearInterruptFlag)(ARM_GPIO_Pin_t pin);
 * @brief      Clear the Interrupt Status
 * @param[in]  pin GPIO Pin
 * @return     Level (0 or 1)
 */

/**
 * @fn          int32_t (*SetAlternateFunction)(ARM_GPIO_Pin_t pin, uint32_t muxMode)
 * @brief       Setup other function for pin(other mux mode)
 * @param[in]   pin         GPIO pin
 * @param[in]   muxMode     mux mode index
 */
/**
 * @brief Access structure of the GPIO Driver.
 */
typedef struct _ARM_DRIVER_GPIO {
    int32_t(*Setup)            (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);         /*Pointer to ARM_GPIO_Setup: Setup GPIO Interface*/
    int32_t(*SetDirection)     (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);            /*Pointer to ARM_GPIO_SetDirection: Set GPIO Direction*/
    int32_t(*SetOutputMode)    (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);               /*Pointer to ARM_GPIO_SetOutputMode: Set GPIO Output Mode*/
    int32_t(*SetPullResistor)  (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);         /*Pointer to ARM_GPIO_SetPullResistor: Set GPIO Pull Resistor*/
    int32_t(*SetEventTrigger)  (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);          /*Pointer to ARM_GPIO_SetEventTrigger: Set GPIO Event Trigger*/
    void(*SetOutput )          (ARM_GPIO_Pin_t pin, uint32_t val);                            /*Pointer to ARM_GPIO_SetOutput: Set GPIO Output Level*/
    uint32_t(*GetInput )       (ARM_GPIO_Pin_t pin);                                          /*Pointer to ARM_GPIO_GetInput : Get GPIO Input Level*/
    uint32_t(*GetInterruptStatus)   (ARM_GPIO_Pin_t pin);
    void (*ClearInterruptFlag)      (ARM_GPIO_Pin_t pin);
    void (*SetAnalogFunction)(ARM_GPIO_Pin_t pin);
    int32_t (*SetAlternateFunction)(ARM_GPIO_Pin_t pin, uint32_t muxMode);
}const ARM_DRIVER_GPIO;

#endif /*_DRIVER_GPIO_H_*/
