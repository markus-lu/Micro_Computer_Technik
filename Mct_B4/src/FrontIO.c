#include "FrontIO.h"
#include "I2C.h"

#define LED0 (1 << 0)
#define LED1 (1 << 1)
#define LED2 (1 << 2)
#define LED3 (1 << 3)
#define TA0 (1 << 4)
#define TA1 (1 << 5)
#define TA2 (1 << 6)
#define TA3 (1 << 7)

static void init() {
    GPIO.init_pin(&FrontIO.interrupt);



// TODO: Einmal I2C Lesen, damit er einen Interrupt beim Knopfdrücken auslöst wird
// TODO: Interrupt auf Pin 2.13 aktivieren
}

static void handle_interrupt() {
    // TODO: Taster abfragen, um zu sehen welcher Taster gedrückt wurde
    // TODO: LED's aktualisieren
    // TODO: Gedrückten Taster an Menu weiter geben, damit dort auf den Tastendruck reagiert werden kann
}

const struct frontio FrontIO = {
        .interrupt = {
                .port = 2,
                .pin = 13,
                .mode = PULL_UP,
                .dir = INPUT,
                .open_drain = false,
        },
        .init = init,
        .handle_interrupt = handle_interrupt
};
