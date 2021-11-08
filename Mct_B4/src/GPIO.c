#include "GPIO.h"

static void init() {
}


static void set_pin_sel(const struct GPIOPin *pin) {
    volatile uint32_t *pinsel = &LPC_PINCON->PINSEL0;
    pinsel += pin->port * 2;

    if (pin->pin < 16) {
        *pinsel |= (MODE_GPIO << (pin->pin * 2));
    } else {
        *pinsel += 1;
        *pinsel |= (MODE_GPIO << ((pin->pin - 16) * 2));
    }
}

static void set_pin_mode(const struct GPIOPin *pin) {
    volatile uint32_t *pinsel = &LPC_PINCON->PINMODE0;
    pinsel += pin->port * 2;

    if (pin->pin < 16) {
        *pinsel |= (pin->mode << (pin->pin * 2));
    } else {
        *pinsel += 1;
        *pinsel |= (pin->mode << ((pin->pin - 16) * 2));
    }
}

static void set_pin_open_drain(const struct GPIOPin *pin) {
    volatile uint32_t *pinsel = &LPC_PINCON->PINMODE_OD0;
    pinsel += pin->port;

    *pinsel |= (pin->open_drain << (pin->pin));
}

static LPC_GPIO_TypeDef *port_to_gpio_address(uint8_t port) {
    LPC_GPIO_TypeDef *ptr = LPC_GPIO_BASE;
    ptr += port;
    return ptr;
}


static void init_pin(const struct GPIOPin *pin) {
    set_pin_sel(pin);
    set_pin_mode(pin);
    set_pin_open_drain(pin);

    volatile LPC_GPIO_TypeDef *gpio = port_to_gpio_address(pin->port);

    if (pin->dir == OUTPUT) {
        gpio->FIODIR |= (1 << pin->pin);
    } else {
        gpio->FIODIR &= ~(1 << pin->pin);
    }
    gpio->FIOMASK &= ~(1 << pin->pin);
}

static void set(const struct GPIOPin *pin) {
    volatile LPC_GPIO_TypeDef *gpio = port_to_gpio_address(pin->port);

    gpio->FIOSET = (1 << pin->pin);
}

static void clear(const struct GPIOPin *pin) {
    volatile LPC_GPIO_TypeDef *gpio = port_to_gpio_address(pin->port);

    gpio->FIOCLR = (1 << pin->pin);
}

static bool get(const struct GPIOPin *pin) {
    volatile LPC_GPIO_TypeDef *gpio = port_to_gpio_address(pin->port);

    return (gpio->FIOPIN & (1 << pin->pin)) != 0;
}

const struct gpio GPIO = {
        .init = init,
        .init_pin = init_pin,
        .set = set,
        .clear = clear,
        .get = get,
};
