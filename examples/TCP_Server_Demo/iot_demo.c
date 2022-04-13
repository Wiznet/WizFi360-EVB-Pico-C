/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>

#include "port_common.h"
#include "timer.h"
#include "iot_socket.h"

/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */
/* Buffer */
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)

/* TCP */
#define TARGET_PORT 5000
#define RECV_TIMEOUT (10 * 1000)

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */

/* TCP */
static uint8_t g_tcp_send_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_tcp_recv_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_tcp_target_ip[4] = {192, 168, 2, 101};

/* Timer  */
static volatile uint32_t g_msec_cnt = 0;

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */

/* TCP */
int recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout);

/* Timer  */
static void repeating_timer_callback(void);
static time_t millis(void);

/**
 * ----------------------------------------------------------------------------------------------------
 * demo
 * ----------------------------------------------------------------------------------------------------
 */
int demo( void )
{
    int retval = 0;
    int32_t af;
    uint8_t client_ip[4];
    uint32_t ip_len = 4;
    uint8_t sock;
    uint8_t sd;
    uint32_t send_cnt = 0;

    wizchip_1ms_timer_initialize(repeating_timer_callback);

    af = IOT_SOCKET_AF_INET;

    sock = iotSocketCreate (af, IOT_SOCKET_SOCK_STREAM, IOT_SOCKET_IPPROTO_TCP);
    if (sock < 0){
        printf("iotSocketCreate failed %d\r\n", sock);
        return sock;
    }
    iotSocketBind(sock, client_ip, sizeof(client_ip), TARGET_PORT);
    iotSocketListen(sock, 1);
    sd = iotSocketAccept(sock, NULL, NULL, NULL);
    printf("Client Accept sd = %d\r\n", sd);

    while(1)
    {   
        sprintf(g_tcp_send_buf, "Hello WizFi360-EVB-Pico-C TCP_Server_Demo send_cnt = %d\r\n", send_cnt++);
        retval = iotSocketSend(sd, g_tcp_send_buf, strlen(g_tcp_send_buf));
        printf("iotSocketSend retval = %d\r\n", retval);

        retval = recv_timeout(sd, g_tcp_recv_buf, ETHERNET_BUF_MAX_SIZE, RECV_TIMEOUT);
        if(retval > 0){
            printf("recv_timeout retval = %d\r\n", retval);
            printf("%.*s\r\n", retval, g_tcp_recv_buf);
        }
    }
}

/* Timer */
static void repeating_timer_callback(void)
{
    g_msec_cnt++;
}

static time_t millis(void)
{
    return g_msec_cnt;
}

/* TCP */
int recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout)
{
    int32_t ret;
    uint32_t n = timeout;

    ret = iotSocketSetOpt (((uint8_t)ctx), IOT_SOCKET_SO_RCVTIMEO, &n, sizeof(n));
    if (ret < 0)
        return (-1);

    ret = iotSocketRecv ((uint8_t)ctx, buf, len);
    if (ret < 0)
        return (0);

    return ret;
}
