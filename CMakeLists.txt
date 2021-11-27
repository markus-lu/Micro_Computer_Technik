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
include_directories(${PROJECT_FOLDER}/libi2c_lcd_mctlite)

file(GLOB_RECURSE USER_SOURCES "${PROJECT_FOLDER}/src/*.c")
file(GLOB_RECURSE USER_INCLUDES "${PROJECT_FOLDER}/inc/*.h")

file(GLOB_RECURSE HAL_SOURCES "lpc_chip_11u6x/src/*.c")
file(GLOB_RECURSE HAL_INCLUDES "lpc_chip_11u6x/inc/*.h")

file(GLOB_RECURSE LIB_INCLUDES "${PROJECT_FOLDER}/libi2c_lcd_mctlite/*.h")

SET(COMMON_FLAGS "-O3 -fno-common -g3 -Wall -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP")
SET(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-rtti -fno-exceptions")
SET(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

#add_executable(${PROJECT_NAME}.axf ${USER_SOURCES} ${HAL_SOURCES} ${USER_INCLUDES} ${HAL_INCLUDES})
add_library(${PROJECT_NAME}.axf OBJECT ${USER_SOURCES} ${HAL_SOURCES} ${USER_INCLUDES} ${HAL_INCLUDES} ${LIB_INCLUDES})
target_link_libraries(${PROJECT_NAME}.axf ${PROJECT_FOLDER}/libi2c_lcd_mctlite/libi2c_lcd_mctlite.a)

