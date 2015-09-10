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
	char *key;
	int colorG = 0, colorB = 0;
	int reached = 0;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(1366, 768, 32,
				  SDL_SWSURFACE | SDL_FULLSCREEN);

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
		SDL_PumpEvents();
		key = SDL_GetKeyState(NULL);
		if(key[SDLK_ESCAPE]) break;
	}
	SDL_Quit();
	return 0;
}
