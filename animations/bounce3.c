#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t a = 0;

static const uint8_t item[] = {250,127,0};

static uint8_t tick(void) {

	
	float x0 = (float)sini(a*60)/(0xffff/(LED_WIDTH+1))-1;
	float x1 = (float)sini(a*45)/(0xffff/(LED_WIDTH+1))-1;
	float x2 = (float)sini(a*150)/(0xffff/(LED_WIDTH+1))-1;



	for(int x = 0; x < LED_WIDTH; x++) 
	{

		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;

		float diff0 = fabsf(x0-x);
		float diff1 = fabsf(x1-x);
		float diff2 = fabsf(x2-x);
	

		if(diff0 < 1.5f)
		{
			//red = (1.5f-diff0)*170;
		}
		if(diff1 < 1.5f)
		{
			//green = (1.5f-diff1)*170;
		}
		if(diff2 < 2)
		{
			blue= (2-diff2)*127;
		}

		setLedX(
			x,
			red,
			green,
			blue
		);
	}
	a+=1;
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
	registerAnimation(init,tick,deinit, 50, 1500,1);
}


