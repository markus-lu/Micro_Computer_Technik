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

#include "types.h"
#include "LPC17xx.h"

// Power Control for Peripherals register
#define TIMER1_POWER_CONTROL_PERIPHERALS_BIT (1 << 2)
#define TIMER2_POWER_CONTROL_PERIPHERALS_BIT (1 << 22)


#define CCLK_DIVIDED_BY_1 0b01
#define CCLK_DIVIDED_BY_2 0b10
#define CCLK_DIVIDED_BY_4 0b00
#define CCLK_DIVIDED_BY_8 0b11
#define TIMER1_PCLKSEL_BIT(value) (value << 4)
#define TIMER2_PCLKSEL_BIT(value) (value << 12)

// TCR
#define TIMER_ENABLE_BIT (1 << 0)
#define TIMER_RESET_BIT (1 << 1)

// MCR
#define INTERRUPT_ON_MATCH_BIT 0b001
#define RESET_ON_MATCH_BIT     0b010
#define STOP_ON_MATCH_BIT      0b100

#define MATCH_CONTROL_REGISTER_WIDTH 3

void timer_init_timer1(void);

void timer_init_timer2(void);

uint32_t timer_get_count(LPC_TIM_TypeDef *timer);

void timer_set_prescaler(LPC_TIM_TypeDef *timer, uint32_t value);

void timer_enable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value);

void timer_clear_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match);

bool timer_has_timer1_ticked(void);

bool timer_has_timer2_ticked(void);

void timer_start_timer(LPC_TIM_TypeDef *timer);

void timer_stop_timer(LPC_TIM_TypeDef *timer);

void timer_deinit_timer1(void);

void timer_deinit_timer2(void);

#endif
