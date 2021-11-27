#include "Timer.h"

static volatile bool timer1_ticked;
static volatile bool timer2_ticked;

void timer_init_timer1 () {
	// Timer 1 Power an
    LPC_SC->PCONP |= TIMER1_POWER_CONTROL_PERIPHERALS_BIT;
    // Clock Festsetzung
    LPC_SC->PCLKSEL0 |= TIMER1_PCLKSEL_BIT(CCLK_DIVIDED_BY_1);
    // Clock resetten
    LPC_TIM1->TCR = TIMER_RESET_BIT;
    // Reset weg nehmen
    LPC_TIM1->TCR = 0;
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    NVIC_EnableIRQ(TIMER1_IRQn);
}

void timer_init_timer2 () {
    LPC_SC->PCONP |= TIMER2_POWER_CONTROL_PERIPHERALS_BIT;
    LPC_SC->PCLKSEL1 |= TIMER2_PCLKSEL_BIT(CCLK_DIVIDED_BY_1);
    LPC_TIM2->TCR = TIMER_RESET_BIT;
    LPC_TIM2->TCR = 0;
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    NVIC_EnableIRQ(TIMER2_IRQn);
}

void timer_start_timer (LPC_TIM_TypeDef *timer) {
    timer->TCR |= TIMER_ENABLE_BIT;
}

void timer_stop_timer (LPC_TIM_TypeDef *timer) {
    timer->TCR &= ~TIMER_ENABLE_BIT;
}

void timer_set_prescaler (LPC_TIM_TypeDef *timer, uint32_t value) {
    timer->PR = value;
}

void timer_enable_match_interrupt (LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value) {
    int match_config = INTERRUPT_ON_MATCH_BIT | RESET_ON_MATCH_BIT;
    // Interrupt mit Reset für Match aktivieren
    timer->MCR |= (match_config << match * MATCH_CONTROL_REGISTER_WIDTH); // Enable Interrupt for Match

    volatile uint32_t *match_register = &timer->MR0;
    match_register += match; // Select correct match register
    *match_register = value; // Set match value
}

void timer_clear_match_interrupt (LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->IR |= (1 << match);
}

bool timer_has_timer1_ticked () {
    bool has_ticked = timer1_ticked;
    timer1_ticked = false;
    return has_ticked;
}

bool timer_has_timer2_ticked () {
    bool has_ticked = timer2_ticked;
    timer2_ticked = false;
    return has_ticked;
}

void timer_deinit_timer1 () {
    timer_stop_timer(LPC_TIM1);
    LPC_TIM1->TCR = TIMER_RESET_BIT;
}

void timer_deinit_timer2 () {
    timer_stop_timer(LPC_TIM2);
    LPC_TIM2->TCR = TIMER_RESET_BIT;
    NVIC_DisableIRQ(TIMER2_IRQn);
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
}

void TIMER1_IRQHandler () {
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    timer_clear_match_interrupt(LPC_TIM1, 1);
    timer1_ticked = true;
}

void TIMER2_IRQHandler () {
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    timer_clear_match_interrupt(LPC_TIM2, 1);
    timer2_ticked = true;
}
