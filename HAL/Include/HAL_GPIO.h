#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_


typedef enum {
    HAL_PORT_A = 0,
    HAL_PORT_B,
    HAL_PORT_C,
    HAL_PORT_D,
    HAL_PORT_E
} hal_gpio_port_t;

/* Configure for Interrupt */
typedef enum {
    HAL_IRQ_DISABLE     = 0x0U,
    HAL_IRQ_RISING      = 0x9U,
    HAL_IRQ_FALLING     = 0xAU,
    HAL_IRQ_EITHER      = 0xBU
} hal_gpio_irq_trigger_t;

/* Configure Pull Register */
typedef enum {
    HAL_PULL_NONE       = 0U,
    HAL_PULL_DOWN       = 1U,
    HAL_PULL_UP         = 2U
} hal_gpio_pull_t;

/**
 * @fn          void HAL_GPIO_EnableClock(hal_gpio_port_t port);
 * @brief       Enable Clock for register
 * @param[in]   port    Port Index
 */
void HAL_GPIO_EnableClock(hal_gpio_port_t port);

/**
 * @fn          void HAL_GPIO_SetMuxMode(hal_gpio_port_t port, uint32_t pin, uint32_t muxmode);
 * @brief       Set Mux mode for Port
 * @param[in]   port    Port Index
 * @param[in]   pin     Pin Index
 * @param[in]   muxmode   mode for PORT(GPIO,...)
 */
void HAL_GPIO_SetMuxMode(hal_gpio_port_t port, uint32_t pin, uint32_t muxmode);

/**
 * @fn          void HAL_GPIO_SetDirection(hal_gpio_port_t port,uint32_t pin, uint32_t isOut);
 * @brief       Set Input/Output for pin
 * @param[in]   port    Port Index
 * @param[in]   pin     Pin Index
 * @param[in]   isOut   Set Input/Output for pin
 */
void HAL_GPIO_SetDirection(hal_gpio_port_t port,uint32_t pin, uint32_t isOut);

/**
 * @fn          void HAL_GPIO_WritePin(hal_gpio_port_t port, uint32_t pin,uint32_t level);
 * @brief       Write the level for pin
 * @param[in]   port    Port index
 * @param[in]   pin     Pin index
 * @param[in]   level   High/Low(1/0) State
 */
void HAL_GPIO_WritePin(hal_gpio_port_t port, uint32_t pin,uint32_t level);

/**
 * @fn          uint32_t HAL_GPIO_ReadPin(hal_gpio_port_t port, uint32_t pin);
 * @brief       Read state pin
 * @param[in]   port    Port index
 * @param[in]   pin     Pin index
 * @return      The state of pin
 */
uint32_t HAL_GPIO_ReadPin(hal_gpio_port_t port, uint32_t pin);

/**
 * @fn          void HAL_GPIO_SetPullResistor(hal_gpio_port_t port, uint32_t pin, hal_gpio_pull_t type);
 * @brief       Set Pull Register
 * @param[in]   port    Port index
 * @param[in]   pin     Pin index
 * @param[in]   type    Type of Pull Register
 */
void HAL_GPIO_SetPullResistor(hal_gpio_port_t port, uint32_t pin, hal_gpio_pull_t type);

/**
 * @fn          void HAL_GPIO_SetInterruptTrigger(hal_gpio_port_t port, uint32_t pin, hal_gpio_irq_trigger_t trigger);
 * @brief       Set Interrupt Trigger
 * @param[in]   port    Port index
 * @param[in]   pin     Pin index
 * @param[in]   trigger    Type of Trigger
 */
void HAL_GPIO_SetInterruptTrigger(hal_gpio_port_t port, uint32_t pin, hal_gpio_irq_trigger_t trigger);

/**
 * @fn          uint32_t HAL_GPIO_GetInterruptStatus(hal_gpio_port_t port, uint32_t pin);
 * @brief       Read Interrupt Status of port
 * @param[in]   port    Port index
 * @param[in]   pin     Pin index
 */
uint32_t HAL_GPIO_GetInterruptStatus(hal_gpio_port_t port, uint32_t pin);

/**
 * @fn          void HAL_GPIO_ClearInterruptFlag(hal_gpio_port_t port, uint32_t pin);
 * @brief       Clear Interrupt Flag
 * @param[in]   port    Port index
 * @param[in]   pin     Pin index
 */
void HAL_GPIO_ClearInterruptFlag(hal_gpio_port_t port, uint32_t pin);

#endif /*_HAL_GPIO_H_*/
