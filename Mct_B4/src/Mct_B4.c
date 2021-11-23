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
    state->last_buttons = 0;
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
        state->menu_should_redraw = true;
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
    Timer.start_timer(LPC_TIM1);
    struct Event event_data[EVENT_COUNT];
    Events.init(event_data);
    struct State state;
    init_state(&state, event_data);
    Menu.init();

    LEDKey.set_brightness(4, true);
    int counter = 0;
    struct DateTime test;

    test.seconds = 11;
    test.minutes = 22;
    test.hours = 11;
    test.weekday = 3;
    test.month = 11;
    test.year = 99;

    RTC.write_time(&test);

    while (true) {
        check_temperature_change(&state);
        check_time_change(&state);
        Menu.loop_once(&state);
        Clock.loop_once(&state);
        if (Timer.get_count(LPC_TIM1) % 500 == 0) {
            RTC.read_time(&test);
            printf("Seconds: %d\n", test.seconds);
            printf("Minuten: %d\n", test.minutes);
            printf("Stunden: %d\n", test.hours);
            printf("Wochentag: %d\n", test.weekday);
            printf("Tag: %d\n", test.day);
            printf("Monat: %d\n", test.month);
            printf("Jahr: %d\n", test.year);

            uint8_t digits[16];
            digits[0] = SEG_0;
            digits[1] = SEG_1;
            digits[2] = SEG_2;
            digits[3] = SEG_3;
            digits[4] = SEG_4;
            digits[5] = SEG_5;
            digits[6] = SEG_6;
            digits[7] = SEG_7;
            digits[8] = SEG_8;
            digits[9] = SEG_9;
            digits[10] = SEG_A;
            digits[11] = SEG_B;
            digits[12] = SEG_C;
            digits[13] = SEG_D;
            digits[14] = SEG_E;
            digits[15] = SEG_F;

            LEDKey.set_display_data(8, &digits[counter++]);
            counter %= 8;
        }
    }

    LEDKey.deinit();
    return 0;
}
