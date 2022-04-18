# How to Test TCP_Server_Demo Example



## Step 1: Prepare software

The following serial terminal programs are required for Loopback example test, download and install from below links.

- [**Tera Term**][link-tera_term]
- [**Hercules**][link-hercules]

## Step 2: Prepare hardware

1. Connect WizFi360-EVB-Pico to desktop or laptop using 5 pin micro USB cable.

## Step 3: Setup TCP_Server_Demo Example

To test the TCP_Server_Demo example, minor settings shall be done in code.

1. Setup SSID information in 'socket_startup.c' in 'WizFi360-EVB-Pico-C/examples/TCP_Server_Demo/' directory.

Setup the SSID information.

```cpp
/* SSID */
#define SSID            ""
#define PASSWORD        ""
```

2. Setup network configuration such as PORT in 'iot_demo.c' which is the TCP_Server_Demo example in 'WizFi360-EVB-Pico-C/examples/TCP_Server_Demo/' directory.

Setup PORT settings to suit your network environment.

```cpp
/* TCP */
#define TARGET_PORT 5000
```

## Step 4: Build

1. After completing the TCP_Server_Demo example configuration, click 'build' in the status bar at the bottom of Visual Studio Code or press the 'F7' button on the keyboard to build.

2. When the build is completed, 'tcp_server_demo.uf2' is generated in 'WizFi360-EVB-Pico-C/build/examples/TCP_Server_Demo/' directory.



## Step 5: Upload and Run

1. While pressing the BOOTSEL button of WizFi360-EVB-Pico power on or reset the board, the USB mass storage 'RPI-RP2' is automatically mounted.

![][link-raspberry_pi_pico_usb_mass_storage]

2. Drag and drop 'tcp_server_demo.uf2' onto the USB mass storage device 'RPI-RP2'.

3. Connect to the serial COM port of Raspberry Pi Pico or W5100S-EVB-Pico with Tera Term. and Check the ip address of WizFi360
![][link-teraterm_wizfi360_ip_check]

4. Open the hercules in your desktop or laptop. Set the port to the same number as the TARGET_PORT in your firmware and Set the IP you checked in itme 3. Click connect on the tcp client tab.
![][link-hercules_tcp_server_setting]

5. If the TCP_Server_Demo example works normally on WizFi360-EVB-Pico, you can see the TCP Data in hercules.
![][link-hercules_tcp_client_result]



<!--
Link
-->

[link-tera_term]: https://osdn.net/projects/ttssh2/releases/
[link-hercules]: https://www.hw-group.com/software/hercules-setup-utility
[link-raspberry_pi_pico_usb_mass_storage]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Server_Demo/raspberry_pi_pico_usb_mass_storage.png


[link-teraterm_wizfi360_ip_check]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Server_Demo/teraterm_wizfi360_ip_check.png

[link-hercules_tcp_client_setting]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Server_Demo/hercules_tcp_client_setting.png                               

[link-hercules_tcp_client_result]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Server_Demo/hercules_tcp_client_result.png                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     