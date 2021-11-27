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
#include "types.h"
#include "Menu.h"
#include "Events.h"
#include "I2CLEDs.h"
#include "Clock.h"
#include "LEDKey.h"
#include "Timer.h"
#include "RGBLED.h"
#include <stdio.h>

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

void init_state(struct State *state, struct Event *event_data) {
    state->blink = false;
    state->menu_screen = SCREEN_MAIN_MENU;
    state->menu_last_buttons = 0;
    state->selected_event = 0;
    state->selected_event_detail = 0;
    state->menu_edit_mode = false;
    state->event_data = event_data;
    state->menu_should_redraw = true;
    state->rgb_state = false;
    state->temperature = 0;
//    state->time;                        // wird an anderer Stelle initialisiert
    state->clock_selected_field = 0;
    state->clock_edit_mode = false;
    state->clock_should_redraw = true;
    state->clock_show_time = true;
    state->clock_last_buttons = 0;
    state->clock_brightness = 0;
}

void check_time_change(struct State *state) {
    struct DateTime new_time;
    rtc_read_time(&new_time);
    if (compare_times(&state->time, &new_time)) {
        if (state->time.minutes != new_time.minutes) {
            for (int i = 0; i < EVENT_COUNT; ++i) {
                struct Event *event = &state->event_data[i];
                if (event->enabled &&
                    event->hour == new_time.hours &&
                    event->minute == new_time.minutes &&
                    ((event->weekdays & (1 << new_time.weekday)) != 0)) {
                    state->rgb_state = event->on_or_off;
                    if (event->on_or_off) {
                        rgbled_set_green();
                    } else {
                        rgbled_set_red();
                    }
                }
            }
        }
        state->time = new_time;
        state->clock_should_redraw = true;
    }
}

void check_temperature_change(struct State *state) {
    uint16_t new_temperature = rtc_read_temp();
    if (new_temperature != state->temperature) {
        state->temperature = new_temperature;
        state->menu_should_redraw = true;
    }
}

int main() {
    SystemCoreClockUpdate();
    printf("Hello World!!!\n");

    rtc_init();
    rgbled_init();
    ledkey_init();
    timer_init_timer1();
    timer_set_prescaler(LPC_TIM1, SystemCoreClock / 1000);
    timer_enable_match_interrupt(LPC_TIM1, 1, 500);
    timer_start_timer(LPC_TIM1);
    struct Event event_data[EVENT_COUNT];
    events_init(event_data);
    struct State state;
    init_state(&state, event_data);
    menu_init();
    clock_init(&state);
    i2cleds_init();

    while (true) {
        if (timer_has_timer1_ticked()) {
            state.blink = !state.blink;
            menu_update_menu(&state);
            if (state.clock_edit_mode) {
                state.clock_should_redraw = true;
            }
        }
        check_temperature_change(&state);
        if (!state.clock_edit_mode) {
            check_time_change(&state);
        }
        menu_loop_once(&state);
        clock_loop_once(&state);
        i2cleds_set_leds(events_get_count(event_data));
    }
    timer_deinit_timer1();
    ledkey_deinit();
    i2c_exit();
    return 0;
}
