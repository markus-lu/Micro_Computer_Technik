cmake_minimum_required(VERSION 3.19)
project(Micro_Computer_Technik C)

set(CMAKE_C_STANDARD 11)

add_definitions(-D__USE_CMSIS)

include_directories(CMSIS_CORE_LPC17xx/inc)
include_directories(Mct_A3/src)

add_executable(Micro_Computer_Technik
        CMSIS_CORE_LPC17xx/inc/core_cm3.h
        CMSIS_CORE_LPC17xx/inc/core_cmFunc.h
        CMSIS_CORE_LPC17xx/inc/core_cmInstr.h
        CMSIS_CORE_LPC17xx/inc/LPC17xx.h
        CMSIS_CORE_LPC17xx/inc/system_LPC17xx.h
        CMSIS_CORE_LPC17xx/src/system_LPC17xx.c
        Mct_A3/src/cr_startup_lpc175x_6x.c
        Mct_A3/src/crp.c
        Mct_A3/src/Mct_A3.c
        Mct_A3/src/Mct_A3.h
        )
