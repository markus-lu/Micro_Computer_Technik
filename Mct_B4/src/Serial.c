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


void serial_init() {
	// GPIO Pins Initialisieren
    gpio_init_pin(&serial_clk);
    gpio_init_pin(&serial_dio);
    gpio_init_pin(&serial_stb);

    // Startwerte für GPIO-Pins setzen
    gpio_set_high(&serial_stb);
    gpio_set_high(&serial_clk);

    // timer 2 Initialisiern
    timer_init_timer2();
    timer_set_prescaler(LPC_TIM2, 1);
    timer_enable_match_interrupt(LPC_TIM2, 1, SERIAL_CLOCK);
    // Timer 2 Starten
    timer_start_timer(LPC_TIM2);
}

static void wait_for_interrupt() {
	// Warten bis timer 2 tickt
    while (!timer_has_timer2_ticked()) {
        __asm("nop");
    }
}

static void set_dio_pin_dir(enum PinDir dir) {
	// Kopie vom DIO-PIN machen weil die Konstante nicht verändert werden kann
	// Alles Kopiert außer die Richtung
    struct GPIOPin dio = {
            .dir = dir,
            .mode = serial_dio.mode,
            .pin = serial_dio.pin,
            .port = serial_dio.port,
            .open_drain = serial_dio.open_drain,
    };
    // Pin neu Initialisiern
    gpio_init_pin(&dio);
}


static void write_byte(uint8_t byte) {
	// Kommunikationsrichtung einstellen
    set_dio_pin_dir(OUTPUT);

    // In einer for schleife die einzelden Bits senden
    for (int i = 0; i < 8; ++i) {
    	// Setzen des Datenbits
        gpio_set(&serial_dio, (byte >> i) & 0b1);
        // Ein Tackt warten
        wait_for_interrupt();
        // Clock Pin Auf 0 ziehen
        gpio_set_low(&serial_clk);
        // Ein Tackt warten
        wait_for_interrupt();
        // Clock Pin Auf 1 ziehen
        gpio_set_high(&serial_clk);
        // Ein Tackt warten
        wait_for_interrupt();
    }
}

static uint8_t read_byte() {
    set_dio_pin_dir(INPUT);

    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        gpio_set_low(&serial_clk);
        wait_for_interrupt();
        uint8_t tmp = gpio_get(&serial_dio);
        byte |= (tmp << i);
        wait_for_interrupt();
        gpio_set_high(&serial_clk);
        wait_for_interrupt();
    }
    return byte;
}

void serial_write_command(uint8_t command) {
	//Strobeleitung muss zu begin der Übertragung auf 0 gezogen werden
    gpio_set_low(&serial_stb);

    // Zwei Tackte abwarten
    wait_for_interrupt();
    wait_for_interrupt();

    // Ein Byte zum Chip senden
    write_byte(command);

    // Zwei Tackte abwarten
    wait_for_interrupt();
    wait_for_interrupt();

    // Zum Beenden der Übertragung Strobeleitung wieder auf 1 ziehen
    gpio_set_high(&serial_stb);
}

void serial_write(uint8_t command, uint8_t *data, uint32_t length) {
    gpio_set_low(&serial_stb);

    wait_for_interrupt();
    wait_for_interrupt();

    write_byte(command);

    wait_for_interrupt();
    wait_for_interrupt();

    for (int i = 0; i < length; ++i) {
        write_byte(data[i]);
    }

    wait_for_interrupt();
    wait_for_interrupt();

    gpio_set_high(&serial_stb);
}

void serial_read(uint8_t command, uint8_t *data, uint32_t length) {
    // start transmission
    gpio_set_low(&serial_stb);

    wait_for_interrupt();
    wait_for_interrupt();

    write_byte(command);

    wait_for_interrupt();
    wait_for_interrupt();

    for (int i = 0; i < length; ++i) {
        data[i] = read_byte();
    }

    wait_for_interrupt();
    wait_for_interrupt();
    // end transmission
    gpio_set_high(&serial_stb);
}

void serial_deinit() {
    timer_deinit_timer2();
}

const struct GPIOPin serial_clk = {
        .port = 0,
        .pin = 10,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};

const struct GPIOPin serial_dio = {
        .port = 0,
        .pin = 11,
        .mode = PULL_DOWN,
        .dir = OUTPUT,
        .open_drain = false,
};

const struct GPIOPin serial_stb = {
        .port = 0,
        .pin = 5,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};
