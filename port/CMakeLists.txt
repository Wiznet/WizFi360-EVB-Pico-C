# ioLibrary_Driver


# timer
add_library(TIMER_FILES STATIC)

target_sources(TIMER_FILES PUBLIC
        ${PORT_DIR}/timer/timer.c
        )

target_include_directories(TIMER_FILES PUBLIC
        ${PORT_DIR}/timer
        )

target_link_libraries(TIMER_FILES PRIVATE
        pico_stdlib      
        )

# WizFi360
add_library(WIZFI360_FILES STATIC)

target_sources(WIZFI360_FILES PUBLIC
        ${PORT_DIR}/WizFi360/src/UART_RP2040.c
        )

target_include_directories(WIZFI360_FILES PUBLIC
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${PORT_DIR}/WizFi360/inc
        ${PORT_DIR}/RTE/inc
        )

target_link_libraries(WIZFI360_FILES PRIVATE
        pico_stdlib
        cmsis_core
        hardware_uart
        CMSIS_FREERTOS_FILES
        WIZFI360_DRIVER_FILES
        FREERTOS_FILES
        )
