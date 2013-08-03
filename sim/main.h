#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

enum {
	LED_WIDTH = 4,
	ZOOM_X = 150,
	ZOOM_Y = 150,
	BORDER = 60,
	FIRST_ON = 22
};

#define MINIDMX
#define SIMULATOR

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);

int sdlpause;

void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t t, uint16_t duration, uint8_t idle);
void setLedX(uint8_t x, uint8_t r,uint8_t g,uint8_t b);
void invLedX(uint8_t x);
void Delay(uint16_t t);
void getLedX(uint8_t x, uint8_t* red, uint8_t* green, uint8_t* blue);
#endif

