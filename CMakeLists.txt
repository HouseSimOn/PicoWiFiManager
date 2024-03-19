cmake_minimum_required(VERSION 3.16)

include(Sdk/pico-sdk/pico_sdk_init.cmake)

include(Sdk/SdkConfig.cmake)

project(WifiManager)

pico_sdk_init()

add_link_options("-Wl,--print-memory-usage")

# It adds flags for every file
# add_compile_options(-Wall -Wextra -Wpedantic)

set(SOURCES)
set(INCLUDE_DIRS)

# To use Regex
# file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Source/*.c")

add_subdirectory("Pages")
add_subdirectory("Source")
add_subdirectory("Lib")

# Print SOURCES
# message("_SOURCES: " ${SOURCES})

# It adds flags only for SOURCES files
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wconversion")

# TODO: glob files from Sdk and Lib and disable flags for them - example below
set_source_files_properties("${CMAKE_SOURCE_DIR}/Lib/dhcpserver/dhcpserver.c" PROPERTIES COMPILE_FLAGS "")

add_executable(WifiManager ${SOURCES})

list(
    APPEND INCLUDE_DIRS
    # Do it, because there is lwipopts.h file in the Sdk directory, to configure lwIP stack.
    "${CMAKE_CURRENT_SOURCE_DIR}/Sdk"
)

include_directories(${INCLUDE_DIRS})

target_link_libraries(WifiManager pico_stdlib)

# from documentation: pico_cyw43_arch_none - If you do not need the TCP/IP stack but wish to use the on-board LED.
if(${PICO_BOARD} STREQUAL "pico_w")
    target_link_libraries(WifiManager pico_cyw43_arch_lwip_threadsafe_background pico_lwip_http)
    # target_link_libraries(WifiManager pico_cyw43_arch_lwip_poll pico_lwip_http)
endif()

pico_enable_stdio_usb(WifiManager 1) 
pico_enable_stdio_uart(WifiManager 0) # use it for PicoProbe debugger

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(WifiManager)

pico_set_linker_script(WifiManager ${CMAKE_CURRENT_SOURCE_DIR}/Source/LinkerScript/custom_memmap.ld)
