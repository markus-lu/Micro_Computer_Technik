/*
 ===============================================================================
 Name        : Mct_B4.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include "Mct_B4.h"
#include "Menu.h"
#include "Clock.h"
#include "LEDKey.h"

#ifdef __USE_CMSIS

#include "LPC17xx.h"
#include <types.h>

#endif

#include <stdio.h>
#include <cr_section_macros.h>
#include <Timer.h>

void delay(uint32_t ms) {
    uint32_t start_time = Timer.get_count(LPC_TIM1);
    while (Timer.get_count(LPC_TIM1) - start_time < ms) {

    }
}

void init_state(struct State *state, struct Event *event_data) {
    state->blink = false;
    state->screen = &MainMenu;
    state->menu_last_buttons = 0;
    state->selected_event = 0;
    state->selected_event_detail = 0;
    state->menu_edit_mode = false;
    state->event_data = event_data;
    state->menu_should_redraw = true;
}

void check_time_change(struct State *state) {
    struct DateTime new_time;
    RTC.read_time(&new_time);
    if (compare_times(&state->time, &new_time)) {
        state->time = new_time;
        state->clock_should_redraw = true;
    }
}

void check_temperature_change(struct State *state) {
    uint16_t new_temperature = RTC.read_temp();
    if (new_temperature != state->temperature) {
        state->temperature = new_temperature;
        state->menu_should_redraw = true;
    }
}

int main() {
    SystemCoreClockUpdate();
    printf("Hello World!!!\n");

    RTC.init();
    LEDKey.init();
    Timer.init_timer1();
    Timer.set_prescaler(LPC_TIM1, SystemCoreClock / 1000);
    Timer.enable_match_interrupt(LPC_TIM1, 1, 500);
    Timer.start_timer(LPC_TIM1);
    struct Event event_data[EVENT_COUNT];
    Events.init(event_data);
    struct State state;
    init_state(&state, event_data);
    Menu.init();

    LEDKey.set_brightness(MAX_BRIGHTNESS, true);

    while (true) {
        check_temperature_change(&state);
        check_time_change(&state);
        Menu.loop_once(&state);
        Clock.loop_once(&state);
        if (Timer.has_timer1_ticked()) {
            state.blink = !state.blink;
            state.menu_should_redraw = state.menu_edit_mode;
            state.clock_should_redraw = state.clock_edit_mode;
        }
    }

    LEDKey.deinit();
    return 0;
}
