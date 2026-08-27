#include "HAL_UART.h"

static LPUART_Type * const LPUART_Bases[] = {
    IP_LPUART0,
    IP_LPUART1,
    IP_LPUART2
};

static const uint8_t LPUART_PCC_Indices[] = {
    PCC_LPUART0_INDEX,
    PCC_LPUART1_INDEX,
    PCC_LPUART2_INDEX
};

static void UART_Clock(system_clock_src_t src)
{
    switch(src)
    {
        case CLK_SRC_SIRCDIV2:
            if ((IP_SCG->SIRCDIV & SCG_SIRCDIV_SIRCDIV2_MASK) == 0)
                IP_SCG->SIRCDIV |= SCG_SIRCDIV_SIRCDIV2(1);
            break;
        case CLK_SRC_FIRCDIV2:
            if ((IP_SCG->FIRCDIV & SCG_FIRCDIV_FIRCDIV2_MASK) == 0)
                IP_SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);
            break;
        case CLK_SRC_SOSCDIV2:
            if ((IP_SCG->SOSCDIV & SCG_SOSCDIV_SOSCDIV2_MASK) == 0)
                IP_SCG->SOSCDIV |= SCG_SOSCDIV_SOSCDIV2(1);
            break;
        case CLK_SRC_SPLLDIV2:
            if ((IP_SCG->SPLLDIV & SCG_SPLLDIV_SPLLDIV2_MASK) == 0)
                IP_SCG->SPLLDIV |= SCG_SPLLDIV_SPLLDIV2(1);
            break;
        default: break;
    }
}

static void HAL_UART_EnableClock(uart_instance_t instance, system_clock_src_t src)
{
    uint8_t pccIndex = LPUART_PCC_Indices[instance];
    UART_Clock(src);

    /* Disable Clock -> Change Source -> Enable Clock */
    IP_PCC->PCCn[pccIndex] &= ~PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[pccIndex] |= PCC_PCCn_PCS(src) | PCC_PCCn_CGC_MASK;
}

void HAL_UART_Init(uart_instance_t instance, const uart_config_t *config)
{
    LPUART_Type *base = LPUART_Bases[instance];
    uint16_t sbr_val;
    uint8_t  osr_actual;
    uint32_t tempBaud;
    uint32_t tempCtrl;

    /*Enable Clock*/
    HAL_UART_EnableClock(instance, config->clockSrc);

    /*Disable TX/RX to config */
    base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

    /* OSR (Over Sampling Ratio) */
    osr_actual = (config->osr < 4) ? 16 : config->osr;

    /* Calculate SBR (Baud Rate Modulo Divisor) */
    /* Baudrate = Clock / (SBR * OSR) -> SBR = Clock / (Baudrate * OSR) */
    if (config->sbr > 0) {
        sbr_val = config->sbr;
    } else {
        if (config->baudRate > 0) {
            sbr_val = (uint16_t)(config->srcFreq / (config->baudRate * osr_actual));
            if (sbr_val == 0) sbr_val = 1; /* SBR must >= 1 */
        } else {
            sbr_val = 4;
        }
    }

    /* Config BAUD */
    tempBaud = base->BAUD;
    tempBaud &= ~(LPUART_BAUD_OSR_MASK | LPUART_BAUD_SBR_MASK | LPUART_BAUD_SBNS_MASK | LPUART_BAUD_M10_MASK);

    /* Write OSR */
    tempBaud |= LPUART_BAUD_OSR(osr_actual - 1);
    tempBaud |= LPUART_BAUD_SBR(sbr_val);

    /* Config Stop Bit */
    if (config->stopBit == UART_STOP_BIT_2) {
        tempBaud |= LPUART_BAUD_SBNS_MASK;
    }

    /* Config 10-bit mode */
    if (config->bitCount == UART_DATA_BITS_10) {
        tempBaud |= LPUART_BAUD_M10_MASK;
    }

    base->BAUD = tempBaud;

    /*Config CTRL */
    tempCtrl = base->CTRL;
    tempCtrl &= ~(LPUART_CTRL_PE_MASK | LPUART_CTRL_PT_MASK | LPUART_CTRL_M_MASK | LPUART_CTRL_M7_MASK);

    /* Data Bits */
    switch (config->bitCount) {
        case UART_DATA_BITS_7:
            tempCtrl |= LPUART_CTRL_M7_MASK;
            break;
        case UART_DATA_BITS_9:
            tempCtrl |= LPUART_CTRL_M_MASK;
            break;
        case UART_DATA_BITS_10:
            tempCtrl |= LPUART_CTRL_M_MASK;
            break;
        default:
            /* 8-bit mode: M=0, M7=0 */
            break;
    }

    /* Parity */
    if (config->parity != UART_PARITY_NONE) {
        tempCtrl |= LPUART_CTRL_PE_MASK;
        if (config->parity == UART_PARITY_ODD) {
            tempCtrl |= LPUART_CTRL_PT_MASK;
        }
    }

    base->CTRL = tempCtrl;

    /* Enable TX and RX */
    base->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
}

void HAL_UART_SendChar(uart_instance_t instance, uint16_t data)
{
    LPUART_Type *base = LPUART_Bases[instance];
    /*Transmit Data Register Empty*/
    while ((base->STAT & LPUART_STAT_TDRE_MASK) == 0);
    base->DATA = (uint32_t)data;
}

uint16_t HAL_UART_ReceiveChar(uart_instance_t instance)
{
    LPUART_Type *base = LPUART_Bases[instance];
    /*Receive Data Register Full*/
    while ((base->STAT & LPUART_STAT_RDRF_MASK) == 0);
    return (uint16_t)(base->DATA & 0x3FF);
}

void HAL_UART_SendString(uart_instance_t instance, const char *str)
{
    while (*str != '\0') {
        HAL_UART_SendChar(instance, (uint8_t)(*str));
        str++;
    }
}


void HAL_UART_EnableRxInterrupt(uart_instance_t instance)
{
    LPUART_Type *base = LPUART_Bases[instance];
    /* Bật bit Receive Interrupt Enable (RIE) */
    base->CTRL |= LPUART_CTRL_RIE_MASK;
}

void HAL_UART_DisableRxInterrupt(uart_instance_t instance)
{
    LPUART_Type *base = LPUART_Bases[instance];
    /* Tắt bit Receive Interrupt Enable (RIE) */
    base->CTRL &= ~LPUART_CTRL_RIE_MASK;
}

uint16_t HAL_UART_ReadDataNonBlocking(uart_instance_t instance)
{
    LPUART_Type *base = LPUART_Bases[instance];
    /* Đọc thẳng thanh ghi DATA mà không cần vòng lặp while chờ */
    return (uint16_t)(base->DATA & 0x3FF);
}
