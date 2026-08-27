#include "Driver_UART.h"
#include "HAL_UART.h"
#include <stddef.h>
#include "board.h"
#include "S32K144.h"
#include "Driver_NVIC.h"

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 1)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    .asynchronous       = 1,
    .synchronous_master = 0,
    .synchronous_slave  = 0,
    .single_wire        = 0,
    .irda               = 0,
    .smart_card         = 0,
    .smart_card_clock   = 0,
    .flow_control_rts   = 0,
    .flow_control_cts   = 0,
    .event_tx_complete  = 0,
    .event_rx_timeout   = 0,
    .reserved           = 0
};

static USART_HW_CONFIG s_HwConfig = {
    .clockSrc = CLK_SRC_FIRCDIV2,
    .srcFreq  = 48000000U,
    .osr      = 16
};

typedef struct {
    ARM_USART_SignalEvent_t cb_event;
    uart_config_t           hw_config;
    uint8_t                 initialized;
    ARM_POWER_STATE         power_state;
    uint8_t                 *rx_buf;
    uint32_t                rx_num;
    uint32_t                rx_cnt;
} USART_RESOURCES;

static USART_RESOURCES Usart0_Resources = {
    .cb_event = NULL,
    .initialized = 0,
    .power_state = ARM_POWER_OFF,
    .rx_buf = NULL,
    .rx_num = 0,
    .rx_cnt = 0
};

//
// Functions
//

void Driver_USART0_SetHWConfig(const USART_HW_CONFIG *config) {
    if (config != NULL) {
        s_HwConfig.clockSrc = config->clockSrc;
        s_HwConfig.srcFreq  = config->srcFreq;
        s_HwConfig.osr      = config->osr;
    }
}

static ARM_DRIVER_VERSION ARM_USART_GetVersion(void) {
    return DriverVersion;
}

static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void) {
    return DriverCapabilities;
}

