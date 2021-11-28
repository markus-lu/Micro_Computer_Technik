#include "I2C.h"

/**
 *  \file     I2C.c
*/

/**
 *  \page I2C
 *
 *  \section sec1 Unterprogramme
 *
 *  \subsection sec1_1 i2c_write
 *  Diese Funktion bietet eine Abstraktion über die Bibliotheksfunktion i2c_write_byte,
 *  welche es erlaubt ein Array an Bytes mit einem Funktionsaufruf zu schreiben.
 *
 *  \subsection sec1_1 i2c_read
 *  Diese Funktion bietet eine Abstraktion über die Bibliotheksfunktion i2c_read_byte,
 *  welche es erlaubt ein Array an Bytes mit einem Funktionsaufruf zu lesen.
 *
 *  <HR>
 *
 *   \author	Markus Luckau, Simon Struck
 *   \date		25.10.2021
 *   \version 	1.0             Fertige Version
 */




/*********************************************************************/
/**
Diese Funktion bietet eine Abstraktion über die Bibliotheksfunktion i2c_write_byte,
welche es erlaubt ein Array an Bytes mit einem Funktionsaufruf zu schreiben.

\param   address
         I2C Adresse, an die geschrieben werden soll
         ()
\param   data

\param   length

\return  -

\version 07.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint8_t i2c_write(uint8_t address, uint8_t *data, uint32_t length) {
    i2c_start();
    uint8_t status = i2c_write_byte(address | I2C_WRITE);
    if (status == ACK) {
        for (uint32_t i = 0; i < length; ++i) {
            // discard result, it's also not checked in the lcd library
            i2c_write_byte(data[i]);
        }
    }
    i2c_stop();
    return status;
}

/*********************************************************************/
/**
Diese Funktion bietet eine Abstraktion über die Bibliotheksfunktion i2c_read_byte,
welche es erlaubt ein Array an Bytes mit einem Funktionsaufruf zu lesen.

 \param   address
         I2C Adresse, an die geschrieben werden soll
         ()
\param   data

\param   length

\return  -

\version 07.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint8_t i2c_read(uint8_t address, uint8_t *data, uint32_t length) {
    i2c_start();
    uint8_t status = i2c_write_byte(address | I2C_READ);
    if (status == ACK) {
        for (uint32_t i = 0; i < length; ++i) {
            // discard result, it's also not checked in the lcd library
            data[i] = i2c_read_byte(i < (length - 1) ? ACK : NACK);
        }
    }
    i2c_stop();
    return status;
}
