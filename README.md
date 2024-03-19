[![Build](https://github.com/HouseSimOn/PicoWifiManager/actions/workflows/build_cmake.yml/badge.svg)](https://github.com/HouseSimOn/PicoWifiManager/actions/workflows/build_cmake.yml)

# Pico Wifi Manager
WiFi Manager for Pico W.

## Repository template
Repository created based on [this template](https://github.com/HouseSimOn/PicoSdkStartTemplate) in version __0.0.1__.

# SDK Documentation
https://raspberrypi.github.io/pico-sdk-doxygen/modules.html

## Setup

Platform: Ubuntu

1. Clone repository.
2. Call `git submodule update --init` in the main directory and then go to the pico-sdk and call this command again. (or `git submodule update --init --recursive`)

### printf - via USB
1. To CMakeLists.txt add following lines:
    ```
    pico_enable_stdio_usb(WifiManager 1)
    pico_enable_stdio_uart(WifiManager 0)
    ```
2. Find connected device: `ls -l /dev/ttyUSB* /dev/ttyACM*`
3. Start minicom: `sudo minicom -b 115200 -o -D /dev/ttyACM0` 

### USB Logger - macOS

1. Find Pico device
    ```
    ls /dev/tty.usb* 
    ```
2. Run screen application
    ```
    screen /dev/tty.usbmodem101 115200
    ```
3. Run `minicom`
    ```
    minicom -D /dev/tty.usbmodem1101 -b 115200
    ```

### `picotool` usage

1. Load .uf2 file to Pico board
    ```
    picotool load -v -x WifiManager.uf2 -f    
    ```

2. Reboot Pico board
    ```
    picotool reboot -f  
    ```