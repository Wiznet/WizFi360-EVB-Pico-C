/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 
#include "UART_RP2040.h"
#include "RP2040_Device.h"

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)
#define UNUSED(x) (void)(x)           /* macro to get rid of 'unused parameter' warning */

void on_uart_isr(void);

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

static const ARM_USART_CAPABILITIES Capabilities = {
    1,      ///< supports UART (Asynchronous) mode 
    0,      ///< supports Synchronous Master mode
    0,      ///< supports Synchronous Slave mode
    0,      ///< supports UART Single-wire mode
    0,      ///< supports UART IrDA mode
    0,      ///< supports UART Smart Card mode
    0,      ///< Smart Card Clock generator available
    0,      ///< RTS Flow Control available
    0,      ///< CTS Flow Control available
    0,      ///< Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,      ///< Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,      ///< RTS Line: 0=not available, 1=available
    0,      ///< CTS Line: 0=not available, 1=available
    0,      ///< DTR Line: 0=not available, 1=available
    0,      ///< DSR Line: 0=not available, 1=available
    0,      ///< DCD Line: 0=not available, 1=available
    0,      ///< RI Line: 0=not available, 1=available
    0,      ///< Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,      ///< Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,      ///< Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0       ///< Signal RI change event: \ref ARM_USART_EVENT_RI
#if (defined(ARM_USART_API_VERSION) && (ARM_USART_API_VERSION >= 0x203U))
  , 0U  // Reserved bits
#endif
  };

// UART Run-Time Information
static UART_INFO UART_Info = {
  .cb_event = 0U,
  .status   = {
    .tx_busy          = 0U,
    .rx_busy          = 0U,
    .tx_underflow     = 0U,
    .rx_overflow      = 0U,
    .rx_break         = 0U,
    .rx_framing_error = 0U,
    .rx_parity_error  = 0U
  },
  .flags        = 0U,
  .flow_control = 0U
};

static UART_TRANSFER_INFO UART_TransferInfo = {
  .rx_num      = 0U,
  .tx_num      = 0U,
  .rx_buf      = 0U,
  .tx_buf      = 0U,
  .rx_cnt      = 0U,
  .tx_cnt      = 0U,
  .break_flag  = 0U,
  .send_active = 0U,
  .padding     = {0U, 0U}
};


// UART0
#if USE_UART0
UART_RESOURCES UART0_Resources = {
  .capabilities = Capabilities,
  .reg = uart0,
  .h = (uart_hw_t *)uart0,
  .info = &UART_Info,
  .xfer = &UART_TransferInfo,
  .irq_num = UART0_IRQ

};
#endif

// UART1
#if USE_UART1
UART_RESOURCES UART1_Resources = {
  .capabilities = Capabilities,
  .reg = uart1,
  .h = (uart_hw_t *)uart1,
  .info = &UART_Info,
  .xfer = &UART_TransferInfo,
  .irq_num = UART1_IRQ, 
};

#endif

/**
  \fn          const UART_RESOURCES UART_Resources (UART_HandleTypeDef *huart)
  \brief       Get UART_RESOURCES strusture from UART_HandleTypeDef
*/
static const UART_RESOURCES * UART_Resources (void) {
  const UART_RESOURCES *uart = NULL;

#if USE_UART0
  uart = &UART0_Resources;
#elif USE_UART1
  uart = &UART1_Resources;
#endif
  return uart;
}


// USART Driver functions

