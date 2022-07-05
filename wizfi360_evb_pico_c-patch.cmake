# CMake minimum required version
cmake_minimum_required(VERSION 3.12)

# Find git
find_package(Git)

if(NOT Git_FOUND)
	message(FATAL_ERROR "Could not find 'git' tool for WIZFI360-EVB-PICO-C patching")
endif()

message("WIZFI360-EVB-PICO-C patch utils found")

set(WIZFI360_EVB_PICO_C_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMSIS_5_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/CMSIS_5")
set(CMSIS_DRIVER_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/CMSIS-Driver")
set(CMSIS_FREERTOS_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/CMSIS-FreeRTOS")
set(IOT_SOCKET_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/IoT_Socket")
set(MBEDTLS_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/CMSIS-Driver")
set(PICO_EXTRAS_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/pico-extras")
set(PICO_SDK_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/pico-sdk")
set(PICO_SDK_TINYUSB_SRC_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/libraries/lib/tinyusb")
set(WIZFI360_EVB_PICO_C_PATCH_DIR "${WIZFI360_EVB_PICO_C_SRC_DIR}/patches")

# Delete untracked files in CMSIS_5
if(EXISTS "${CMSIS_5_SRC_DIR}/.git")
	message("cleaning CMSIS_5...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_5_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_5_SRC_DIR} reset --hard)
	message("CMSIS_5 cleaned")
endif()

# Delete untracked files in CMSIS-Driver
if(EXISTS "${CMSIS_DRIVER_SRC_DIR}/.git")
	message("cleaning CMSIS-Driver...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_DRIVER_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_DRIVER_SRC_DIR} reset --hard)
	message("CMSIS-Driver cleaned")
endif()

# Delete untracked files in CMSIS-FreeRTOS
if(EXISTS "${CMSIS_FREERTOS_SRC_DIR}/.git")
	message("cleaning CMSIS-FreeRTOS...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_FREERTOS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_FREERTOS_SRC_DIR} reset --hard)
	message("CMSIS-FreeRTOS cleaned")
endif()

# Delete untracked files in IoT_Socket
if(EXISTS "${IOT_SOCKET_SRC_DIR}/.git")
	message("cleaning IoT_Socket...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOT_SOCKET_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOT_SOCKET_SRC_DIR} reset --hard)
	message("IoT_Socket cleaned")
endif()

# Delete untracked files in mbedtls
if(EXISTS "${MBEDTLS_SRC_DIR}/.git")
	message("cleaning mbedtls...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${MBEDTLS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${MBEDTLS_SRC_DIR} reset --hard)
	message("mbedtls cleaned")
endif()

# Delete untracked files in pico-extras
if(EXISTS "${PICO_EXTRAS_SRC_DIR}/.git")
	message("cleaning pico-extras...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} reset --hard)
	message("pico-extras cleaned")
endif()

# Delete untracked files in pico-sdk
if(EXISTS "${PICO_SDK_SRC_DIR}/.git")
	message("cleaning pico-sdk...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} reset --hard)
	message("pico-sdk cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${WIZFI360_EVB_PICO_C_SRC_DIR} submodule update --init)

# Delete untracked files in tinyusb
if(EXISTS "${PICO_SDK_TINYUSB_SRC_DIR}/.git")
	message("cleaning pico-sdk tinyusb...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} reset --hard)
	message("pico-sdk tinyusb cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} submodule update --init)

# CMSIS-FreeRTOS patch
message("submodules CMSIS-FreeRTOS initialised")
 
file(GLOB CMSIS_FREERTOS_PATCHES
	"${WIZFI360_EVB_PICO_C_PATCH_DIR}/01_cmsis_freertos_cmsis_os2.patch"
	)

foreach(CMSIS_FREERTOS_PATCH IN LISTS CMSIS_FREERTOS_PATCHES)
	message("Running patch ${CMSIS_FREERTOS_PATCH}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} apply ${CMSIS_FREERTOS_PATCH}
		WORKING_DIRECTORY ${CMSIS_FREERTOS_SRC_DIR}
	)
endforeach()
