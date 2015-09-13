/* ==========================================================================
 * @file    : screen_saver.c
 *
 * @description : This file contains main program.
 *
 * @author  : Aman Kumar (2015)
 *
 * @copyright   : The code contained herein is licensed under the GNU General
 *		Public License. You may obtain a copy of the GNU General
 *		Public License Version 2 or later at the following locations:
 *              http://www.opensource.org/licenses/gpl-license.html
 *              http://www.gnu.org/copyleft/gpl.html
 * ========================================================================*/

#include <SDL/SDL.h>

#define ever ;;

int main()
{
	SDL_Surface* screen;
	SDL_Event event;
	int kill = 0, mouse = 0;
	int colorG = 0, colorB = 0;
	int reached = 0;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(1366, 768, 32,
				  SDL_SWSURFACE | SDL_FULLSCREEN);

	SDL_ShowCursor(SDL_DISABLE);
	for(ever) {
		SDL_FillRect(screen, NULL,
			     SDL_MapRGB(screen->format, 255, colorG,
					colorB));
		usleep(10000);
		SDL_Flip(screen);
		if(!reached) {
			if((colorG < 255) && (colorB == 0)) {
				colorG++;
			} else if((colorG == 255) && (colorB < 255)) {
				colorB++;
			} else if((colorG == 255) && (colorB == 255)) {
				reached = 1;
			}
		} else if(reached) {
			if(colorG == 255 && colorB > 0) {
				colorB--;
			} else if(colorG > 0 && colorB == 0) {
				colorG--;
			} else if(colorG == 0 && colorB == 0) {
				reached = 0;
			}
		}
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
				case SDL_KEYDOWN:
					kill = 1;
					break;
				case SDL_MOUSEMOTION:
					if(mouse++ == 20) kill = 1;
					break;
				}
		}
		if(kill) break;
	}

	SDL_Quit();
	return 0;
}
