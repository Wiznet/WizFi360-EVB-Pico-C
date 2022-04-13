set(TARGET_NAME tcp_server_demo)

add_executable(${TARGET_NAME}
        ${TARGET_NAME}.c
        app_main.c
        socket_startup.c
        iot_demo.c
        )

target_include_directories(${TARGET_NAME} PUBLIC
        ${PORT_DIR}
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        pico_stdlib
        cmsis_core
        CMSIS_FREERTOS_FILES
        hardware_spi
        hardware_dma
        FREERTOS_FILES
        TIMER_FILES
        WIZFI360_FILES
        IOT_SOCKET_FILES
        )

pico_enable_stdio_usb(${TARGET_NAME} 0)
pico_enable_stdio_uart(${TARGET_NAME} 1)

pico_add_extra_outputs(${TARGET_NAME})