/**
  \fn          ARM_DRIVER_VERSION UART_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION UART_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES UART_GetCapabilities (void)
  \brief       Get driver capabilities
  \param[in]   usart     Pointer to USART resources
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES UART_GetCapabilities (const UART_RESOURCES *uart) {
  return uart->capabilities;
}

/**
  \fn          int32_t UART_Initialize (      ARM_USART_SignalEvent_t  cb_event
                                         const UART_RESOURCES          *uart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   uart      Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Initialize (const UART_RESOURCES         *uart,
                                          ARM_USART_SignalEvent_t  cb_event) {

  if (uart->info->flags & UART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  // Clear Status flags
  uart->info->status.tx_busy          = 0U;
  uart->info->status.rx_busy          = 0U;
  uart->info->status.tx_underflow     = 0U;
  uart->info->status.rx_overflow      = 0U;
  uart->info->status.rx_break         = 0U;
  uart->info->status.rx_framing_error = 0U;
  uart->info->status.rx_parity_error  = 0U;

  // Initialize UART Run-time Resources
  uart->info->cb_event = cb_event;

  // Clear transfer information
  memset(uart->xfer, 0, sizeof(UART_TRANSFER_INFO));

  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  uart->info->flags = UART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Uninitialize (const UART_RESOURCES *uart)
  \brief       De-initialize UART Interface.
  \param[in]   usart     Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Uninitialize (const UART_RESOURCES *uart) {

  // Reset UART status flags
  uart->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_PowerControl (ARM_POWER_STATE state)
  \brief       Control UART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_PowerControl (const UART_RESOURCES *uart,
                                             ARM_POWER_STATE state) {


  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:

      // UART peripheral reset
      NVIC_DisableIRQ(uart->irq_num);
      NVIC_ClearPendingIRQ(uart->irq_num);
      //UART_PeripheralReset (uart->reg);
      uart_deinit (uart->reg);
      uart->info->flags &= ~UART_FLAG_POWERED;

      // Clear Status flags
      uart->info->status.tx_busy          = 0U;
      uart->info->status.rx_busy          = 0U;
      uart->info->status.tx_underflow     = 0U;
      uart->info->status.rx_overflow      = 0U;
      uart->info->status.rx_break         = 0U;
      uart->info->status.rx_framing_error = 0U;
      uart->info->status.rx_parity_error  = 0U;

      uart->xfer->send_active             = 0U;

      // Clear powered flag
      uart->info->flags &= ~UART_FLAG_POWERED;
      break;
    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      // Clear Status flags
      uart->info->status.tx_busy          = 0U;
      uart->info->status.rx_busy          = 0U;
      uart->info->status.tx_underflow     = 0U;
      uart->info->status.rx_overflow      = 0U;
      uart->info->status.rx_break         = 0U;
      uart->info->status.rx_framing_error = 0U;
      uart->info->status.rx_parity_error  = 0U;

      uart->xfer->send_active             = 0U;
      uart->xfer->break_flag              = 0U;

      uart->info->flow_control            = 0U;

      // Set flag initialized
      uart->info->flags = UART_FLAG_POWERED | UART_FLAG_INITIALIZED;

      uart_init (uart->reg, UART_DEFAULT_BAUDRATE);

      // Clear and enable USART IRQ
      NVIC_ClearPendingIRQ(uart->irq_num);
      NVIC_EnableIRQ(uart->irq_num);

      // And set up and enable the interrupt handlers
      irq_set_exclusive_handler(uart->irq_num, on_uart_isr);
      irq_set_enabled(uart->irq_num, true);

      // Now enable the UART to send interrupts - RX only
      uart_set_irq_enables(uart->reg, true, false);


    break;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Send (const void            *data,
                                        uint32_t         num,
                                  const UART_RESOURCES *uart)
  \brief       Start sending data to UART transmitter.
  \param[in]   data  Pointer to buffer with data to send to UART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Send (const UART_RESOURCES *uart,
                             const void           *data,
                                   uint32_t        num) {

  uint32_t event = 0;
  
  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((uart->info->flags & UART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if (uart->xfer->send_active != 0U) {
    // Send is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set Send Active flag
  uart->xfer->send_active = 1U;

  // Save transmit buffer info
  uart->xfer->tx_cnt = 0U;
  uart->xfer->tx_num = num;
  uart->xfer->tx_buf = (const uint8_t *)data;

  uart_write_blocking(uart->reg, (const uint8_t *)uart->xfer->tx_buf, uart->xfer->tx_num);
  uart->xfer->tx_buf = NULL;
  uart->xfer->tx_cnt = uart->xfer->tx_num;
  uart->xfer->send_active = 0U;

  event |= ARM_USART_EVENT_SEND_COMPLETE;
  if (uart->info->cb_event != NULL)
    uart->info->cb_event (event);

  return ARM_DRIVER_OK;

}

/**
  \fn          int32_t UART_Receive (      void            *data,
                                           uint32_t         num,
                                     const UART_RESOURCES  *uart)
  \brief       Start receiving data from UART receiver.
  \param[out]  data  Pointer to buffer for data to receive from UART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Receive (const UART_RESOURCES *uart,
                                       void           *data,
                                       uint32_t        num) {
                             
  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((uart->info->flags & UART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if (uart->info->status.rx_busy == 1U) {
    // Receive operation is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set receive busy flag
  uart->info->status.rx_busy = 1U;

  // Clear status flags
  uart->info->status.rx_break          = 0U;
  uart->info->status.rx_framing_error  = 0U;
  uart->info->status.rx_overflow       = 0U;
  uart->info->status.rx_parity_error   = 0U;

  // Set receive info
  uart->xfer->rx_cnt =  0U;
  uart->xfer->rx_num = num;
  uart->xfer->rx_buf = (uint8_t *)data;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t UART_Transfer (const void             *data_out,
                                             void            *data_in,
                                             uint32_t         num,
                                       const UART_RESOURCES  *uart)
  \brief       Start sending/receiving data to/from UART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to UART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from UART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   uart      Pointer to UART resources
  \return      \ref execution_status
*/
static int32_t UART_Transfer (const UART_RESOURCES *uart,
                                  const void           *data_out,
                                        void           *data_in,
                                        uint32_t        num) {


  UNUSED(data_out);
  UNUSED(data_in);
  UNUSED(num);
  UNUSED(uart);

  // Supported only in Synchronous mode
  return ARM_DRIVER_ERROR;
}

