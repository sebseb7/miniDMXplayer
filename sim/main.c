#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"
#include <string.h>
#include<sys/time.h>

#include <termios.h>
#include <fcntl.h>
#include <errno.h>
//#if defined(MAC_OS_X_VERSION_10_4) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
//#endif

static int serial_bridge;

void init_serial() {

	serial_bridge = open("/dev/cu.usbserial-A602SRIU", O_RDWR);

	struct termios config;
	memset(&config, 0, sizeof(config));
	tcgetattr(serial_bridge, &config);

	config.c_iflag = 0;
	config.c_oflag = 0;
	config.c_lflag = 0;
	config.c_cc[VMIN] = 1;
	config.c_cc[VTIME] = 5;

	cfsetospeed(&config, B115200);
	cfsetispeed(&config, B115200);
	config.c_cflag = CS8 | CREAD | CLOCAL | CSTOPB;
	tcsetattr(serial_bridge, TCSANOW, &config);



    speed_t speed = 9600;
    if ( ioctl( serial_bridge,  IOSSIOSPEED, &speed ) == -1 )
    {
        printf( "Error %d calling ioctl( ..., IOSSIOSPEED, ... )\n", errno );
    }

}



int sdlpause = 0;

#define MAX_ANIMATIONS 200
#define MAX_APPS 200

int animationcount = 0;
int appcount = 0;

struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	int duration;
	uint32_t timing;
	uint32_t idle;
} animations[MAX_ANIMATIONS];


struct app {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	int min_delay;
} apps[MAX_APPS];


SDL_Surface* screen;
void Delay(uint16_t t)
{
	
}

int leds[LED_WIDTH][4];
void setLedX(uint8_t x, uint8_t red,uint8_t green,uint8_t blue) {
	if (x >= LED_WIDTH) return;
	leds[x][0] = red;
	leds[x][1] = green;
	leds[x][2] = blue;
	leds[x][3] = 1;
}

void invLedX(uint8_t x) {
	if (x >= LED_WIDTH) return;
	leds[x][0] = 255 - leds[x][0];
	leds[x][1] = 255 - leds[x][1];
	leds[x][2] = 255 - leds[x][2];
	leds[x][3] = 1;
}
void getLedX(uint8_t x, uint8_t* red, uint8_t* green, uint8_t* blue) {
	if (x >= LED_WIDTH) return;
	*red = leds[x][0];
	*green = leds[x][1];
	*blue = leds[x][2];
}

void registerAnimation(init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count, uint8_t idle)
{
	if(animationcount == MAX_ANIMATIONS)
		return;
	animations[animationcount].init_fp = init;
	animations[animationcount].tick_fp = tick;
	animations[animationcount].deinit_fp = deinit;
	animations[animationcount].duration = count;
	animations[animationcount].idle = idle;
	animations[animationcount].timing = 1000/t;

	animationcount++;

}


void fillRGB(uint8_t r,uint8_t g, uint8_t b)
{
	int x;

	for(x = 0; x < LED_WIDTH; x++) {
		leds[x][0]=r;
		leds[x][1]=g;
		leds[x][2]=b;
		leds[x][3]=1;
	}
}


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

	fillRGB(0,0,0);

	srand(time(NULL));

	init_serial();
	char l  = 0;

	int current_animation = 0;

	screen = SDL_SetVideoMode(LED_WIDTH*ZOOM_X+((LED_WIDTH+1)*BORDER),ZOOM_Y+BORDER+BORDER,32, SDL_SWSURFACE | SDL_DOUBLEBUF);


	animations[current_animation].init_fp();
	
	int tick_count = 0;
	int running = 1;
	//unsigned long long int startTime = get_clock();
	Uint32 lastFrame = SDL_GetTicks(); 

	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							if(sdlpause == 0)
							{
								sdlpause = 1;
							}
							else{
								sdlpause = 0;
							}
							break;
						default: break;
					}
				default: break;
			}
		}

		animations[current_animation].tick_fp();


	


		
		int x;
		for(x = 0; x < LED_WIDTH; x++) {

			if(leds[x][3] == 1)
			{
				SDL_Rect rect = { (ZOOM_X+BORDER)*x+(ZOOM_X/15)+BORDER, BORDER, ZOOM_X-(ZOOM_X/15), ZOOM_Y};
				SDL_FillRect(
					screen, 
					&rect, 
					SDL_MapRGB(screen->format, leds[x][0],leds[x][1],leds[x][2])
				);

				leds[x][3] = 0;

			}

		}

		
		SDL_Flip(screen);

		l++;	
		usleep(5000);
		speed_t speed = 38400;
		if ( ioctl( serial_bridge,  IOSSIOSPEED, &speed ) == -1 )
		{
			printf( "Error %d calling ioctl( ..., IOSSIOSPEED, ... )\n", errno );
		}
		usleep(1200);

		unsigned char c=0;
		write(serial_bridge,&c,1);
		usleep(200);
		
		speed = 250000;
		if ( ioctl( serial_bridge,  IOSSIOSPEED, &speed ) == -1 )
		{
			printf( "Error %d calling ioctl( ..., IOSSIOSPEED, ... )\n", errno );
		}
		usleep(200);

		unsigned char d[255];
			
		d[0]=0;

		for(int u = 0; u < LED_WIDTH; u++)
		{
			d[u*6+1]=leds[u][0];
			d[u*6+2]=leds[u][1];
			d[u*6+3]=leds[u][2];
			d[u*6+4]=0;
			d[u*6+5]=0;
			d[u*6+6]=0;
		}
		write(serial_bridge,d,LED_WIDTH*6);




		Uint32 now = SDL_GetTicks() - lastFrame; 

		if( now < animations[current_animation].timing )
		{
			SDL_Delay(animations[current_animation].timing - now);
		}
		lastFrame = SDL_GetTicks();

		
		tick_count++;


		if(tick_count == animations[current_animation].duration)
		{
			animations[current_animation].deinit_fp();

			current_animation++;
			if(current_animation == animationcount)
			{
				current_animation = 0;
			}
			tick_count=0;

			SDL_Rect rect = { 0, 0, LED_WIDTH*ZOOM_X,ZOOM_Y };
			SDL_FillRect(
				screen, 
				&rect, 
				SDL_MapRGB(screen->format,0,0,0)
			);
			SDL_Flip(screen);
			SDL_Delay(300);


			animations[current_animation].init_fp();


		}
	}

	SDL_Quit();
	return 0;
}

