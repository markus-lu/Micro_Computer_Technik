//=============================================================================
// lcdlib_armcm3.h
// Low Level Treiber für das LC Display
//=============================================================================
// vorherige Rev. von R. Ebel
// Version 2.3 vom 12.07.2012, U.Schoon und A.Busboom
//
//=============================================================================
// Version 2.4 25.05.2015, R.Ebel und T.Hobbie
#ifndef LCDLIB_ARMCM3_H_
#define LCDLIB_ARMCM3_H_

//
// LCD-Bibliothek zur Ansteuerung von Displays mit 1, 2 oder 4 Zeilen   ,
// im 4-Bit-Modus
//
//   --------------------------------------------------------------------------
//   void lcd_init         (uint8_t LineNo);
//
//        Initialisierung des LC-Displays
//
//        LineNo gibt die Anzahl der Zeilen des verwendeten Displays an.
//        Werte : [1|2|4]
//   --------------------------------------------------------------------------
//   void lcd_clrscr       (void);
//
//        LCD-Bildschirm löschen
//   --------------------------------------------------------------------------
//   void lcd_write_char   (UCHAR ch);
//
//        Zeichen an aktueller Cursorposition auf das LCD schreiben,
//        incl. Zeichensatzkorrektur bei Umlauten / Ligatur
//   --------------------------------------------------------------------------
//   void lcd_write_string (UCHAR *st);
//
//        String an aktueller Cursorposition auf das LCD schreiben
//   --------------------------------------------------------------------------
//   void lcd_write_string_xy (UCHAR *st, UCHAR x, UCHAR y);
//
//        String an Cursorposition (x,y) auf das LCD schreiben
//   --------------------------------------------------------------------------
//   void lcd_write_uint   (UINT num, UCHAR len);
//
//        UINT-Variable an aktueller Cursorposition auf das LCD schreiben,
//        wobei "len" die Länge angibt ( zulässig : len = [ 1..5 ] )
//   --------------------------------------------------------------------------
//   void lcd_write_int    (INT num, UCHAR len);
//
//        Signed Int-Variable an aktueller Cursorposition auf das LCD schreiben,
//        wobei "len" die Länge angibt ( zulässig : len = [ 1..6 ] )
//   --------------------------------------------------------------------------
//   void lcd_gotoxy       (UCHAR x, UCHAR y);
//
//        Direkte Cursor-Positionierung in X- und Y-Richtung.
//        Das hier verwendete Display hat 4 Zeilen á 20 Zeichen, somit wird mit
//        "gotoxy (20,4)" die rechte, untere Ecke adressiert
//   --------------------------------------------------------------------------
//   void lcd_cursor       (UCHAR cursor_mode);
//
//        LCD-Cursormodus setzen, möglich sind C_OFF, C_BLINK, C_UNDERLINE,
//        z.B.: lcd_cursor (C_OFF)                  Cursor aus
//              lcd_cursor (C_BLINK)                Cursor blinkt (Block)
//              lcd_cursor (C_BLINK | C_UNDERLINE)  Cursor blinkt (Underline)
//=============================================================================

#ifndef _LCDLIB_ARMCM3
#define _LCDLIB_ARMCM3


#define PWM_PERIODENDAUER 	100	// Periodendauer aller PWM Kanäle (MR0)
#undef  USE_PWM

#define LCD_DELAY 4000  //7500

//=== Konfigurationsteil (nur hier sind Änderungen erforderlich): =============

#define _CONNECT_4BIT_        // Art des Anschlusses: 4 Bit

#undef  _DIP204_6             // EA DIP404-4NLED 4*20-Display DIL
#undef	_DIP204_4
#undef  _L2014                // Seiko 4*20-Display L2014
#define  _2004A
#undef _2X24NORM             // Standard-HD44780-Display mit 2*24 Zeichen

#define _USE_SPRINTF_         // aktivieren, falls sprintf genutzt werden soll
//---------- ARM Cortex M3-Testboard: ---------------------------------------------

#define I2C_PCF8574_LCD_ADDR 0x4e

#define LCD_E              4  // Steuerleitung E  des LCD-Interfaces
#define LCD_RS             1  // Steuerleitung RS des LCD-Interfaces
#define LCD_RW             2  // Steuerleitung R/_W des LCD-Interfaces
#define LCD_LIGHT	       8  // LCD-Hintergrundbeleuchtung
#define DATA_OFFSET        4  // Anzahl Offsetbits ab D0
                              // z.B.: Daten von PB2..PB5 : --> D_O = 2
