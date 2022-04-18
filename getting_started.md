# Getting Started with WizFi360-EVB-Pico-C Examples

These sections will guide you through a series of steps from configuring development environment to running WizFi360-EVB-Pico-C examples using the **WIZnet's Wi-Fi products**.

- [Getting Started with WizFi360-EVB-Pico-C Examples](#getting-started-with-wizfi360-evb-pico-c-examples)
  - [Development environment configuration](#development-environment-configuration)
  - [Hardware requirements](#hardware-requirements)
  - [WizFi360-EVB-Pico-C example structure](#wizfi360-evb-pico-c-example-structure)
  - [WizFi360-EVB-Pico-C example testing](#wizfi360-evb-pico-c-example-testing)



<a name="development_environment_configuration"></a>
## Development environment configuration

To test the WizFi360-EVB-Pico-C examples, the development environment must be configured to use Raspberry Pi Pico or WizFi360-EVB-Pico.

The WizFi360-EVB-Pico-C examples were tested by configuring the development environment for **Windows**. Please refer to the '**9.2. Building on MS Windows**' section of '**Getting started with Raspberry Pi Pico**' document below and configure accordingly.

- [**Getting started with Raspberry Pi Pico**][link-getting_started_with_raspberry_pi_pico]

**Visual Studio Code** was used during development and testing of WizFi360-EVB-Pico-C examples, the guide document in each directory was prepared also base on development with Visual Studio Code. Please refer to corresponding document.



<a name="hardware_requirements"></a>
## Hardware requirements

The WizFi360-EVB-Pico-C examples use **Raspberry Pi Pico** and **WizFi360-EVB** - Wi-Fi module built on WIZnet's [**WizFi360**][link-wizfi360] Wi-Fi module or **WizFi360-EVB-Pico** - Wi-Fi module built on [**RP2040**][link-rp2040] and WIZnet's [**WizFi360**][link-wizfi360] Wi-Fi module.

- [**Raspberry Pi Pico**][link-raspberry_pi_pico]

![][link-raspberry_pi_pico_main]

- [**WizFi360-EVB-Pico**][link-wizfi360-evb-pico]

![][link-wzfi360-evb-pico_main]



<a name="wizfi360-evb-pico-c_example_structure"></a>
## WizFi360-EVB-Pico-C example structure

Examples are available at '**WizFi360-EVB-Pico-C/examples/**' directory. As of now, following examples are provided.

- [**SSL Client Demo**][link-ssl_client_demo]
- [**TCP Client Demo**][link-tcp_client_demo]
- [**TCP Server Demo**][link-tcp_server_demo]

Note that **CMSIS_5**, **CMSIS_Driver**, **CMSIS_FreeRTOS**, **IoT_Socket**, **mbedtls**, **pico-sdk** are needed to run WizFi360-EVB-Pico-C examples.

Libraries are located in the '**WIZFI360-EVB-PICO-C/libraries/**' directory.

- [**CMSIS_5**][link-cmsis_5]
- [**CMSIS_Driver**][link-cmsis_driver]
- [**CMSIS_FreeRTOS**][link-cmsis_freertos]
- [**IoT_Socket**][link-iot_socket]
- [**mbedtls**][link-mbedtls]
- [**pico-sdk**][link-pico_sdk]
- [**pico-extras**][link-pico_extras]

If you want to modify the code that MCU-dependent and use a MCU other than **RP2040**, you can modify it in the **WIZFI360-EVB-PICO-C/port/** directory.

port is located in the '**WIZFI360-EVB-PICO-C/port/**' directory.

- [**FreeRTOS-Kernel**][link-port_freertos_kernel]
- [**mbedtls**][link-port_mbedtls]
- [**RTE**][link-port_rte]
- [**timer**][link-port_timer]
- [**WizFi360**][link-port_wizfi360]



<a name="wizfi360-evb-pico-c_example_testing"></a>
## WizFi360-EVB-Pico-C example testing

1. Download

If the WizFi360-EVB-Pico-C examples are cloned, the library set as a submodule is an empty directory. Therefore, if you want to download the library set as a submodule together, clone the WizFi360-EVB-Pico-C examples with the following Git command.

```cpp
/* Change directory */
// change to the directory to clone
cd [user path]

// e.g.
cd D:/RP2040

/* Clone */
git clone --recurse-submodules https://github.com/Wiznet/WizFi360-EVB-Pico-C.git
```

With Visual Studio Code, the library set as a submodule is automatically downloaded, so it doesn't matter whether the library set as a submodule is an empty directory or not, so refer to it.

2. Patch

With Visual Studio Code, each library set as a submodule is automatically patched, but if you do not use Visual Studio Code, each library set as a submodule must be manually patched with the Git commands below in each library directory.

- CMSIS-FreeRTOS

```cpp
/* Change directory */
// change to the 'CMSIS-FreeRTOS' library directory
cd [user path]/WizFi360-EVB-Pico-C/libraries/CMSIS-FreeRTOS

// e.g.
cd D:/RP2040/WizFi360-EVB-Pico-C/libraries/CMSIS-FreeRTOS

/* Patch */
git apply ../../patches/01_CMSIS-FreeRTOS_cmsis_os2.patch
```

3. Test

Please refer to 'README.md' in each example directory to find detail guide for testing WizFi360-EVB-Pico-C examples.

<!--
Link
-->

[link-getting_started_with_raspberry_pi_pico]: https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf

[link-wizfi360]: https://docs.wiznet.io/Product/Wi-Fi-Module/WizFi360/wizfi360

[link-rp2040]: https://www.raspberrypi.org/products/rp2040/
[link-raspberry_pi_pico]: https://www.raspberrypi.org/products/raspberry-pi-pico/
[link-raspberry_pi_pico_main]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/getting_started/raspberry_pi_pico_main.png

[link-wizfi360-evb-pico]: https://docs.wiznet.io/Product/Open-Source-Hardware/wizfi360-evb-pico
[link-wizfi360-evb-pico_main]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/getting_started/wizfi360-evb-pico_main.png

[link-ssl_client_demo]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/examples/SSL_Client_Demo
[link-tcp_client_demo]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/examples/TCP_Client_Demo
[link-tcp_server_demo]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/examples/TCP_Server_Demo

[link-cmsis_5]: https://github.com/ARM-software/CMSIS-Driver
[link-cmsis_driver]: https://github.com/ARM-software/CMSIS-Driver
[link-cmsis_freertos]: https://github.com/ARM-software/CMSIS-FreeRTOS
[link-iot_socket]: https://github.com/MDK-Packs/IoT_Socket
[link-mbedtls]: https://github.com/ARMmbed/mbedtls
[link-pico_sdk]: https://github.com/raspberrypi/pico-sdk
[link-pico_extras]: https://github.com/raspberrypi/pico-extras

[link-port_freertos_kernel]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/port/FreeRTOS-Kernel/inc
[link-port_mbedtls]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/port/mbedtls/inc
[link-port_rte]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/port/RTE/inc
[link-port_timer]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/port/timer
[link-port_wizfi360]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/tree/main/port/WizFi360
