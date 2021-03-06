#include "I2C.h"

/**
 *  \file     I2C.c
*/

/*********************************************************************/
/**
 \brief Diese Funktion bietet eine Abstraktion über die Bibliotheksfunktion i2c_write_byte,
welche es erlaubt ein Array an Bytes mit einem Funktionsaufruf zu schreiben.

\param  address
        I2C Adresse, an die geschrieben werden soll
        (0x00 - 0x7F)
\param  data
        Ein Pointer, von dem length Bytes geschrieben werden sollen
\param  length
        Anzahl an Bytes, die geschrieben werden sollen

\return status Status des Slaves
        (ACK, wenn Slave geantwortet hat, NACK wenn nicht)

\version 29.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint8_t i2c_write(uint8_t address, uint8_t *data, uint32_t length) {
	// Startbedingungen auf den I2C Bus legen
    i2c_start();
    // Adressbyte zusammen mit dem Wunsch zu schreiben senden
    uint8_t status = i2c_write_byte((address << 1) | I2C_WRITE);
    // Bei einer ACK Antwort
    if (status == ACK) {
    	// Schleife zum Seriellen Senden
        for (uint32_t i = 0; i < length; ++i) {
            // Byte senden
            i2c_write_byte(data[i]);
        }
    }
    // Stopbedingungen auf den I2C Bus legen
    i2c_stop();
    // Rückgabe ob Daten Empfangen wurden
    return status;
}

/*********************************************************************/
/**
 \brief Diese Funktion bietet eine Abstraktion über die Bibliotheksfunktion i2c_read_byte,
welche es erlaubt ein Array an Bytes mit einem Funktionsaufruf zu lesen.

\param  address
        I2C Adresse, von der gelesen werden soll
        (0x00 - 0x7F)
\param  data
        Ein Pointer, ein den length Bytes eingelesen werden sollen
\param  length
        Anzahl an Bytes, die gelesen werden sollen

\return status Status des Slaves
        (ACK, wenn Slave geantwortet hat, NACK wenn nicht)

\version 29.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint8_t i2c_read(uint8_t address, uint8_t *data, uint32_t length) {
	// Startbedingungen auf den I2C Bus legen
    i2c_start();
    // Adressbyte zusammen mit dem Wunsch zu lesen senden
    uint8_t status = i2c_write_byte((address << 1) | I2C_READ);
    // Bei einer ACK Antwort
    if (status == ACK) {
    	// Schleife zum seriellen Empfangen
        for (uint32_t i = 0; i < length; ++i) {
            // beim lesen des letzen Byte sende ein NACK
        	// ansonsten ein Ack um den Empfang zu bestätigen
            data[i] = i2c_read_byte(i < (length - 1) ? ACK : NACK);
        }
    }
    // Stopbedingungen auf den I2C Bus legen
    i2c_stop();
    // Rückgabe ob Daten Empfangen wurden
    return status;
}
