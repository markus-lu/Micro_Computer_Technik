#include "Timer.h"

/**
 *  \file    Timer.c
*/

/**
 * \brief Globale Variable um mit dem ISR von Timer 1 zu kommunizieren.
 */
static volatile bool timer1_ticked;

/**
 * \brief Globale Variable um mit dem ISR von Timer 2 zu kommunizieren.
 */
static volatile bool timer2_ticked;

/*********************************************************************/
/**
\brief Diese Funktion initialisiert Timer 1.

 1. Die Peripherie erhält Stromzufuhr durch das PCONP-Register
 2. Die Clock wird im PCLKSEL-Register gesetzt
 3. Timer wird zurückgesetzt
 4. Interrupt wird gecleared
 5. Interrupt wird aktiviert

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
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

/*********************************************************************/
/**
\brief Diese Funktion initialisiert Timer 2.

 1. Die Peripherie erhält Stromzufuhr durch das PCONP-Register
 2. Die Clock wird im PCLKSEL-Register gesetzt
 3. Timer wird zurückgesetzt
 4. Interrupt wird gecleared
 5. Interrupt wird aktiviert

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
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

/*********************************************************************/
/**
\brief Diese Funktion startet einen Timer.

\param  timer
        Pointer zu dem Timer, der gestartet werden soll
        (LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3)

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void timer_start_timer (LPC_TIM_TypeDef *timer) {
	// Timer Control Register
	// Enable Bit Setzen um den Timer zu starten
    timer->TCR |= TIMER_ENABLE_BIT;
}

/*********************************************************************/
/**
\brief Diese Funktion stoppt einen Timer.

\param  timer
        Pointer zu dem Timer, der gestoppt werden soll
        (LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3)

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void timer_stop_timer (LPC_TIM_TypeDef *timer) {
    timer->TCR &= ~TIMER_ENABLE_BIT;
}

/*********************************************************************/
/**
\brief Diese Funktion setzt den Prescaler für einen Timer.

\param  timer
        Pointer zu dem Timer, dessen Prescaler gesetzt werden soll
        (LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3)
\param  value
        Wert, auf den der Prescaler gesetzt werden soll.
        (0 - 4294967295)

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void timer_set_prescaler (LPC_TIM_TypeDef *timer, uint32_t value) {
	// Prescale-Register
	// Wert setzen
	// wenn PR und PC(Prescale Counter) den gleichen Wert haben tickt der Timer
    timer->PR = value;
}

/*********************************************************************/
/**
\brief Diese Funktion aktiviert uns setzt einen Matchwert für einen Timer.

\param  timer
        Pointer zu dem Timer, dessen Match aktiviert werden soll
        (LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3)
\param  match
        Match-Register
        (0 - 3)
\param  value
        Wert, auf den das Match-Register gesetzt werden soll.
        (0 - 4294967295)

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
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

/*********************************************************************/
/**
\brief Diese Funktion cleared das Interrupt-Register für ein Match für einen Timer.

\param  timer
        Pointer zu dem Timer, dessen Match gecleared werden soll
        (LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3)
\param  match
        Match-Register
        (0 - 3)

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void timer_clear_match_interrupt (LPC_TIM_TypeDef *timer, uint8_t match) {
    timer->IR |= (1 << match);
}

/*********************************************************************/
/**
\brief Diese Funktion prüft, ob Timer 1 seit dem letzten Funktionsaufruf einen Interrupt hatte (getickt hat)

 Prüft und aktualisiert globale Variable timer2_ticked.

\param  -

\return Boolean, der angibt, ob Timer 1 getickt hat
        (false / true)

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
bool timer_has_timer1_ticked () {
	// Merken ob Timer1 getickt hat
    bool has_ticked = timer1_ticked;
    // Timer1 auf false setzen
    timer1_ticked = false;
    // return ob Timer1 getickt hat
    return has_ticked;
}


/*********************************************************************/
/**
\brief Diese Funktion prüft, ob Timer 2 seit dem letzten Funktionsaufruf einen Interrupt hatte (getickt hat)

 Prüft und aktualisiert globale Variable timer2_ticked.
\param  -

\return Boolean, der angibt, ob Timer 2 getickt hat
        (false / true)

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
bool timer_has_timer2_ticked () {
    bool has_ticked = timer2_ticked;
    timer2_ticked = false;
    return has_ticked;
}


/*********************************************************************/
/**
\brief Diese Funktion deinitialisiert Timer 1.

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void timer_deinit_timer1 () {
    timer_stop_timer(LPC_TIM1);
    LPC_TIM1->TCR = TIMER_RESET_BIT;
    NVIC_DisableIRQ(TIMER1_IRQn);
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
}

/*********************************************************************/
/**
\brief Diese Funktion deinitialisiert Timer 2.

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void timer_deinit_timer2 () {
    timer_stop_timer(LPC_TIM2);
    LPC_TIM2->TCR = TIMER_RESET_BIT;
    NVIC_DisableIRQ(TIMER2_IRQn);
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
}

/*********************************************************************/
/**
\brief Interrupt Service Routine für Timer 1.

 Setzt globale Variable timer1_ticked auf true.

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void TIMER1_IRQHandler () {
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    timer_clear_match_interrupt(LPC_TIM1, 1);
    timer1_ticked = true;
}


/*********************************************************************/
/**
\brief Interrupt Service Routine für Timer 2.

 Setzt globale Variable timer2_ticked auf true.

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void TIMER2_IRQHandler () {
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    timer_clear_match_interrupt(LPC_TIM2, 1);
    timer2_ticked = true;
}
