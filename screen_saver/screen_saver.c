#include <SDL/SDL.h>

int main()
{
	SDL_Surface* screen;
	SDL_Event event;
	char *key;
	int color1 = 0, color2 = 0;
	int kill = 0, reached = 0;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(1366, 768, 32,
				  SDL_SWSURFACE | SDL_FULLSCREEN);

	while(1) {
		SDL_FillRect(screen, NULL,
			     SDL_MapRGB(screen->format, 255, color1,
					color2));
		usleep(10000);
		key = SDL_GetKeyState(NULL);
		SDL_Flip(screen);
		if(!reached) {
			if((color1 < 255) && (color2 == 0)) {
				color1++;
			} else if((color1 == 255) && (color2 < 255)) {
				color2++;
			} else if((color1 == 255) && (color2 == 255)) {
				reached = 1;
			}
		} else if(reached) {
			if(color1 == 255 && color2 > 0) {
				color2--;
			} else if(color1 > 0 && color2 == 0) {
				color1--;
			} else if(color1 == 0 && color2 == 0) {
				reached = 0;
			}
		}
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					kill = 1;
					break;
			}
		}
		if(kill) break;
		if(key[SDLK_ESCAPE]) break;
	}
	SDL_Quit();
	return 0;
}
