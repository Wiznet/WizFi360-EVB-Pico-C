
# FreeRTOS
add_library(FREERTOS_FILES STATIC)

target_sources(FREERTOS_FILES PUBLIC
        ${FREERTOS_DIR}/event_groups.c
        ${FREERTOS_DIR}/list.c
        ${FREERTOS_DIR}/queue.c
        ${FREERTOS_DIR}/stream_buffer.c
        ${FREERTOS_DIR}/tasks.c
        ${FREERTOS_DIR}/timers.c
        ${FREERTOS_DIR}/portable/GCC/ARM_CM0/port.c  		
        ${FREERTOS_DIR}/portable/MemMang/heap_4.c
)

target_include_directories(FREERTOS_FILES PUBLIC
        ${PORT_DIR}/FreeRTOS-Kernel/inc
        ${PORT_DIR}/RTE/inc
        ${FREERTOS_DIR}/include
        ${FREERTOS_DIR}/portable/GCC/ARM_CM0
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
)

target_link_libraries(FREERTOS_FILES PRIVATE
        pico_stdlib
        cmsis_core
        CMSIS_FREERTOS_FILES
)

# CMSIS-FreeRTOS
add_library(CMSIS_FREERTOS_FILES STATIC)

target_sources(CMSIS_FREERTOS_FILES PUBLIC
        ${CMSIS_FREERTOS_DIR}/source/ARM/clib_arm.c        
        ${CMSIS_FREERTOS_DIR}/source/cmsis_os2.c        
        ${CMSIS_FREERTOS_DIR}/source/freertos_evr.c
        ${CMSIS_FREERTOS_DIR}/source/os_systick.c
)

target_include_directories(CMSIS_FREERTOS_FILES PUBLIC
        ${CMSIS_FREERTOS_DIR}/include
        ${CMSIS_5_DIR}/CMSIS/RTOS2/Include
        ${CMSIS_5_DIR}/CMSIS/Core/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
        ${PORT_DIR}/FreeRTOS-Kernel/inc
        ${PORT_DIR}/RTE/inc
)

target_link_libraries(CMSIS_FREERTOS_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
)

# WizFi360 CMSIS-Driver
add_library(WIZFI360_DRIVER_FILES STATIC)

target_sources(WIZFI360_DRIVER_FILES PUBLIC
        ${WIZFI360_DRIVER_DIR}/BufList/BufList.c
        ${WIZFI360_DRIVER_DIR}/BufList/LinkList.c
        ${WIZFI360_DRIVER_DIR}/WiFi_WizFi360.c
        ${WIZFI360_DRIVER_DIR}/WiFi_WizFi360_Os.c
        ${WIZFI360_DRIVER_DIR}/WizFi360.c
        ${WIZFI360_DRIVER_DIR}/WizFi360_Serial.c
)

target_include_directories(WIZFI360_DRIVER_FILES PUBLIC
        ${WIZFI360_DRIVER_DIR}
        ${WIZFI360_DRIVER_DIR}/BufList
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${CMSIS_5_DIR}/CMSIS/RTOS2/Include
        ${CMSIS_5_DIR}/CMSIS/Core/Include
        ${FREERTOS_DIR}/include
        ${FREERTOS_DIR}/portable/GCC/ARM_CM0
        ${PORT_DIR}/FreeRTOS-Kernel/inc
)

target_link_libraries(WIZFI360_DRIVER_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
        CMSIS_FREERTOS_FILES
        WIZFI360_FILES
)

# IoT_Socket
add_library(IOT_SOCKET_FILES STATIC)

target_sources(IOT_SOCKET_FILES PUBLIC
        ${IOT_SOCKET_DIR}/source/wifi/iot_socket.c
        )

target_include_directories(IOT_SOCKET_FILES PUBLIC
        ${IOT_SOCKET_DIR}/include
        )

target_link_libraries(IOT_SOCKET_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
        CMSIS_FREERTOS_FILES
        WIZFI360_FILES
)