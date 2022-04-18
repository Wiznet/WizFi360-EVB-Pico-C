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

#include "mbedtls/x509_crt.h"
#include "mbedtls/error.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/compat-1.3.h"
/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */
/* Buffer */
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)

/* SSL */
#define TARGET_PORT 443
#define RECV_TIMEOUT (10 * 1000)

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */

/* SSL */
static uint8_t g_ssl_send_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_ssl_recv_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_ssl_target_ip[4] = {192, 168, 2, 101};
static mbedtls_ctr_drbg_context g_ctr_drbg;
static mbedtls_ssl_config g_conf;
static mbedtls_ssl_context g_ssl;
/* Timer  */
static volatile uint32_t g_msec_cnt = 0;

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */

/* SSL */
static int wizfi_ssl_init(uint8_t *socket_fd);
static int ssl_random_callback(void *p_rng, unsigned char *output, size_t output_len);
static int recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout);

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
    const int *list = NULL;
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

    retval = wizfi_ssl_init(sock);
    if (retval < 0)
    {
        printf(" SSL initialize failed %d\n", retval);
        return retval;
    }

    /* Get ciphersuite information */
    printf(" Supported ciphersuite lists\n");
    list = mbedtls_ssl_list_ciphersuites();
    while (*list)
    {
        printf(" %-42s\n", mbedtls_ssl_get_ciphersuite_name(*list));
        list++;
        if (!*list)
        {
            break;
        }
        printf(" %s\n", mbedtls_ssl_get_ciphersuite_name(*list));
        list++;
    }

    retval = iotSocketConnect (sock, g_ssl_target_ip, 4, TARGET_PORT);
    if (retval < 0){
        iotSocketClose (sock);
        printf("iotSocketConnect failed %d\r\n", retval);
        return retval;
    }

    while(1)
    {   
        sprintf(g_ssl_send_buf, "Hello WizFi360-EVB-Pico-C SSL_Client_Demo send_cnt = %d\r\n", send_cnt++);
        retval = mbedtls_ssl_write(&g_ssl, g_ssl_send_buf, strlen(g_ssl_send_buf));
        printf("mbedtls_ssl_write retval = %d\r\n", retval);

        retval = mbedtls_ssl_read(&g_ssl, g_ssl_recv_buf, ETHERNET_BUF_MAX_SIZE);
        if(retval > 0){
            printf("mbedtls_ssl_read retval = %d\r\n", retval);
            printf("%.*s\r\n", retval, g_ssl_recv_buf);
        }
    }
}


/* SSL */
static int wizfi_ssl_init(uint8_t *socket_fd)
{
    int retval;

    mbedtls_ctr_drbg_init(&g_ctr_drbg);
    mbedtls_ssl_init(&g_ssl);
    mbedtls_ssl_config_init(&g_conf);

    if ((retval = mbedtls_ssl_config_defaults(&g_conf,
                                              MBEDTLS_SSL_IS_CLIENT,
                                              MBEDTLS_SSL_TRANSPORT_STREAM,
                                              MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        printf(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n", retval);
        return -1;
    }
    printf(" Socket descriptor %d\n", socket_fd);

    mbedtls_ssl_conf_authmode(&g_conf, MBEDTLS_SSL_VERIFY_NONE);
    mbedtls_ssl_conf_rng(&g_conf, ssl_random_callback, &g_ctr_drbg);
    mbedtls_ssl_conf_endpoint(&g_conf, MBEDTLS_SSL_IS_CLIENT);
    mbedtls_ssl_conf_read_timeout(&g_conf, RECV_TIMEOUT);

    if ((retval = mbedtls_ssl_setup(&g_ssl, &g_conf)) != 0)
    {
        printf(" failed\n  ! mbedtls_ssl_setup returned %d\n", retval);
        return -1;
    }

    mbedtls_ssl_set_bio(&g_ssl, socket_fd, iotSocketSend, iotSocketRecv, recv_timeout);
    return retval;
}

static int ssl_random_callback(void *p_rng, unsigned char *output, size_t output_len)
{
    int i;

    if (output_len <= 0)
    {
        return 1;
    }
    for (i = 0; i < output_len; i++)
    {
        *output++ = rand() % 0xff;
    }
    srand(rand());
    return 0;
}

static int recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout)
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

/* Timer */
static void repeating_timer_callback(void)
{
    g_msec_cnt++;
}

static time_t millis(void)
{
    return g_msec_cnt;
}
