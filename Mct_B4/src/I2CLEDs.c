#include "I2CLEDs.h"

#define OUTPUT_P0 0b010
#define OUTPUT_P1 0b011
#define Config_P0 0b110
#define Config_P1 0b111
#define Dir_IN 0b1
#define Dir_OUT 0b0



/**
 *  \file    I2CLEDs.c
*/


/*********************************************************************/
 /**
  \brief Diese Funktion Initialisiert den I²C Portexpander für die LEDs.

 \param	-

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void i2cleds_init() {
	// Array zum Daten in I2C SChreiben
    uint8_t configuration[2];
    // Command Byte
    // Configuration Port 0
    configuration[0] = Config_P0;
    // Data Byte
    // Port 0 auf Ausgang konfigurieren
    configuration[1] = Dir_OUT;
    // Senden der Configuration
    i2c_write(PCA9539_ADDRESS, configuration, 2);

    // Command Byte
    // Configuration Port 1
    configuration[0] = Config_P1;
    // Data Byte
    // Port 1 auf Ausgang konfigurieren
    configuration[1] = Dir_OUT;
    // Senden der Configuration
    i2c_write(PCA9539_ADDRESS, configuration, 2);
}


/*********************************************************************/
 /**
  \brief Diese Funktion schaltet für die Anzahl der aktivierten Events die
 entsprechende Anzahl an LEDs über den I²C Portexpander ein.

 \param	event_count
 	 	Interger der die Anzahl der zurzeit aktivierten Events enthält
 	 	(0 - 15)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void i2cleds_set_leds(uint32_t event_count) {
    uint16_t leds = 0;

    for (int i = 0; i < 16 - event_count; ++i) {
        leds |= (1 << i);
    }

    uint8_t cmd[2];
    cmd[0] = OUTPUT_P0;
    cmd[1] = leds;
    i2c_write(PCA9539_ADDRESS, cmd, 2);

    cmd[0] = OUTPUT_P1;
    cmd[1] = (leds >> 8);
    i2c_write(PCA9539_ADDRESS, cmd, 2);
}
