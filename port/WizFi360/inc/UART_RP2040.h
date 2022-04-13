/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef _UART_RP2040_H_
#define _UART_RP2040_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "RTE_Components.h"
#include "Driver_USART.h"
#include "RP2040_Device.h"
#include CMSIS_device_header


// USART flags
#define UART_FLAG_INITIALIZED      ((uint8_t)(1U))
#define UART_FLAG_POWERED          ((uint8_t)(1U << 1))
#define UART_FLAG_CONFIGURED       ((uint8_t)(1U << 2))
#define UART_FLAG_TX_ENABLED       ((uint8_t)(1U << 3))
#define UART_FLAG_RX_ENABLED       ((uint8_t)(1U << 4))


// DMA Use
#define UART_DMA_USE_TX           (1U << 0)
#define UART_DMA_USE_RX           (1U << 1)
#define UART_DMA_USE_TX_RX        (UART_DMA_USE_TX | UART_DMA_USE_RX)

// UART0 Configuration
#if USE_UART0
// Peripheral: UART0
#define UART_PERIPHERAL          uart0
#define UART0_MODE_ASYNC
#define UART_IRQ_NUM             UART0_IRQ
#elif USE_UART1
// Peripheral: UART1
#define UART_PERIPHERAL          uart1
#define UART1_MODE_ASYNC
#define UART_IRQ_NUM             UART1_IRQ
#endif

// UART0 used in Asynchronous mode
#define UART_MODE_ASYNC         1

// UART DMA USE
#ifdef UART_TX_DMA_Instance
  #define UART_DMA_USE_TX        UART_DMA_USE_TX
#else
  #define UART_DMA_USE_TX        0
#endif

#ifdef UART_RX_DMA_Instance
  #define UART_DMA_USE_RX        UART_DMA_USE_RX
#else
  #define UART_DMA_USE_RX        0
#endif
#define UART_DMA_USE            (UART_DMA_USE_TX | UART_DMA_USE_RX)

// UART Transfer Information (Run-Time)
typedef struct _UART_TRANSFER_INFO {
        uint32_t                rx_num;            // Total number of receive data
        uint32_t                tx_num;            // Total number of transmit data
        uint8_t                *rx_buf;            // Pointer to in data buffer
  const uint8_t                *tx_buf;            // Pointer to out data buffer
        uint32_t                rx_cnt;            // Number of data received
        uint32_t                tx_cnt;            // Number of data sent
        uint8_t                 break_flag;        // Transmit break flag
        uint8_t                 send_active;       // Send active flag
  const uint8_t                 padding[2];

} UART_TRANSFER_INFO;

typedef struct _UART_STATUS {
  uint8_t tx_busy;                      // Transmitter busy flag
  uint8_t rx_busy;                      // Receiver busy flag
  uint8_t tx_underflow;                 // Transmit data underflow detected (cleared on start of next send operation)
  uint8_t rx_overflow;                  // Receive data overflow detected (cleared on start of next receive operation)
  uint8_t rx_break;                     // Break detected on receive (cleared on start of next receive operation)
  uint8_t rx_framing_error;             // Framing error detected on receive (cleared on start of next receive operation)
  uint8_t rx_parity_error;              // Parity error detected on receive (cleared on start of next receive operation)
} UART_STATUS;

// UART Information (Run-time)
typedef struct _UART_INFO {
  ARM_USART_SignalEvent_t cb_event;     // Event Callback
  UART_STATUS             status;       // Status flags
  uint8_t                 flags;        // Current USART flags
  uint32_t                flow_control;      // Flow control
} UART_INFO;

// UART Resources definition
typedef const struct {
  ARM_USART_CAPABILITIES  capabilities;      // Capabilities
  uart_inst_t            *reg;          // UART instance
  uart_hw_t              *h;            // UART peripheral pointer
  UART_INFO              *info;         // Run-Time Information
  UART_TRANSFER_INFO     *xfer;         // UART transfer information
  IRQn_Type               irq_num;      // UART IRQ Number
  uint8_t                 dma_use_tx;
  uint8_t                 dma_use_rx;
  uint16_t                reserved;
} UART_RESOURCES;


// Global functions and variables exported by driver .c module
#ifdef UART0_MODE_ASYNC
extern ARM_DRIVER_USART Driver_UART0;
#endif

#ifdef UART1_MODE_ASYNC
extern ARM_DRIVER_USART Driver_UART1;
#endif

#endif /* _UART_RP2040_H_ */
