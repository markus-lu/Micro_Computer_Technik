#include "Timer.h"

static void init_timer2() {
    LPC_SC->PCLKSEL1 |= (0b01 << 12);
}

static uint32_t get_count(LPC_TIM_TypeDef *timer) {
    return timer->TC;
}

static void start_timer(LPC_TIM_TypeDef *timer){
	timer->TCR |= 1;
}

static void stop_timer(LPC_TIM_TypeDef *timer){
	timer->TCR &= ~1;
}

static void set_prescaler(LPC_TIM_TypeDef *timer, uint32_t value) {
    timer->PR = value;
}

static void enable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value) {
    timer->MCR |= (0b001 << match * 3); // Enable Interrupt for Match

    volatile uint32_t *match_register = &timer->MR0;
    match_register += match; // Select correct match register
    *match_register = value; // Set match value
}

static void disable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->MCR &= ~(0b1 << match * 3); // Disable Interrupt for Match
}

static void clear_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->IR |= (1 << match);
}

static void deinit(LPC_TIM_TypeDef *timer) {

}

const struct timer Timer = {
        .init_timer2 = init_timer2,
        .get_count = get_count,
        .set_prescaler = set_prescaler,
        .enable_match_interrupt = enable_match_interrupt,
        .disable_match_interrupt = disable_match_interrupt,
        .clear_match_interrupt = clear_match_interrupt,
        .deinit = deinit,
};
