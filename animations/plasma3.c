#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"
#include "libs/hsv2rgb.h"

static uint16_t a = 0;

static uint16_t h = 0;

static uint8_t tick(void) {

	
	uint8_t x;

	uint16_t sin1 = sini(a);
		
	uint16_t y_part =  sini(sin1)/65535.0f*180.0f;


	for(x = 0; x < LED_WIDTH; x++) 
	{
		h = (sini(x*580)/65535.0f*180.0f)+ y_part;


		struct hsv_colour hsv;
		struct rgb_colour rgb;

		hsv.h = h/360.0f;
		hsv.s = 1;
		hsv.v = 1;

		hsv2rgb( &hsv, &rgb );

		float norm = sqrtf(rgb.r*rgb.r + rgb.b*rgb.b + rgb.g*rgb.g);
		rgb.r /= norm;
		rgb.g /= norm;
		rgb.b /= norm;   

		setLedX(x,rgb.r*255,rgb.g*255,rgb.b*255);

	}
	a+=10;
	if(a==0x4000)
	{
		a=0;
	}
	return 0;
}

static void init(void)
{
	a = 0;
}
static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, 35, 1350,1);
}


