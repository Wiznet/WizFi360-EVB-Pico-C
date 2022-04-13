
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

#include "hardware/regs/addressmap.h"


/*

 * Define the Device Header File: 

 */

#define CMSIS_device_header "RP2040.h"//DEVICE_RP2040_FILE//"ARMCM0plus.h"//"RP2040.h"



/* ARM.FreeRTOS::RTOS:Config:CMSIS RTOS2:10.4.6 */

#define RTE_RTOS_FreeRTOS_CONFIG_RTOS2  /* RTOS FreeRTOS Config for CMSIS RTOS2 API */

/* ARM.FreeRTOS::RTOS:Core:Cortex-M:10.4.6 */

#define RTE_RTOS_FreeRTOS_CORE          /* RTOS FreeRTOS Core */

/* ARM.FreeRTOS::RTOS:Event Groups:10.4.6 */

#define RTE_RTOS_FreeRTOS_EVENTGROUPS   /* RTOS FreeRTOS Event Groups */

/* ARM.FreeRTOS::RTOS:Heap:Heap_4:10.4.6 */

#define RTE_RTOS_FreeRTOS_HEAP_4        /* RTOS FreeRTOS Heap 4 */

/* ARM.FreeRTOS::RTOS:Timers:10.4.6 */

#define RTE_RTOS_FreeRTOS_TIMERS        /* RTOS FreeRTOS Timers */

/* ARM::CMSIS:RTOS2:FreeRTOS:Cortex-M:10.4.6 */

#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */

        #define RTE_CMSIS_RTOS2_FreeRTOS        /* CMSIS-RTOS2 FreeRTOS */

/* Keil.ARM Compiler::Compiler:Event Recorder:DAP:1.4.0 */

//#define RTE_Compiler_EventRecorder

//          #define RTE_Compiler_EventRecorder_DAP

#define RTE_Security_mbedTLS            /* Security mbed TLS */
/*  Keil.ARM Compiler::Compiler:I/O:STDOUT:User:1.2.0 */
#define RTE_Compiler_IO_STDOUT          /* Compiler I/O: STDOUT */
          #define RTE_Compiler_IO_STDOUT_User     /* Compiler I/O: STDOUT User */
/*  Keil::CMSIS Driver:WiFi:WizFi360:UART:1.5.0 */
#define RTE_Drivers_WiFi_WIZFI360_UART   /* Driver WiFi WIZFI360 (UART) */
/*  MDK-Packs::IoT Client:AWS:4.0.0 */
#define RTE_IoT_Client_AWS              /* AWS IoT Client */
/*  MDK-Packs::IoT Utility:AWS:Common:1.1.0 */
#define RTE_IoT_AWS_Base                /* AWS Base */
/*  MDK-Packs::IoT Utility:AWS:MQTT:2.1.0 */
#define RTE_IoT_AWS_MQTT                /* AWS MQTT */
/*  MDK-Packs::IoT Utility:AWS:Platform:CMSIS-RTOS2 mbedTLS:1.0.0 */
#define RTE_IoT_AWS_Platform            /* AWS Platform */
        #define RTE_IoT_AWS_Platform_RTOS2      /* AWS Platform CMSIS-RTOS2 */
        #define RTE_IoT_AWS_Platform_mbedTLS    /* AWS Platform mbedTLS */
/*  MDK-Packs::IoT Utility:Socket:WiFi:1.0.0 */
#define RTE_IoT_Socket                  /* IoT Socket */
        #define RTE_IoT_Socket_WiFi             /* IoT Socket: WiFi */





#endif /* RTE_COMPONENTS_H */

