cmake_minimum_required(VERSION 3.16)

include(Sdk/pico-sdk/pico_sdk_init.cmake)

include(Sdk/SdkConfig.cmake)

project(PicoSdkStartTemplate)

pico_sdk_init()

add_link_options("-Wl,--print-memory-usage")

set(SOURCES)
set(INCLUDE_DIRS)

# To use Regex
# file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Source/*.c")

add_subdirectory("Source")

add_executable(PicoSdkStartTemplate ${SOURCES})

include_directories(${INCLUDE_DIRS})

target_link_libraries(PicoSdkStartTemplate pico_stdlib)

if(${PICO_BOARD} STREQUAL "pico_w")
    target_link_libraries(PicoSdkStartTemplate pico_cyw43_arch_none)
endif()

# PicoProbe uses UART
pico_enable_stdio_usb(PicoSdkStartTemplate 0)
pico_enable_stdio_uart(PicoSdkStartTemplate 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(PicoSdkStartTemplate)
