#include "Timer.h"

static volatile bool timer1_ticked;
static volatile bool timer2_ticked;

void timer_init_timer1 () {
	// Power Contol for Peripherals Register
	// Bit kontrolieren Stromzufuhr zur angeschlossenden Peripherie
    LPC_SC->PCONP |= TIMER1_POWER_CONTROL_PERIPHERALS_BIT;
    // Peripheral Clock Selection Register
    // Einstellung des Clockteilers von Hauptclock
    LPC_SC->PCLKSEL0 |= TIMER1_PCLKSEL_BIT(CCLK_DIVIDED_BY_1);
    // Timer Control Register
    // Reset setzen
    LPC_TIM1->TCR = TIMER_RESET_BIT;
    // Reset Wegnehmen
    LPC_TIM1->TCR = 0;
    // Nested Vectored Interrupt Controller
    // Falls Interrupt noch anliegt löschung von diesem
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    // Interrupt für Timer 1 aktiviern
    NVIC_EnableIRQ(TIMER1_IRQn);
}

void timer_init_timer2 () {
	// Power Contol for Peripherals Register
	// Bit kontrolieren Stromzufuhr zur angeschlossenden Peripherie
    LPC_SC->PCONP |= TIMER2_POWER_CONTROL_PERIPHERALS_BIT;
    // Peripheral Clock Selection Register
    // Einstellung des Clockteilers von Hauptclock
    LPC_SC->PCLKSEL1 |= TIMER2_PCLKSEL_BIT(CCLK_DIVIDED_BY_1);
    // Timer Control Register
    // Reset setzen
    LPC_TIM2->TCR = TIMER_RESET_BIT;
    // Reset Wegnehmen
    LPC_TIM2->TCR = 0;
    // Nested Vectored Interrupt Controller
    // Falls Interrupt noch anliegt löschung von diesem
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    // Interrupt für Timer 2 aktiviern
    NVIC_EnableIRQ(TIMER2_IRQn);
}

void timer_start_timer (LPC_TIM_TypeDef *timer) {
	// Timer Control Register
	// Enable Bit Setzen um den Timer zu starten
    timer->TCR |= TIMER_ENABLE_BIT;
}

void timer_stop_timer (LPC_TIM_TypeDef *timer) {
    timer->TCR &= ~TIMER_ENABLE_BIT;
}

void timer_set_prescaler (LPC_TIM_TypeDef *timer, uint32_t value) {
	// Prescale-Register
	// Wert setzen
	// wenn PR und PC(Prescale Counter) den gleichen Wert haben tickt der Timer
    timer->PR = value;
}

void timer_enable_match_interrupt (LPC_TIM_TypeDef *timer, uint8_t match, uint32_t value) {
	// Match Control Register
	// Bei einem Match soll der Timer einnen Interupt auslösen und wider von neuem anfangen zu zählen
    int match_config = INTERRUPT_ON_MATCH_BIT | RESET_ON_MATCH_BIT;
    // Interrupt mit Reset für Match aktivieren
    timer->MCR |= (match_config << match * MATCH_CONTROL_REGISTER_WIDTH);

    // Match Register enthällt den Wert auf den der Timer warten soll
    volatile uint32_t *match_register = &timer->MR0;
    // Pionter aufs richtige Match Register setzen
    match_register += match;
    // wert in das Match Register reinspeichern
    *match_register = value;
}

void timer_clear_match_interrupt (LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->IR |= (1 << match);
}

bool timer_has_timer1_ticked () {
	// Merken ob Timer1 getickt hat
    bool has_ticked = timer1_ticked;
    // Timer1 auf false setzen
    timer1_ticked = false;
    // return ob Timer1 getickt hat
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