#define _I2C
#undef  _EXTFUNCSET			  // Erweiterter Zeichsatz (Sondersymbole)
//=== Ende Konfigurationsteil =================================================



//=== Auto-Konfigurationsteil (nicht ändern !) ================================

//#define LCD_RW    "_undef0_"  // Steuerleitung RW des LCD-Interfaces
#define LCD_BUSY  "_undef1_"  // Bit 7 des Datenports

//=== Display-Konfigurationen: ===

#ifdef _DIP204_6
#define _CHIP_KS0073          // verwendeter Chipsatz
#undef  _CHIP_HD44780         // nicht verwendeter Chipsatz
#define ADDR_LINE1      0x80  // Startadresse +0x80 DDRAM Zeile 1
#define ADDR_LINE2      0xa0  // Startadresse +0x80 DDRAM Zeile 2
#define ADDR_LINE3      0xc0  // Startadresse +0x80 DDRAM Zeile 3
#define ADDR_LINE4      0xe0  // Startadresse +0x80 DDRAM Zeile 4
#endif

#ifdef _DIP204_4
#define _CHIP_KS0073          // verwendeter Chipsatz
#undef  _CHIP_HD44780         // nicht verwendeter Chipsatz
#define ADDR_LINE1      0x80  // Startadresse DDRAM Zeile 1
#define ADDR_LINE2      0xa0  // Startadresse DDRAM Zeile 2
#define ADDR_LINE3      0xc0  // Startadresse DDRAM Zeile 3
#define ADDR_LINE4      0xe0  // Startadresse DDRAM Zeile 4
#endif

#ifdef _L2014
#undef  _CHIP_KS0073          // nicht verwendeter Chipsatz
#define _CHIP_HD44780         // verwendeter Chipsatz
#define ADDR_LINE1      0x80  // Startadresse DDRAM Zeile 1
#define ADDR_LINE2      0xC0  // Startadresse DDRAM Zeile 2
#define ADDR_LINE3      0x94  // Startadresse DDRAM Zeile 3
#define ADDR_LINE4      0xD4  // Startadresse DDRAM Zeile 4
#endif

#ifdef _2004A
#undef  _CHIP_KS0073          // nicht verwendeter Chipsatz
#define _CHIP_HD44780         // verwendeter Chipsatz
#define ADDR_LINE1      0x80  // Startadresse DDRAM Zeile 1
#define ADDR_LINE2      0xC0  // Startadresse DDRAM Zeile 2
#define ADDR_LINE3      0x94  // Startadresse DDRAM Zeile 3
#define ADDR_LINE4      0xD4  // Startadresse DDRAM Zeile 4
#endif

#ifdef _2X24NORM
#undef  _CHIP_KS0073          // nicht verwendeter Chipsatz
#define _CHIP_HD44780         // verwendeter Chipsatz
#define ADDR_LINE1      0x80  // Startadresse DDRAM Zeile 1
#define ADDR_LINE2      0xC0  // Startadresse DDRAM Zeile 2
#define ADDR_LINE3      0x80  // - nur 2 Zeilen erlaubt !
#define ADDR_LINE4      0x80  // - nur 2 Zeilen erlaubt !
#endif

//=== Ende Auto-Konfigurationsteil ============================================

//--- Typen: ------------------------------------------------------------------

#include "types.h"

//--- Konstanten: -------------------------------------------------------------

#define SOLID 			   0x10
#define BLINK 			   0x50
#define OFF 			   0x00

#define C_OFF              0  // Cursor aus
#define C_BLINK            1  // Cursor blinkt
#define C_UNDERLINE        2  // Cursor "underline" statt Block

#define nop()              __asm volatile ("nop")

//--- Prototypen: -------------------------------------------------------------

void lcd_init(uint8_t LineNo, uint8_t speed);
void lcd_clrscr(void);
void lcd_write_char (uchar_t ch);
void lcd_write_string (char_t *st);
void lcd_write_4lines (char_t *line1 ,char_t *line2 ,char_t *line3 ,char_t *line4);
void lcd_write_string_xy (char_t *st, uchar_t x, uchar_t y);
void lcd_write_uint (uint16_t num, uint8_t len);
void lcd_write_int (int16_t num, uint8_t len);
void lcd_gotoxy (uint8_t x, uint8_t y);
void lcd_cursor (uint8_t cursor_mode);
void lcd_bglight(uint8_t brightness);
void lcd_write_symbols(uint8_t segram_addr, uint8_t state);
void I2C_lcd_write(uchar_t data);

#endif   // _LCDLIB_ARMCM3
#endif   // _LCDLIB_ARMCM3
/******************************************************************************
**                            End Of File
******************************************************************************/
