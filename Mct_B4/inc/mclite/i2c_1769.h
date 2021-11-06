/***************************************************************
 * Autor: 	R. Ebel / Tim Hobbie
 * Datei:	i2c_1769.h
 * Version: 1.0
 * Beschreibung: Funktionen zur Kommunikation auf dem I²C Bus
 * (Initialisierung, Beendigung, Start-/Stopbedingung,
 * Senden/Empfangen eines Bytes)
****************************************************************/

#ifndef I2C_1769
#define I2C_1769

#include "LPC17xx.h"

/* Defines für die verschiedenen Geschwindigkeitsmodi */
#define I2C_SM	   0
#define I2C_FM	   1
#define I2C_FMPLUS 2

/* Bit-Defines zur besseren Lesbarkeit */
#define ACK	  	  0 // ACK auf Bus ist immer Low Pegel
#define NACK 	  1 // NACK auf Bus ist immer High Pegel
#define I2C_READ  1 // Lesebedingung
#define I2C_WRITE 0 // Schreibbedingung

void i2c_init(uint8_t i2c_speed);	// Initialisiert die I²C Kommunikation
void i2c_exit(void);	// Beendet die I²C Kommunikation

void i2c_start(void);	// Sendet die Startbedingung auf den Bus
void i2c_stop (void);	// Sendet die Stopbedingung auf den Bus

uint8_t i2c_write_byte(uint8_t byte); 	// Schreibt ein Byte auf den Bus
uint8_t i2c_read_byte (uint8_t ack);	// Liest ein Byte vom Bus

#endif 
