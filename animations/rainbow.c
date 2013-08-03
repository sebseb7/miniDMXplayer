#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"
#include "libs/hsv2rgb.h"

static uint16_t h = 0;

static uint8_t tick(void) {

		int h2 = h;
	for(int x=0;x<LED_WIDTH;x++)
	{
	h+=40;
	if(h>=360)
		h=0;

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
	h=h2;
	h+=1;
	if(h>=360)
		h=0;

	return 0;
}

static void init(void)
{
}
static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, 10, 4000,1);
}




