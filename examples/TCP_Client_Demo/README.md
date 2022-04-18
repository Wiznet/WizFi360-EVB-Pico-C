# How to Test TCP_Client_Demo Example



## Step 1: Prepare software

The following serial terminal programs are required for TCP_Client_Demo example test, download and install from below links.

- [**Tera Term**][link-tera_term]
- [**Hercules**][link-hercules]

## Step 2: Prepare hardware

1. Connect WizFi360-EVB-Pico to desktop or laptop using 5 pin micro USB cable.

## Step 3: Setup TCP_Client_Demo Example

To test the TCP_Client_Demo example, minor settings shall be done in code.

1. Setup SSID information in 'socket_startup.c' in 'WizFi360-EVB-Pico-C/examples/TCP_Client_Demo/' directory.

Setup the SSID infomation.

```cpp
/* SSID */
#define SSID            ""
#define PASSWORD        ""
```

2. Setup network configuration such as IP and PORT in 'iot_demo.c' which is the TCP_Client_Demo example in 'WizFi360-EVB-Pico-C/examples/TCP_Client_Demo/' directory.

Setup IP and PORT settings to suit your network environment.

```cpp
/* TCP */
#define TARGET_PORT 6000
static uint8_t g_tcp_target_ip[4] = {192, 168, 2, 101};
```

3. Open the hercules in your desktop or laptop. Set the port to the same number as the TARGET_PORT in your firmware and click listen on the tcp server tab.
![][link-hercules_tcp_server_setting]

## Step 4: Build

1. After completing the TCP_Client_Demo example configuration, click 'build' in the status bar at the bottom of Visual Studio Code or press the 'F7' button on the keyboard to build.

2. When the build is completed, 'tcp_client_demo.uf2' is generated in 'WizFi360-EVB-Pico-C/build/examples/TCP_Client_Demo/' directory.



## Step 5: Upload and Run

1. While pressing the BOOTSEL button of WizFi360-EVB-Pico power on or reset the board, the USB mass storage 'RPI-RP2' is automatically mounted.

![][link-raspberry_pi_pico_usb_mass_storage]

2. Drag and drop 'tcp_client_demo.uf2' onto the USB mass storage device 'RPI-RP2'.

3. If the TCP_Client_Demo example works normally on WizFi360-EVB-Pico, you can see the TCP Data in hercules.

![][link-hercules_tcp_server_result]



<!--
Link
-->

[link-tera_term]: https://osdn.net/projects/ttssh2/releases/
[link-hercules]: https://www.hw-group.com/software/hercules-setup-utility
[link-raspberry_pi_pico_usb_mass_storage]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Client_Demo/raspberry_pi_pico_usb_mass_storage.png

[link-hercules_tcp_server_setting]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Client_Demo/hercules_tcp_server_setting.png

[link-hercules_tcp_server_result]: https://github.com/Wiznet/WizFi360-EVB-Pico-C/blob/main/static/images/TCP_Client_Demo/hercules_tcp_server_result.png                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     