/**
  \fn          uint32_t UART_GetTxCount (const UART_RESOURCES *uart)
  \brief       Get transmitted data count.
  \param[in]   uart     Pointer to UART resources
  \return      number of data items transmitted
*/
static uint32_t UART_GetTxCount (const UART_RESOURCES *uart) {
  return uart->xfer->tx_cnt;
}

/**
  \fn          uint32_t UART_GetRxCount (const UART_RESOURCES *uart)
  \brief       Get received data count.
  \param[in]   uart     Pointer to UART resources
  \return      number of data items received
*/
static uint32_t UART_GetRxCount (const UART_RESOURCES *uart) {
  return uart->xfer->rx_cnt;
}

/**
  \fn          int32_t UART_Control (      uint32_t          control,
                                           uint32_t          arg,
                                      const UART_RESOURCES  *uart)
  \brief       Control UART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   uart    Pointer to UART resources
  \return      common \ref execution_status and driver specific \ref uart_execution_status
*/
static int32_t UART_Control (const UART_RESOURCES *uart,
                                       uint32_t        control,
                                       uint32_t        arg) {
  uint32_t mode;
  uint32_t uart_data_bits, uart_parity, uart_stopbits;
  uint8_t uart_rts, uart_cts;

  uart_data_bits = 0;
  uart_parity = 0;
  uart_stopbits = 0;
  uart_rts = 0;
  uart_cts = 0;

  if (control & (1U << 4)) {
    // USART Miscellaneous Operations
    switch (control & ARM_USART_CONTROL_Msk) {
      case ARM_USART_SET_DEFAULT_TX_VALUE:       // Set default Transmit value (Synchronous Receive only); arg = value
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_SET_IRDA_PULSE:             // Set IrDA Pulse in ns; arg: 0=3/16 of bit period
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_SET_SMART_CARD_GUARD_TIME:  // Set Smart Card Guard Time; arg = number of bit periods
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_SET_SMART_CARD_CLOCK:       // Set Smart Card Clock in Hz; arg: 0=Clock not generated
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_CONTROL_SMART_CARD_NACK:    // Smart Card NACK generation; arg: 0=disabled, 1=enabled
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_CONTROL_TX:                 // Transmitter; arg: 0=disabled, 1=enabled
        if (arg == 0U) {
          // Disable UART transmitter
          hw_clear_bits(&uart->h->cr, UART_UARTCR_TXE_BITS);
          uart->info->flags &= ~UART_FLAG_TX_ENABLED;
        }
        else {
          hw_set_bits(&uart->h->cr, UART_UARTCR_TXE_BITS);
          uart->info->flags |= UART_FLAG_TX_ENABLED;
        }
        return ARM_DRIVER_OK;

      case ARM_USART_CONTROL_RX:                 // Receiver; arg: 0=disabled, 1=enabled
        // Check if RX line available
        if (arg == 0U) {
          // Disable USART receive and receive interrupt
          uart_set_irq_enables(uart->reg, false, false);

          // Disable USART receiver
          hw_clear_bits(&uart->h->cr, UART_UARTCR_RXE_BITS);
          uart->info->flags &= ~UART_FLAG_RX_ENABLED;
        }
        else {
          // Enable USART receiver
          hw_set_bits(&uart->h->cr, UART_UARTCR_RXE_BITS);

          // Enable USART receive and receive timeout interrupt
          uart_set_irq_enables(uart->reg, true, false);
          uart->info->flags |= UART_FLAG_RX_ENABLED;
        }
        return ARM_DRIVER_OK;

      case ARM_USART_CONTROL_BREAK:              // Continuous Break transmission; arg: 0=disabled, 1=enabled
        return ARM_DRIVER_ERROR_UNSUPPORTED;
      case ARM_USART_ABORT_SEND:                 // Abort ARM_USART_Send
        // Clear transfer info
        uart->xfer->tx_buf = NULL;
        uart->xfer->tx_num = 0U;
        uart->xfer->tx_cnt = 0U;

        // Clear Send Active flag
        uart->xfer->send_active = 0U;

        return ARM_DRIVER_OK;

      case ARM_USART_ABORT_RECEIVE:              // Abort ARM_USART_Receive
        // Clear transfer info
        uart->xfer->rx_buf = NULL;
        uart->xfer->rx_num = 0;
        uart->xfer->rx_cnt = 0;

        // Clear RX Busy status
        uart->info->status.rx_busy = 0U;

        return ARM_DRIVER_OK;

      case ARM_USART_ABORT_TRANSFER:             // Abort ARM_USART_Transfer
        // Transfer not implemented
        return ARM_DRIVER_ERROR;

      default: 
        return ARM_DRIVER_ERROR;
    }
  } else {
    // USART Mode

    if ((uart->info->status.rx_busy != 0U) || (uart->xfer->send_active != 0U)) {
      // Receive or send operation in progress
      return ARM_DRIVER_ERROR_BUSY;
    }

    switch (control & ARM_USART_CONTROL_Msk) {
      case ARM_USART_MODE_ASYNCHRONOUS:          // UART (Asynchronous); arg = Baudrate
        //mode = ARM_USART_MODE_ASYNCHRONOUS;    
        uart_init(uart->reg, UART_DEFAULT_BAUDRATE);
        uart_set_fifo_enabled(uart->reg, true);
        break;

      case ARM_USART_MODE_SYNCHRONOUS_MASTER:    // Synchronous Master (generates clock signal); arg = Baudrate
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_MODE_SYNCHRONOUS_SLAVE:     // Synchronous Slave (external clock signal)
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_MODE_SINGLE_WIRE:           // UART Single-wire (half-duplex); arg = Baudrate
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_MODE_IRDA:                  // UART IrDA; arg = Baudrate
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      case ARM_USART_MODE_SMART_CARD:            // UART Smart Card; arg = Baudrate
        return ARM_DRIVER_ERROR_UNSUPPORTED;

      default:
        return ARM_DRIVER_ERROR;
    }

    // Data Bits
    switch (control & ARM_USART_DATA_BITS_Msk) {
      case ARM_USART_DATA_BITS_5:
        break;
      case ARM_USART_DATA_BITS_6:
          uart_data_bits = 6;
          hw_write_masked(&uart->h->lcr_h,
                         ((uart_data_bits - 5u) << UART_UARTLCR_H_WLEN_LSB),
                         UART_UARTLCR_H_WLEN_BITS);
        break;
      case ARM_USART_DATA_BITS_7:
          uart_data_bits = 7;
          hw_write_masked(&uart->h->lcr_h,
                         ((uart_data_bits - 5u) << UART_UARTLCR_H_WLEN_LSB),
                         UART_UARTLCR_H_WLEN_BITS);

        break;
      case ARM_USART_DATA_BITS_8:
          uart_data_bits = 8;
          hw_write_masked(&uart->h->lcr_h,
                         ((uart_data_bits - 5u) << UART_UARTLCR_H_WLEN_LSB),
                         UART_UARTLCR_H_WLEN_BITS);

        break;
      default:  return ARM_USART_ERROR_DATA_BITS;
    }

    // Parity Bit
    switch (control & ARM_USART_PARITY_Msk) {
      case ARM_USART_PARITY_NONE:
        uart_parity = UART_PARITY_NONE;
        hw_write_masked(&uart->h->lcr_h,
                       (bool_to_bit(uart_parity != UART_PARITY_NONE) << UART_UARTLCR_H_PEN_LSB) |
                       (bool_to_bit(uart_parity == UART_PARITY_EVEN) << UART_UARTLCR_H_EPS_LSB),
                        UART_UARTLCR_H_PEN_BITS |
                        UART_UARTLCR_H_EPS_BITS);
        break;
      case ARM_USART_PARITY_ODD:
        uart_parity = UART_PARITY_ODD;
        hw_write_masked(&uart->h->lcr_h,
               (bool_to_bit(uart_parity != UART_PARITY_NONE) << UART_UARTLCR_H_PEN_LSB) |
               (bool_to_bit(uart_parity == UART_PARITY_EVEN) << UART_UARTLCR_H_EPS_LSB),
                UART_UARTLCR_H_PEN_BITS |
                UART_UARTLCR_H_EPS_BITS);
        break;
      case ARM_USART_PARITY_EVEN:
        uart_parity = UART_PARITY_EVEN;
        hw_write_masked(&uart->h->lcr_h,
                       (bool_to_bit(uart_parity != UART_PARITY_NONE) << UART_UARTLCR_H_PEN_LSB) |
                       (bool_to_bit(uart_parity == UART_PARITY_EVEN) << UART_UARTLCR_H_EPS_LSB),
                        UART_UARTLCR_H_PEN_BITS |
                        UART_UARTLCR_H_EPS_BITS);

        break;
      default: return ARM_USART_ERROR_PARITY;
    }

    // Stop Bit
    switch (control & ARM_USART_STOP_BITS_Msk) {
      case ARM_USART_STOP_BITS_1:
        uart_stopbits = 1;
        hw_write_masked(&uart->h->lcr_h,
                       ((uart_stopbits - 1u) << UART_UARTLCR_H_STP2_LSB),
                       UART_UARTLCR_H_STP2_BITS);
        break;
      case ARM_USART_STOP_BITS_2:
        uart_stopbits = 2;
        hw_write_masked(&uart->h->lcr_h,
                       ((uart_stopbits - 1u) << UART_UARTLCR_H_STP2_LSB),
                       UART_UARTLCR_H_STP2_BITS);
        break;
      default: return ARM_USART_ERROR_STOP_BITS;
    }

    // Flow Control
    switch (control & ARM_USART_FLOW_CONTROL_Msk) {
      case ARM_USART_FLOW_CONTROL_NONE:
        gpio_set_function(UART_RTS_PIN, GPIO_FUNC_NULL);
        gpio_set_function(UART_CTS_PIN, GPIO_FUNC_NULL);
        uart_rts = 0;
        uart_cts = 0;
        hw_write_masked(&uart->h->cr,
                       (bool_to_bit(uart_cts) << UART_UARTCR_CTSEN_LSB) | (bool_to_bit(uart_rts) << UART_UARTCR_RTSEN_LSB),
                       UART_UARTCR_RTSEN_BITS | UART_UARTCR_CTSEN_BITS);
        break;

      case ARM_USART_FLOW_CONTROL_RTS:        
#if (UART_RTS_PIN)
        gpio_set_function(UART_RTS_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_CTS_PIN, GPIO_FUNC_NULL);
        uart_rts = 1;
        uart_cts = 0;
        hw_write_masked(&uart->h->cr,
                       (bool_to_bit(uart_cts) << UART_UARTCR_CTSEN_LSB) | (bool_to_bit(uart_rts) << UART_UARTCR_RTSEN_LSB),
                       UART_UARTCR_RTSEN_BITS | UART_UARTCR_CTSEN_BITS);
        break;
#else
        return ARM_USART_ERROR_FLOW_CONTROL;
#endif

      case ARM_USART_FLOW_CONTROL_CTS:
#if (UART_CTS_PIN)
        gpio_set_function(UART_RTS_PIN, GPIO_FUNC_NULL);
        gpio_set_function(UART_CTS_PIN, GPIO_FUNC_UART);
        uart_rts = 0;
        uart_cts = 1;
        hw_write_masked(&uart->h->cr,
                       (bool_to_bit(uart_cts) << UART_UARTCR_CTSEN_LSB) | (bool_to_bit(uart_rts) << UART_UARTCR_RTSEN_LSB),
                       UART_UARTCR_RTSEN_BITS | UART_UARTCR_CTSEN_BITS);

#else
        return ARM_USART_ERROR_FLOW_CONTROL;
#endif
      case ARM_USART_FLOW_CONTROL_RTS_CTS:
#if (UART_RTS_PIN && UART_CTS_PIN)
        gpio_set_function(UART_RTS_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_CTS_PIN, GPIO_FUNC_UART);
        uart_rts = 1;
        uart_cts = 1;
        hw_write_masked(&uart->h->cr,
                       (bool_to_bit(uart_cts) << UART_UARTCR_CTSEN_LSB) | (bool_to_bit(uart_rts) << UART_UARTCR_RTSEN_LSB),
                       UART_UARTCR_RTSEN_BITS | UART_UARTCR_CTSEN_BITS);

        break;
#else
        return ARM_USART_ERROR_FLOW_CONTROL;
#endif

      default: return ARM_USART_ERROR_FLOW_CONTROL;
    }
    // Configuration is valid, apply setti
    // Setup baud rate dividers
    uart_set_baudrate(uart->reg, arg);

    // Set configured flag
    uart->info->flags |= UART_FLAG_CONFIGURED;
  }
  
  return ARM_DRIVER_OK;
}


