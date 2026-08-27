#include "S32K144.h"
#include "Driver_GPIO.h"
#include "HAL_GPIO.h"

/*******************************************************************************
* Definitions
******************************************************************************/

/* Pin Mapping */
#define GPIO_MAX_PINS         160u
#define PIN_IS_AVAILABLE(n)   ((n) < GPIO_MAX_PINS)

/* Macro to calculate Port index and Pin index */
#define GET_PORT_INDEX(pin)   ((pin) / 32U)    /* Divide by 32 */
#define GET_PIN_INDEX(pin)    ((pin) % 32U)    /* Modulo 32 */

/*******************************************************************************
* Prototypes
******************************************************************************/

static int32_t GPIO_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
static int32_t GPIO_SetDirection(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);
static int32_t GPIO_SetOutputMode(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);
static int32_t GPIO_SetPullResistor(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);
static int32_t GPIO_SetEventTrigger(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);
static void GPIO_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);
static uint32_t GPIO_GetInput(ARM_GPIO_Pin_t pin);
static uint32_t GPIO_GetInterruptStatus(ARM_GPIO_Pin_t pin);
static void GPIO_ClearInterruptFlag(ARM_GPIO_Pin_t pin);
static void GPIO_SetAnalogFunction(ARM_GPIO_Pin_t pin);
static int32_t GPIO_SetAlternateFunction(ARM_GPIO_Pin_t pin, uint32_t muxMode);

/*******************************************************************************
* Variables
******************************************************************************/

/* Exposed to Linker as "Driver_GPIO0" (Must match external expectation in main.c) */
ARM_DRIVER_GPIO Driver_GPIO0 = {
   GPIO_Setup,
   GPIO_SetDirection,
   GPIO_SetOutputMode,
   GPIO_SetPullResistor,
   GPIO_SetEventTrigger,
   GPIO_SetOutput,
   GPIO_GetInput,
   GPIO_GetInterruptStatus,
   GPIO_ClearInterruptFlag,
   GPIO_SetAnalogFunction,
   GPIO_SetAlternateFunction
};

/*******************************************************************************
* Code
******************************************************************************/

static uint32_t GPIO_GetInput(ARM_GPIO_Pin_t pin)
{
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;
    uint32_t val = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex = GET_PIN_INDEX(pin);
        val = HAL_GPIO_ReadPin(portIndex, pinIndex);
    }

    return val;
}

static void GPIO_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex  = GET_PIN_INDEX(pin);
        if (val != 0U)
        {
            HAL_GPIO_WritePin(portIndex, pinIndex, val);
        }
        else
        {
            HAL_GPIO_WritePin(portIndex, pinIndex, val);
        }
    }
}

static int32_t GPIO_SetDirection(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
{
    int32_t result = ARM_DRIVER_OK;
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex  = GET_PIN_INDEX(pin);
        switch (direction)
        {
            case ARM_GPIO_OUTPUT:
                HAL_GPIO_SetDirection(portIndex, pinIndex, direction);
                break;
            case ARM_GPIO_INPUT:
                HAL_GPIO_SetDirection(portIndex, pinIndex, direction);
                break;
            default:
                result = ARM_DRIVER_ERROR;
                break;
        }
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

static int32_t GPIO_SetOutputMode(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
{
    /* Not used return OK */
    return ARM_DRIVER_OK;
}

static int32_t GPIO_SetEventTrigger(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{
    int32_t result = ARM_DRIVER_OK;
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex = GET_PIN_INDEX(pin);

        switch (trigger)
        {
            case ARM_GPIO_TRIGGER_NONE:
                HAL_GPIO_SetInterruptTrigger(portIndex, pinIndex, HAL_IRQ_DISABLE);
                break;
            case ARM_GPIO_TRIGGER_RISING_EDGE:
                HAL_GPIO_SetInterruptTrigger(portIndex, pinIndex, HAL_IRQ_RISING);
                break;
            case ARM_GPIO_TRIGGER_FALLING_EDGE:
                HAL_GPIO_SetInterruptTrigger(portIndex, pinIndex, HAL_IRQ_FALLING);
                break;
            case ARM_GPIO_TRIGGER_EITHER_EDGE:
                HAL_GPIO_SetInterruptTrigger(portIndex, pinIndex, HAL_IRQ_EITHER);
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

static int32_t GPIO_SetPullResistor(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
    int32_t result = ARM_DRIVER_OK;
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex = GET_PIN_INDEX(pin);

        switch (resistor)
        {
            case ARM_GPIO_PULL_NONE:
                HAL_GPIO_SetPullResistor(portIndex, pinIndex, HAL_PULL_NONE);
                break;
            case ARM_GPIO_PULL_UP:
                HAL_GPIO_SetPullResistor(portIndex, pinIndex, HAL_PULL_UP);
                break;
            case ARM_GPIO_PULL_DOWN:
                HAL_GPIO_SetPullResistor(portIndex, pinIndex, HAL_PULL_DOWN);
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

static int32_t GPIO_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
    int32_t result = ARM_DRIVER_OK;
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex  = GET_PIN_INDEX(pin);

        /* Enable clock for PORT */
        HAL_GPIO_EnableClock(portIndex);

        /* Enable GPIO mode with MUX[1] */
        HAL_GPIO_SetMuxMode(portIndex, pinIndex, 1U);
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

static uint32_t GPIO_GetInterruptStatus(ARM_GPIO_Pin_t pin)
{
    hal_gpio_port_t port = (hal_gpio_port_t)0U;
    uint32_t pinIdx = 0U;
    uint32_t status = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        port = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIdx = GET_PIN_INDEX(pin);
        status = HAL_GPIO_GetInterruptStatus(port, pinIdx);
    }

    return status;
}

static void GPIO_ClearInterruptFlag(ARM_GPIO_Pin_t pin)
{
    hal_gpio_port_t port = (hal_gpio_port_t)0U;
    uint32_t pinIdx = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        port = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIdx = GET_PIN_INDEX(pin);

        HAL_GPIO_ClearInterruptFlag(port, pinIdx);
    }
}

static int32_t GPIO_SetAlternateFunction(ARM_GPIO_Pin_t pin, uint32_t muxMode)
{
    int32_t result = ARM_DRIVER_OK;
    hal_gpio_port_t portIndex = (hal_gpio_port_t)0U;
    uint32_t pinIndex = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        portIndex = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIndex = GET_PIN_INDEX(pin);

        HAL_GPIO_EnableClock(portIndex);
        HAL_GPIO_SetMuxMode(portIndex, pinIndex, muxMode);
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

static void GPIO_SetAnalogFunction(ARM_GPIO_Pin_t pin)
{
    hal_gpio_port_t port = (hal_gpio_port_t)0U;
    uint32_t pinIdx = 0U;

    if (PIN_IS_AVAILABLE(pin))
    {
        port = (hal_gpio_port_t)GET_PORT_INDEX(pin);
        pinIdx = GET_PIN_INDEX(pin);

        /* Enable Clock */
        HAL_GPIO_EnableClock(port);

        /* Set Mux = 0 (Pin Disabled / Analog) */
        HAL_GPIO_SetMuxMode(port, pinIdx, 0U);
    }
}
