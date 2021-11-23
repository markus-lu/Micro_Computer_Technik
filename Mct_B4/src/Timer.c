#include "Timer.h"

static volatile bool ticked;

static void init_timer1() {
    LPC_SC->PCONP |= TIMER1_PCONP_BIT;
    LPC_SC->PCLKSEL0 |= TIMER1_PCLKSEL_BIT(CCLK_DIVIDED_BY_1);
    LPC_TIM1->TCR = TIMER_RESET;
    LPC_TIM1->TCR = 0;
}

static void init_timer2() {
    LPC_SC->PCONP |= TIMER2_PCONP_BIT;
    LPC_SC->PCLKSEL1 |= TIMER2_PCLKSEL_BIT(CCLK_DIVIDED_BY_1);
    LPC_TIM2->TCR = TIMER_RESET;
    LPC_TIM2->TCR = 0;
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    NVIC_EnableIRQ(TIMER2_IRQn);
}

static uint32_t get_count(LPC_TIM_TypeDef *timer) {
    return timer->TC;
}

static void start_timer(LPC_TIM_TypeDef *timer) {
    timer->TCR |= TIMER_ENABLE;
}

static void stop_timer(LPC_TIM_TypeDef *timer) {
    timer->TCR &= ~TIMER_ENABLE;
}

static void set_prescaler(LPC_TIM_TypeDef *timer, uint32_t value) {
    timer->PR = value;
}

static void enable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value) {
    int match_config = INTERRUPT_ON_MATCH | RESET_ON_MATCH;
    timer->MCR |= (match_config << match * MATCH_CONTROL_REGISTER_WIDTH); // Enable Interrupt for Match

    volatile uint32_t *match_register = &timer->MR0;
    match_register += match; // Select correct match register
    *match_register = value; // Set match value
}

static void disable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->MCR &= ~(INTERRUPT_ON_MATCH << match * MATCH_CONTROL_REGISTER_WIDTH); // Disable Interrupt for Match
}

static void clear_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->IR |= (1 << match);
}

static bool has_ticked() {
    bool has_ticked = ticked;
    ticked = false;
    return has_ticked;
}

static void deinit_timer1() {
    stop_timer(LPC_TIM1);
    LPC_TIM1->TCR = TIMER_RESET;
}

static void deinit_timer2() {
    stop_timer(LPC_TIM2);
    LPC_TIM2->TCR = TIMER_RESET;
    NVIC_DisableIRQ(TIMER2_IRQn);
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
}

void TIMER2_IRQHandler() {
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    clear_match_interrupt(LPC_TIM2, 1);
    ticked = true;
}

const struct timer Timer = {
        .init_timer1 = init_timer1,
        .init_timer2 = init_timer2,
        .get_count = get_count,
        .set_prescaler = set_prescaler,
        .enable_match_interrupt = enable_match_interrupt,
        .disable_match_interrupt = disable_match_interrupt,
        .clear_match_interrupt = clear_match_interrupt,
        .has_ticked = has_ticked,
        .start_timer = start_timer,
        .stop_timer = stop_timer,
        .deinit_timer1 = deinit_timer1,
        .deinit_timer2 = deinit_timer2,
};
