cmake_minimum_required(VERSION 2.4.6)

project(reprap-firmware)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# Target atmega644p
add_definitions( -mmcu=atmega644p )
# Clock rate
add_definitions( -DF_CPU=16000000 )
# Optimize for size
add_definitions( -Os -fpack-struct -fshort-enums )
# Warnings
add_definitions( -Wall )

add_executable(firmware
  main.c
  digital.c
  uart.c
  )

add_custom_command(TARGET firmware POST_BUILD COMMAND avr-objcopy -O ihex -R .eeprom firmware firmware.hex)