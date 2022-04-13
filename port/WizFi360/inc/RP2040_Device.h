/* -----------------------------------------------------------------------------
 * Copyright (c) 2020 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        16. January 2020
 * $Revision:    V1.0
 * Project:      UART Driver definitions for ST STM32L5xx
 * -------------------------------------------------------------------------- */

#ifndef _RP2040_DEVICE_H_
#define _RP2040_DEVICE_H_

#define USE_UART0 0
#define USE_UART1 1

#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_CTS_PIN 6
#define UART_RTS_PIN 7

#define UART_DEFAULT_BAUDRATE 115200

#endif /* _RP2040_DEVICE_H_ */
