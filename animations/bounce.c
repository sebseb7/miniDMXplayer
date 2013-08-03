#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t a = 0;

static uint8_t tick(void) {

	int step = a % LED_WIDTH;



	for(int x = 0; x < LED_WIDTH; x++) 
	{

		int red = 0;
		if (step==x)
			red=255;

		setLedX(x,red,0,0);
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
	registerAnimation(init,tick,deinit, 6, 100,1);
}


