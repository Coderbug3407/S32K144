#include "S32K144.h"
#include "HAL_GPIO.h"


/* Lookup Table for GPIO Bases (PTA, PTB, ...) - Used for Input/Output Data */
static GPIO_Type * const s_GpioBases[] = {
    IP_PTA, IP_PTB, IP_PTC, IP_PTD, IP_PTE
};

/* Lookup Table for PORT Bases (PORTA, PORTB, ...) - Used for MUX/PCR */
static PORT_Type * const s_PortBases[] = {
    IP_PORTA, IP_PORTB, IP_PORTC, IP_PORTD, IP_PORTE
};

/* PCC indices - Use to enable clock for port */
static const uint32_t s_PccIndices[] = {
    PCC_PORTA_INDEX,
    PCC_PORTB_INDEX,
    PCC_PORTC_INDEX,
    PCC_PORTD_INDEX,
    PCC_PORTE_INDEX
};

void HAL_GPIO_EnableClock(hal_gpio_port_t port){

    IP_PCC->PCCn[s_PccIndices[port]] |= PCC_PCCn_CGC_MASK;
}

void HAL_GPIO_SetMuxMode(hal_gpio_port_t port, uint32_t pin, uint32_t muxmode){
    s_PortBases[port]->PCR[pin] &= ~PORT_PCR_MUX_MASK;
    s_PortBases[port]->PCR[pin] |= PORT_PCR_MUX(muxmode);
}

void HAL_GPIO_SetDirection(hal_gpio_port_t port,uint32_t pin,uint32_t isOut){
    if (isOut) {
        s_GpioBases[port]->PDDR |=  (1U << pin);
    } else {
        s_GpioBases[port]->PDDR &= ~ (1U << pin);
    }
}

void HAL_GPIO_WritePin(hal_gpio_port_t port, uint32_t pin, uint32_t level){
    if (level) {
       s_GpioBases[port]->PSOR = (1U <<pin);
    } else {
       s_GpioBases[port]->PCOR = (1U <<pin);
    }
}

uint32_t HAL_GPIO_ReadPin(hal_gpio_port_t port, uint32_t pin){
    return s_GpioBases[port]->PDIR >> pin & 1U;
}

void HAL_GPIO_SetPullResistor(hal_gpio_port_t port, uint32_t pin, hal_gpio_pull_t type){
    s_PortBases[port]->PCR[pin] &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);

    if (type == HAL_PULL_UP) {
        s_PortBases[port]->PCR[pin] |= (PORT_PCR_PE(1) | PORT_PCR_PS(1)); /* Enable + Pull Up */
    }
    else if (type == HAL_PULL_DOWN) {
        s_PortBases[port]->PCR[pin] |= (PORT_PCR_PE(1) | PORT_PCR_PS(0)); /* Enable + Pull Down */
    }
}

void HAL_GPIO_SetInterruptTrigger(hal_gpio_port_t port, uint32_t pin, hal_gpio_irq_trigger_t trigger){
    s_PortBases[port]->PCR[pin] &= ~PORT_PCR_IRQC_MASK;
    s_PortBases[port]->PCR[pin] |= PORT_PCR_IRQC(trigger);
}

uint32_t HAL_GPIO_GetInterruptStatus(hal_gpio_port_t port, uint32_t pin){
    return (s_PortBases[port]->ISFR >> pin) & 1U;
}

void HAL_GPIO_ClearInterruptFlag(hal_gpio_port_t port, uint32_t pin){
    s_PortBases[port]->ISFR = (1U << pin); /* Write 1 to clear (w1c) */
}




