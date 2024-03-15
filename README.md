[![Build](https://github.com/HouseSimOn/PicoSdkStartTemplate/actions/workflows/build_cmake.yml/badge.svg)](https://github.com/HouseSimOn/PicoSdkStartTemplate/actions/workflows/build_cmake.yml)

# PicoSdkStartTemplate
Start repository template with Pi Pico and Pi PicoW SDK

## Template version
__0.0.1__

# Documentation
https://raspberrypi.github.io/pico-sdk-doxygen/modules.html

## Performed steps

Platform: Ubuntu

Based on: https://github.com/raspberrypi/pico-sdk

1. Install `sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`.
2. Repository created and cloned `git clone https://github.com/HouseSimOn/PicoSdkStartTemplate.git`.
3. Added submodule using `git submodule add https://github.com/raspberrypi/pico-sdk.git Sources/Sdk/pico-sdk` command.
4. Commit: `git commit -m "Added pico-sdk submodule"`.
5. Call `git submodule update --init` in the main directory and then go to the pico-sdk and call this command again.
6. Create settings.json for intellisense work with following content
    ```json
    { 
        "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools" 
    } 
    ```

### printf - via USB (Ubuntu minicom)
1. To CMakeLists.txt add following lines:
    ```
    pico_enable_stdio_usb(PicoSdkStartTemplate 1)
    pico_enable_stdio_uart(PicoSdkStartTemplate 0)
    ```
2. Find connected device: `ls -l /dev/ttyUSB* /dev/ttyACM*`
3. Start minicom: `sudo minicom -b 115200 -o -D /dev/ttyACM0` 

### USB Logger - macOS

1. Find Pico device
    ```
    ls /dev/tty.usb* 
    ```
2. Run `screen` application
    ```
    screen /dev/tty.usbmodem1101 115200
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