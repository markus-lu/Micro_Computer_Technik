cmake_minimum_required(VERSION 3.19)
project(Micro_Computer_Technik C)

set(CMAKE_C_STANDARD 11)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_LINKER_PREFERENCE arm-none-eabi-ld)

#set(PROJECT_FOLDER Mct_A3)
set(PROJECT_FOLDER Mct_B4)

add_definitions(-D__USE_CMSIS)

include_directories(CMSIS_CORE_LPC17xx/inc)
include_directories(${PROJECT_FOLDER}/inc)
include_directories(include)
include_directories(libi2c_lcd_mctlite)

file(GLOB_RECURSE USER_SOURCES "${PROJECT_FOLDER}/src/*.c")
file(GLOB_RECURSE USER_INCLUDES "${PROJECT_FOLDER}/inc/*.h")

file(GLOB_RECURSE HAL_SOURCES "lpc_chip_11u6x/src/*.c")
file(GLOB_RECURSE HAL_INCLUDES "lpc_chip_11u6x/inc/*.h")

#add_executable(${PROJECT_NAME}.axf ${USER_SOURCES} ${HAL_SOURCES} ${USER_INCLUDES} ${HAL_INCLUDES})
add_library(${PROJECT_NAME}.axf OBJECT ${USER_SOURCES} ${HAL_SOURCES} ${USER_INCLUDES} ${HAL_INCLUDES})
target_link_libraries(${PROJECT_NAME}.axf Mct_B4/lib/libi2c_lcd_mctlite.a)

