#include <Timer.h>
#include "Serial.h"

#define RISE_TIME 2                //   2 us
#define FALL_TIME 120              // 120 us
#define DOUBLE_CLOCK (1000 * 1000) //  1 MHz
#define CLOCK (500 * 1000)         // 500 kHz

#define MIN_CLOCK_PULSE_WIDTH   400 // ns
#define MIN_STROBE_PULSE_WIDTH 1000 // ns
#define MIN_DATA_SETUP_TIME     100 // ns
#define MIN_DATA_HOLD_TIME      100 // ns
#define MIN_CLK_STB_TIME       1000 // ns
#define MIN_WAIT_TIME          1000 // ns


static void init() {
    GPIO.init_pin(&Serial.clk);
    GPIO.init_pin(&Serial.dio);
    GPIO.init_pin(&Serial.stb);

    GPIO.set_high(&Serial.stb);
    GPIO.set_high(&Serial.clk);

    Timer.init_timer2();
    Timer.set_prescaler(LPC_TIM2, SystemCoreClock / DOUBLE_CLOCK);
}

static void write(uint8_t address, uint8_t *data, uint32_t length) {
    // TODO: Rausfinden, wie man Daten schreibt (Timer2?)
}

static void read(uint8_t address, uint8_t *data, uint32_t length) {
    // TODO: Rausfinden, wie man Daten liest (Timer2?)
}

static void deinit() {

}

const struct serial Serial = {
        .clk = {
                .port = 0,
                .pin = 10,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .dio = {
                .port = 0,
                .pin = 11,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .stb = {
                .port = 0,
                .pin = 5,
                .mode = NONE,
                .dir = OUTPUT,
                .open_drain = false,
        },
        .init = init,
        .write = write,
        .read = read,
        .deinit = deinit,
};