static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event) {
    Usart0_Resources.cb_event = cb_event;
    Usart0_Resources.initialized = 1;
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Uninitialize(void) {
    Usart0_Resources.cb_event = NULL;
    Usart0_Resources.initialized = 0;
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_PowerControl(ARM_POWER_STATE state) {
    Usart0_Resources.power_state = state;
    switch (state) {
        case ARM_POWER_OFF:
            HAL_UART_DisableRxInterrupt(UART_INSTANCE_1);
            DRIVER_NVIC_DeInit(LPUART1_RxTx_IRQn);
            break;
        case ARM_POWER_LOW:
            return ARM_DRIVER_ERROR;
        case ARM_POWER_FULL:
            if (Usart0_Resources.initialized == 0) return ARM_DRIVER_ERROR;
            break;
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Send(const void *data, uint32_t num) {
    if (data == NULL || num == 0) return ARM_DRIVER_ERROR_PARAMETER;
    if (Usart0_Resources.power_state != ARM_POWER_FULL) return ARM_DRIVER_ERROR;

    const uint8_t *buf = (const uint8_t *)data;

    for (uint32_t i = 0; i < num; i++) {
        HAL_UART_SendChar(UART_INSTANCE_1, buf[i]);
    }

    if (Usart0_Resources.cb_event) {
        Usart0_Resources.cb_event(ARM_USART_EVENT_SEND_COMPLETE);
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Receive(void *data, uint32_t num) {
    if (data == NULL || num == 0) return ARM_DRIVER_ERROR_PARAMETER;
    if (Usart0_Resources.power_state != ARM_POWER_FULL) return ARM_DRIVER_ERROR;

    /* Lưu thông tin bộ đệm và reset biến đếm */
    Usart0_Resources.rx_buf = (uint8_t *)data;
    Usart0_Resources.rx_num = num;
    Usart0_Resources.rx_cnt = 0;

    /* Kích hoạt ngắt RX ở tầng HAL */
    HAL_UART_EnableRxInterrupt(UART_INSTANCE_1);

    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Control(uint32_t control, uint32_t arg) {
    if (Usart0_Resources.power_state != ARM_POWER_FULL) return ARM_DRIVER_ERROR;

    /* Load Hardware config*/
    Usart0_Resources.hw_config.clockSrc = s_HwConfig.clockSrc;
    Usart0_Resources.hw_config.srcFreq  = s_HwConfig.srcFreq;
    Usart0_Resources.hw_config.osr      = s_HwConfig.osr;
    Usart0_Resources.hw_config.sbr      = 0;

    /* Load Baudrate */
    if ((control & ARM_USART_CONTROL_Msk) == ARM_USART_MODE_ASYNCHRONOUS) {
        Usart0_Resources.hw_config.baudRate = arg;
    } else {
        return ARM_USART_ERROR_MODE;
    }

    /*Map Data Bits */
    switch (control & ARM_USART_DATA_BITS_Msk) {
        case ARM_USART_DATA_BITS_8: Usart0_Resources.hw_config.bitCount = UART_DATA_BITS_8; break;
        case ARM_USART_DATA_BITS_7: Usart0_Resources.hw_config.bitCount = UART_DATA_BITS_7; break;
        case ARM_USART_DATA_BITS_9: Usart0_Resources.hw_config.bitCount = UART_DATA_BITS_9; break;
        default: return ARM_USART_ERROR_DATA_BITS;
    }

    /*Map Parity */
    switch (control & ARM_USART_PARITY_Msk) {
        case ARM_USART_PARITY_NONE: Usart0_Resources.hw_config.parity = UART_PARITY_NONE; break;
        case ARM_USART_PARITY_EVEN: Usart0_Resources.hw_config.parity = UART_PARITY_EVEN; break;
        case ARM_USART_PARITY_ODD:  Usart0_Resources.hw_config.parity = UART_PARITY_ODD;  break;
        default: return ARM_USART_ERROR_PARITY;
    }

    /*Map Stop Bits */
    switch (control & ARM_USART_STOP_BITS_Msk) {
        case ARM_USART_STOP_BITS_1: Usart0_Resources.hw_config.stopBit = UART_STOP_BIT_1; break;
        case ARM_USART_STOP_BITS_2: Usart0_Resources.hw_config.stopBit = UART_STOP_BIT_2; break;
        default: return ARM_USART_ERROR_STOP_BITS;
    }

    /* Call HAL Init */
    HAL_UART_Init(UART_INSTANCE_1, &Usart0_Resources.hw_config);

    /* BẬT NGẮT NVIC CHO LPUART1 */
    DRIVER_NVIC_Init(LPUART1_RxTx_IRQn, 1U);

    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num) { return ARM_DRIVER_ERROR; }
static uint32_t ARM_USART_GetTxCount(void) { return 0; }
static uint32_t ARM_USART_GetRxCount(void) { return Usart0_Resources.rx_cnt; }
static ARM_USART_STATUS ARM_USART_GetStatus(void) {
    ARM_USART_STATUS status = {0};
    return status;
}
static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control) { return ARM_DRIVER_ERROR; }
static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(void) {
    ARM_USART_MODEM_STATUS status = {0};
    return status;
}

void LPUART1_RxTx_IRQHandler(void)
{

    if ((IP_LPUART1->STAT & (LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK |
                                 LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK)) != 0U)
        {
            /* Ghi bit 1 vào các cờ lỗi để xóa chúng đi (w1c - write 1 to clear) */
            IP_LPUART1->STAT |= (LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK |
                                 LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK);
        }

    /* Kiểm tra xem ngắt xảy ra có phải do cờ Receive Data Register Full (RDRF) hay không */
    if ((IP_LPUART1->STAT & LPUART_STAT_RDRF_MASK) != 0U)
    {
        /* Đọc dữ liệu ra (Đồng thời xóa luôn cờ ngắt RDRF) */
        uint16_t data = HAL_UART_ReadDataNonBlocking(UART_INSTANCE_1);

        if (Usart0_Resources.rx_buf != NULL && Usart0_Resources.rx_cnt < Usart0_Resources.rx_num)
        {
            /* Ghi byte vừa nhận vào buffer */
            Usart0_Resources.rx_buf[Usart0_Resources.rx_cnt] = (uint8_t)data;
            Usart0_Resources.rx_cnt++;

            /* Kiểm tra nếu đã nhận đủ số lượng byte mong muốn */
            if (Usart0_Resources.rx_cnt >= Usart0_Resources.rx_num)
            {
                /* Tắt ngắt để tránh bị gọi lại liên tục khi chưa có yêu cầu mới */
                HAL_UART_DisableRxInterrupt(UART_INSTANCE_1);

                /* Bắn Event sang cho main.c xử lý */
                if (Usart0_Resources.cb_event)
                {
                    Usart0_Resources.cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
                }
            }
        }
    }
}

/* Interface Control Block */
extern ARM_DRIVER_USART Driver_USART0;
ARM_DRIVER_USART Driver_USART0 = {
    ARM_USART_GetVersion,
    ARM_USART_GetCapabilities,
    ARM_USART_Initialize,
    ARM_USART_Uninitialize,
    ARM_USART_PowerControl,
    ARM_USART_Send,
    ARM_USART_Receive,
    ARM_USART_Transfer,
    ARM_USART_GetTxCount,
    ARM_USART_GetRxCount,
    ARM_USART_Control,
    ARM_USART_GetStatus,
    ARM_USART_SetModemControl,
    ARM_USART_GetModemStatus
};
