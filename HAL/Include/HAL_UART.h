#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <stdint.h>
#include "device_registers.h"
#include "board.h"



/* --- Instance Selection --- */
typedef enum {
    UART_INSTANCE_0 = 0U,
    UART_INSTANCE_1 = 1U,
    UART_INSTANCE_2 = 2U
} uart_instance_t;

/* --- Frame Settings Definitions --- */

/* Data Bits */
typedef enum {
    UART_DATA_BITS_8  = 0U,
    UART_DATA_BITS_7  = 1U,
    UART_DATA_BITS_9  = 2U,
    UART_DATA_BITS_10 = 3U
} uart_bit_count_t;

/* Parity */
typedef enum {
    UART_PARITY_NONE = 0U,
    UART_PARITY_EVEN = 1U,
    UART_PARITY_ODD  = 2U
} uart_parity_t;

/* Stop Bit */
typedef enum {
    UART_STOP_BIT_1 = 0U,
    UART_STOP_BIT_2 = 1U
} uart_stop_bit_t;

/* --- Main Configuration Struct --- */
typedef struct {
    /* Clock */
    system_clock_src_t clockSrc;
    uint32_t srcFreq;
    /* Baudrate & Over Sampling */
    uint32_t baudRate;
    uint16_t sbr;
    uint8_t  osr;

    /* Frame Format */
    uart_bit_count_t bitCount;
    uart_parity_t    parity;
    uart_stop_bit_t  stopBit;

} uart_config_t;

/** 
 * @brief  Initializes the UART peripheral with the given configuration.
 *
 * @param  instance  UART instance to initialize.
 * @param  config    Pointer to uart_config_t structure containing configuration settings.
 *
 */
void HAL_UART_Init(uart_instance_t instance, const uart_config_t *config);


/**
  * @brief  Sends a single character over UART.
  * @param  instance  UART instance to use for sending.
  * @param  data      Character to send.
 */
void HAL_UART_SendChar(uart_instance_t instance, uint16_t data);

/**
  * @brief  Receives a single character from UART.
  * @param  instance  UART instance to use for receiving.
  * @return Received character.
 */
uint16_t HAL_UART_ReceiveChar(uart_instance_t instance);

/**
  * @brief  Sends a null-terminated string over UART.
  * @param  instance  UART instance to use for sending.
  * @param  str       Pointer to the null-terminated string to send.
 */
void HAL_UART_SendString(uart_instance_t instance, const char *str);

/**
 * @brief Enables the RX Interrupt for the specified UART instance.
 * @param instance UART instance
 */
void HAL_UART_EnableRxInterrupt(uart_instance_t instance);

/**
 * @brief Disables the RX Interrupt for the specified UART instance.
 * @param instance UART instance
 */
void HAL_UART_DisableRxInterrupt(uart_instance_t instance);

/**
 * @brief Receives a single character from UART without blocking (No while loop).
 * @param instance UART instance
 * @return Received character.
 */
uint16_t HAL_UART_ReadDataNonBlocking(uart_instance_t instance);

#endif /* HAL_UART_H_ */
