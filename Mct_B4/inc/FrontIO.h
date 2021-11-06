/*
 ===============================================================================
 Name        : FrontIO.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Input Output Frontplatte
 ===============================================================================
 */

struct frontio {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct frontio FrontIO;