/**
  \fn          ARM_USART_STATUS UART_GetStatus (const UART_RESOURCES *uart)
  \brief       Get UART status.
  \param[in]   uart     Pointer to UART resources
  \return      UART status \ref ARM_UART_STATUS
*/
static ARM_USART_STATUS UART_GetStatus (const UART_RESOURCES *uart) {
  ARM_USART_STATUS uart_status;

  if (uart->xfer->send_active != 0U) {
    uart_status.tx_busy        = 1U;
  } else {
    //status.tx_busy        = ((usart->reg->FR & UART_FR_BUSY_Msk) ? (1U) : (0U));
  }

  uart_status.rx_busy          = uart->info->status.rx_busy;
  uart_status.tx_underflow     = uart->info->status.tx_underflow;
  uart_status.rx_overflow      = uart->info->status.rx_overflow;
  uart_status.rx_break         = uart->info->status.rx_break;
  uart_status.rx_framing_error = uart->info->status.rx_framing_error;
  uart_status.rx_parity_error  = uart->info->status.rx_parity_error;

  return uart_status;

}

/**
  \fn          int32_t UART_SetModemControl (const UART_RESOURCES         *uart,
                                                    ARM_USART_MODEM_CONTROL  control)
  \brief       Set UART Modem Control line state.
  \param[in]   usart     Pointer to UART resources
  \param[in]   control   \ref ARM_UART_MODEM_CONTROL
  \return      \ref execution_status
*/
static int32_t UART_SetModemControl (const UART_RESOURCES                *uart,
                                                 ARM_USART_MODEM_CONTROL control) {

  (void) control;

  // No modem control in synchronous mode
  return ARM_DRIVER_ERROR;

}


