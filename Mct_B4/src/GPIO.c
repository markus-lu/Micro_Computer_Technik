#include "GPIO.h"
#include "LPC17xx.h"

void gpio_set_pin_sel(const struct GPIOPin *pin) {
	// Adresse vom PINSEL-Register für Port 0
	volatile uint32_t *pinsel = &LPC_PINCON->PINSEL0;
    // Um auf PINSEL2 zu kommen plus port mal 2
	pinsel += pin->port * 2;

	// wenn Untere hälfte benutzt wir (pin 0 - 15)
    if (pin->pin < 16) {
    	// GPIO Modus an die Bits für den Pin schreiben
        *pinsel |= (MODE_GPIO << (pin->pin * 2));
    } else {
    	// wenn 16 - 31
    	// ein Pinselregister weiterspringen
        *pinsel += 1;
    	// GPIO Modus an die Bits für den Pin schreiben
        *pinsel |= (MODE_GPIO << ((pin->pin - 16) * 2));
    }
}

void gpio_set_pin_mode(const struct GPIOPin *pin) {
	// Rausuchen des Richtigen Pinmodus
	volatile uint32_t *pinmode = &LPC_PINCON->PINMODE0;
	// Um auf PINMODE2 zu kommen plus port mal 2
    pinmode += pin->port * 2;

    // wenn Untere hälfte benutzt wir (pin 0 - 15)
    if (pin->pin < 16) {
    	// GPIO Modus an die Bits für den Pin schreiben
    	// Mögliche Pull-Ups oder Pull-Downs defenieren
        *pinmode |= (pin->mode << (pin->pin * 2));
    } else {
    	// wenn 16 - 31
    	// ein Pinmoderegister weiterspringen
        *pinmode += 1;
        // GPIO Modus an die Bits für den Pin schreiben
        // Mögliche Pull-Ups oder Pull-Downs defenieren
        *pinmode |= (pin->mode << ((pin->pin - 16) * 2));
    }
}

void gpio_set_pin_open_drain(const struct GPIOPin *pin) {
	// Rausuchen des Pinmode für Port 0
    volatile uint32_t *pinmode_od = &LPC_PINCON->PINMODE_OD0;
    // Um zum Richtigen Port zu kommen Port-offset hinzuaddieren
    pinmode_od += pin->port;

    // Opendrain Pin Setzen oder nicht setzetn
    *pinmode_od |= (pin->open_drain << (pin->pin));
}

static LPC_GPIO_TypeDef *get_gpio_port(uint8_t port) {
	// GPIO Basisadresse holen
	LPC_GPIO_TypeDef *ptr = (LPC_GPIO_TypeDef *) LPC_GPIO_BASE;
	// Addition des Ports um zum richtigen GPIO Struckt zu Springen
    ptr += port;
    // Rückgabe der Portadresse
    return ptr;
}


void gpio_init_pin(const struct GPIOPin *pin) {
	// Initialisierung GPIO
	gpio_set_pin_sel(pin);
    gpio_set_pin_mode(pin);
    gpio_set_pin_open_drain(pin);

    //Rausuchen der Richtigen GPIO Adresse für den Port
    volatile LPC_GPIO_TypeDef *gpio = get_gpio_port(pin->port);

    // wenn Ausgang
    if (pin->dir == OUTPUT) {
    	// FIODIR-Bit Setzen
        gpio->FIODIR |= (1 << pin->pin);
    } else {
    	// FIODIR-Bit Löschen (Eingang)
        gpio->FIODIR &= ~(1 << pin->pin);
    }
    // Pin in GPIO-Maske freigeben
    gpio->FIOMASK &= ~(1 << pin->pin);
}

void gpio_set_high(const struct GPIOPin *pin) {
	// Pin mit dem Register FIOSET einschalten
    get_gpio_port(pin->port)->FIOSET = (1 << pin->pin);
}

void gpio_set_low(const struct GPIOPin *pin) {
	// Pin mit dem Register FIOCLR ausschalten
    get_gpio_port(pin->port)->FIOCLR = (1 << pin->pin);
}

void gpio_set(const struct GPIOPin *pin, bool state) {
    if (state) {
        gpio_set_high(pin);
    } else {
        gpio_set_low(pin);
    }
}

bool gpio_get(const struct GPIOPin *pin) {
    volatile uint32_t fiopin = get_gpio_port(pin->port)->FIOPIN;
    uint32_t masked_pin = fiopin & (1 << pin->pin);
    return masked_pin != 0;
}
