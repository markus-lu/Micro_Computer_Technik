#include <Timer.h>
#include "Serial.h"
#include "GPIO.h"

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
    Timer.enable_match_interrupt(LPC_TIM2, 1, 10);
    Timer.start_timer(LPC_TIM2);
}

static void wait_for_interrupt(){
	while(!Timer.has_ticked()){
		__asm("nop");
	}
}

static void write_byte(uint8_t byte){
	struct GPIOPin dio = {
			.dir = OUTPUT,
			.mode = Serial.dio.mode,
			.pin = Serial.dio.pin,
			.port = Serial.dio.port,
			.open_drain = Serial.dio.open_drain,
	};
	GPIO.init_pin(&dio);
	for (int i = 0; i < 8; ++i) {
		GPIO.set(&Serial.dio, ((byte >> i) & 0b1));
	    wait_for_interrupt();
	    GPIO.set_low(&Serial.clk);
	    wait_for_interrupt();
	    GPIO.set_high(&Serial.clk);
	    wait_for_interrupt();
	}
}

static void write(uint8_t *data, uint32_t length) {
    GPIO.set_low(&Serial.stb);
    wait_for_interrupt();
    wait_for_interrupt();
    for (int i = 0; i < length; ++i) {
    	write_byte(data[i]);
	}

	wait_for_interrupt();
	wait_for_interrupt();
	GPIO.set_high(&Serial.stb);
}

static uint8_t read_byte(){
	uint8_t byte = 0;
	struct GPIOPin dio = {
			.dir = INPUT,
			.mode = Serial.dio.mode,
			.pin = Serial.dio.pin,
			.port = Serial.dio.port,
			.open_drain = Serial.dio.open_drain,
	};
	GPIO.init_pin(&Serial.dio);
	for (int i = 0; i < 8; i++) {
	    GPIO.set_low(&Serial.clk);
	    wait_for_interrupt();
	    byte |= (GPIO.get(&Serial.dio) << i);
	    wait_for_interrupt();
	    GPIO.set_high(&Serial.clk);
	    wait_for_interrupt();
	}
	return byte;
}

static void read(uint8_t *data, uint32_t length) {
    GPIO.set_low(&Serial.stb);
    wait_for_interrupt();
    wait_for_interrupt();
    for (int i = 0; i < length; ++i) {
    	read_byte(data[i]);
	}

	wait_for_interrupt();
	wait_for_interrupt();
	GPIO.set_high(&Serial.stb);
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
