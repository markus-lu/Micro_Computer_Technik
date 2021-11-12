/*
 ===============================================================================
 Name        : Timer.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Timer Treiber
 ===============================================================================
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "LPC17xx.h"

struct timer {
    void (*init_timer2)(void);
    uint32_t (*get_count)(LPC_TIM_TypeDef* timer);
    void (*set_prescaler)(LPC_TIM_TypeDef* timer, uint32_t value);
    void (*enable_match_interrupt)(LPC_TIM_TypeDef* timer, uint8_t match, uint32_t value);
    void (*disable_match_interrupt)(LPC_TIM_TypeDef* timer, uint8_t match);
    void (*clear_match_interrupt)(LPC_TIM_TypeDef* timer, uint8_t match);
    void (*deinit)(LPC_TIM_TypeDef* timer);
};

extern const struct timer Timer;

#endif