/**
  \fn          ARM_UART_MODEM_STATUS UART_GetModemStatus (void)
  \brief       Get UART Modem Status lines state.
  \return      modem status \ref ARM_UART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS UART_GetModemStatus (const UART_RESOURCES *uart) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  return modem_status;
}

void on_uart_isr(void){
  UART_RESOURCES * uart = UART_Resources();
  uint32_t event = 0U;

  // Check for errors
  if ((uart->h->ris & UART_UARTRIS_OERIS_BITS) != 0U) {
    // Overrun error
    uart->info->status.rx_overflow = 1U;
    event |= ARM_USART_EVENT_RX_OVERFLOW;
    hw_set_bits(&uart->h->icr, UART_UARTICR_OEIC_BITS);
  }
  
  if((uart->h->ris & UART_UARTRIS_BERIS_BITS) != 0U) {
    // Break error
    uart->info->status.rx_break = 1U;
    event |= ARM_USART_EVENT_RX_BREAK;
    hw_set_bits(&uart->h->icr, UART_UARTICR_BEIC_BITS);
  }
  
  if ((uart->h->ris & UART_UARTRIS_PERIS_BITS) != 0U) {
    uart->info->status.rx_parity_error = 1U;
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
    hw_set_bits(&uart->h->icr, UART_UARTICR_PEIC_BITS);
  }
  
  if ((uart->h->ris & UART_UARTRIS_FERIS_BITS) != 0U) {
    uart->info->status.rx_framing_error = 1U;
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    hw_set_bits(&uart->h->icr, UART_UARTICR_FEIC_BITS);
  }

  if ((uart->h->ris & UART_UARTRIS_RTRIS_BITS) != 0U) {
//    uart->info->status.rx_framing_error = 1U;
    //event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
    hw_set_bits(&uart->h->icr, UART_UARTICR_RTIC_BITS);
  }

  if (uart->xfer->rx_buf != NULL) {
    if (uart_is_readable(uart->reg)) {
      uart->xfer->rx_buf[uart->xfer->rx_cnt++] = (uint8_t)uart_getc(uart->reg);
    }
    if (uart->xfer->rx_cnt == uart->xfer->rx_num)
    {
      // Receive operation is completed
      uart->xfer->rx_buf = NULL;
      uart->info->status.rx_busy = 0U;
      // Signal receive complete
      event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
    }
  }
  else {  
    // Current byte is lost, send overflow event
    event |= ARM_USART_EVENT_RX_OVERFLOW;
  }

  if (event != 0U){
    // Signal error event
    if (uart->info->cb_event != NULL) {
      uart->info->cb_event (event);
    }
    event = 0U;
  }
}


// USART0 Driver Wrapper functions
#if USE_UART0
static ARM_USART_CAPABILITIES  UART0_GetCapabilities (void)                                                { return UART_GetCapabilities (&UART0_Resources);                         }
static int32_t                 UART0_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return UART_Initialize      (&UART0_Resources, cb_event);               }
static int32_t                 UART0_Uninitialize    (void)                                                { return UART_Uninitialize    (&UART0_Resources);                         }
static int32_t                 UART0_PowerControl    (ARM_POWER_STATE state)                               { return UART_PowerControl    (&UART0_Resources, state);                  }
static int32_t                 UART0_Send            (const void *data, uint32_t num)                      { return UART_Send            (&UART0_Resources, data, num);              }
static int32_t                 UART0_Receive         (void *data, uint32_t num)                            { return UART_Receive         (&UART0_Resources, data, num);              }
static int32_t                 UART0_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return UART_Transfer        (&UART0_Resources, data_out, data_in, num); }
static uint32_t                UART0_GetTxCount      (void)                                                { return UART_GetTxCount      (&UART0_Resources);                         }
static uint32_t                UART0_GetRxCount      (void)                                                { return UART_GetRxCount      (&UART0_Resources);                         }
static int32_t                 UART0_Control         (uint32_t control, uint32_t arg)                      { return UART_Control         (&UART0_Resources, control, arg);           }
static ARM_USART_STATUS        UART0_GetStatus       (void)                                                { return UART_GetStatus       (&UART0_Resources);                         }
static int32_t                 UART0_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return UART_SetModemControl (&UART0_Resources, control);                }
static ARM_USART_MODEM_STATUS  UART0_GetModemStatus  (void)                                                { return UART_GetModemStatus  (&UART0_Resources);                         }

// USART1 Driver Control Block
extern ARM_DRIVER_USART Driver_USART0;
       ARM_DRIVER_USART Driver_USART0 = {
    UART_GetVersion,
    UART0_GetCapabilities,
    UART0_Initialize,
    UART0_Uninitialize,
    UART0_PowerControl,
    UART0_Send,
    UART0_Receive,
    UART0_Transfer,
    UART0_GetTxCount,
    UART0_GetRxCount,
    UART0_Control,
    UART0_GetStatus,
    UART0_SetModemControl,
    UART0_GetModemStatus
};
#endif

// USART1 Driver Wrapper functions
#if USE_UART1
static ARM_USART_CAPABILITIES  UART1_GetCapabilities (void)                                                { return UART_GetCapabilities (&UART1_Resources);                         }
static int32_t                 UART1_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return UART_Initialize      (&UART1_Resources, cb_event);               }
static int32_t                 UART1_Uninitialize    (void)                                                { return UART_Uninitialize    (&UART1_Resources);                         }
static int32_t                 UART1_PowerControl    (ARM_POWER_STATE state)                               { return UART_PowerControl    (&UART1_Resources, state);                  }
static int32_t                 UART1_Send            (const void *data, uint32_t num)                      { return UART_Send            (&UART1_Resources, data, num);              }
static int32_t                 UART1_Receive         (void *data, uint32_t num)                            { return UART_Receive         (&UART1_Resources, data, num);              }
static int32_t                 UART1_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return UART_Transfer        (&UART1_Resources, data_out, data_in, num); }
static uint32_t                UART1_GetTxCount      (void)                                                { return UART_GetTxCount      (&UART1_Resources);                         }
static uint32_t                UART1_GetRxCount      (void)                                                { return UART_GetRxCount      (&UART1_Resources);                         }
static int32_t                 UART1_Control         (uint32_t control, uint32_t arg)                      { return UART_Control         (&UART1_Resources, control, arg);           }
static ARM_USART_STATUS        UART1_GetStatus       (void)                                                { return UART_GetStatus       (&UART1_Resources);                         }
static int32_t                 UART1_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return UART_SetModemControl (&UART1_Resources, control);                }
static ARM_USART_MODEM_STATUS  UART1_GetModemStatus  (void)                                                { return UART_GetModemStatus  (&UART1_Resources);                         }

// USART1 Driver Control Block
extern ARM_DRIVER_USART Driver_USART1;
       ARM_DRIVER_USART Driver_USART1 = {
    UART_GetVersion,
    UART1_GetCapabilities,
    UART1_Initialize,
    UART1_Uninitialize,
    UART1_PowerControl,
    UART1_Send,
    UART1_Receive,
    UART1_Transfer,
    UART1_GetTxCount,
    UART1_GetRxCount,
    UART1_Control,
    UART1_GetStatus,
    UART1_SetModemControl,
    UART1_GetModemStatus
};

#endif



