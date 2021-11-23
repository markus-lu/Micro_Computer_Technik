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

#define TIMER1_PCONP_BIT (1 << 2)
#define TIMER2_PCONP_BIT (1 << 22)

#define CCLK_DIVIDED_BY_4 0b00
#define CCLK_DIVIDED_BY_1 0b01
#define CCLK_DIVIDED_BY_2 0b10
#define CCLK_DIVIDED_BY_8 0b11
#define TIMER1_PCLKSEL_BIT(value) (value << 4)
#define TIMER2_PCLKSEL_BIT(value) (value << 12)

// TCR
#define TIMER_ENABLE (1 << 0)
#define TIMER_RESET (1 << 1)

// MCR
#define INTERRUPT_ON_MATCH 0b001
#define RESET_ON_MATCH     0b010
#define STOP_ON_MATCH      0b100

#define MATCH_CONTROL_REGISTER_WIDTH 3

struct timer {
    void (*init_timer1)(void);

    void (*init_timer2)(void);

    uint32_t (*get_count)(LPC_TIM_TypeDef *timer);

    void (*set_prescaler)(LPC_TIM_TypeDef *timer, uint32_t value);

    void (*enable_match_interrupt)(LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value);

    void (*disable_match_interrupt)(LPC_TIM_TypeDef *timer, uint8_t match);

    void (*clear_match_interrupt)(LPC_TIM_TypeDef *timer, uint8_t match);

    bool (*has_timer1_ticked)(void);

    bool (*has_timer2_ticked)(void);

    void (*start_timer)(LPC_TIM_TypeDef *timer);

    void (*stop_timer)(LPC_TIM_TypeDef *timer);

    void (*deinit_timer1)(void);

    void (*deinit_timer2)(void);
};

extern const struct timer Timer;

#endif
