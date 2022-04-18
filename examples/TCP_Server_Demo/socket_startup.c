/* -----------------------------------------------------------------------------
 * Copyright (c) 2020 Arm Limited (or its affiliates). All rights reserved.
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
 * -------------------------------------------------------------------------- */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "Driver_WiFi.h"

#define SSID            ""
#define PASSWORD        ""

#define SECURITY_TYPE   ARM_WIFI_SECURITY_WPA2

extern ARM_DRIVER_WIFI Driver_WiFi1;

int32_t socket_startup (void) {
  ARM_WIFI_CONFIG_t config;
  int32_t ret;
  uint8_t net_info[4];
  
  printf("Connecting to WiFi ...\r\n");

  ret = Driver_WiFi1.Initialize  (NULL);
  printf("Driver_WiFix.Initialize  (NULL) = %d\r\n", ret);
  
  ret = Driver_WiFi1.PowerControl(ARM_POWER_FULL);
  printf("Driver_WiFix.PowerControl(ARM_POWER_FULL) = %d\r\n", ret);
  
  memset((void *)&config, 0, sizeof(config));

  config.ssid     = SSID;
  config.pass     = PASSWORD;
  config.security = SECURITY_TYPE;
  config.ch       = 0U;

  ret = Driver_WiFi1.Activate(0U, &config);
  printf("Driver_WiFix.Activate(0U, &config) = %d\r\n", ret);

  ret = Driver_WiFi1.IsConnected();  
  printf("Driver_WiFix.IsConnected() = %d\r\n", ret);
  
  if (ret == 0U) {
    printf("WiFi network connection failed!\r\n");
    return (-1);
  } else {
    printf("WiFi network connection succeeded!\r\n");
  }

  Driver_WiFi1.GetOption(0, ARM_WIFI_IP, net_info, 4);
  printf("ARM_WIFI_IP = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

  Driver_WiFi1.GetOption(0, ARM_WIFI_IP_SUBNET_MASK, net_info, 4);
  printf("ARM_WIFI_IP_SUBNET_MASK = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

  Driver_WiFi1.GetOption(0, ARM_WIFI_IP_GATEWAY, net_info, 4);
  printf("ARM_WIFI_IP_GATEWAY = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

  return 0;
}
