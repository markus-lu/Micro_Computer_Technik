#include "Timer.h"

static void init(LPC_TIM_TypeDef *timer) {

}

void get_count(LPC_TIM_TypeDef *timer) {

}

void set_prescaler(LPC_TIM_TypeDef *timer, uint32_t value) {

}

void enable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value) {

}

void disable_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match) {

}

void clear_match_interrupt(LPC_TIM_TypeDef *timer, uint8_t match) {

}

static void deinit(LPC_TIM_TypeDef *timer) {

}

const struct timer Timer = {
        .init = init,
        .get_count = get_count,
        .set_prescaler = set_prescaler,
        .enable_match_interrupt = enable_match_interrupt,
        .disable_match_interrupt = disable_match_interrupt,
        .clear_match_interrupt = clear_match_interrupt,
        .deinit = deinit,
};
