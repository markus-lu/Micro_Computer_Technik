#include "GPIO.h"
#include "LPC17xx.h"

void gpio_set_pin_sel(const struct GPIOPin *pin) {
    volatile uint32_t *pinsel = &LPC_PINCON->PINSEL0;
    pinsel += pin->port * 2;

    if (pin->pin < 16) {
        *pinsel |= (MODE_GPIO << (pin->pin * 2));
    } else {
        *pinsel += 1;
        *pinsel |= (MODE_GPIO << ((pin->pin - 16) * 2));
    }
}

void gpio_set_pin_mode(const struct GPIOPin *pin) {
    volatile uint32_t *pinmode = &LPC_PINCON->PINMODE0;
    pinmode += pin->port * 2;

    if (pin->pin < 16) {
        *pinmode |= (pin->mode << (pin->pin * 2));
    } else {
        *pinmode += 1;
        *pinmode |= (pin->mode << ((pin->pin - 16) * 2));
    }
}

void gpio_set_pin_open_drain(const struct GPIOPin *pin) {
    volatile uint32_t *pinsel = &LPC_PINCON->PINMODE_OD0;
    pinsel += pin->port;

    *pinsel |= (pin->open_drain << (pin->pin));
}

static LPC_GPIO_TypeDef *get_gpio_port(uint8_t port) {
    LPC_GPIO_TypeDef *ptr = (LPC_GPIO_TypeDef *) LPC_GPIO_BASE;
    ptr += port;
    return ptr;
}


void gpio_init_pin(const struct GPIOPin *pin) {
    gpio_set_pin_sel(pin);
    gpio_set_pin_mode(pin);
    gpio_set_pin_open_drain(pin);

    volatile LPC_GPIO_TypeDef *gpio = get_gpio_port(pin->port);

    if (pin->dir == OUTPUT) {
        gpio->FIODIR |= (1 << pin->pin);
    } else {
        gpio->FIODIR &= ~(1 << pin->pin);
    }
    gpio->FIOMASK &= ~(1 << pin->pin);
}

void gpio_set_high(const struct GPIOPin *pin) {
    get_gpio_port(pin->port)->FIOSET = (1 << pin->pin);
}

void gpio_set_low(const struct GPIOPin *pin) {
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
