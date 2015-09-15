/* ==========================================================================
 * @file    : lock.c
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

#include <signal.h>
#include <SDL/SDL.h>

#define ever ;;
#define CURR_DIR "PATH"

SDL_Surface *screen;
SDL_Surface *lock;

void display_screen(void)
{
	int alpha = 0;
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = lock->w;
	src.h = lock->h;
	dest.x = (screen->w - lock->w)/2;
	dest.y = (screen->h - lock->h)/2;
	dest.w = lock->w;
	dest.h = lock->h;

	while(alpha++ < 40) {
		SDL_SetAlpha(lock, SDL_SRCALPHA, alpha);
		SDL_BlitSurface(lock, &src, screen, &dest);
		SDL_Flip(screen);
		usleep(20000);
	}
	alarm(7);
}

void handle_alrm(int sig)
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Flip(screen);
}

int main()
{
	SDL_Event event;
	SDL_Surface *tmp;
	char passwd[20] = {0};
	char pwd[20] = "aman";
	char ch;
	int i = 0, display = 0;
	int kill = 0, mouse = 0;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(1366, 768, 32,
				  SDL_SWSURFACE | SDL_FULLSCREEN);

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Flip(screen);

	SDL_ShowCursor(SDL_DISABLE);
	tmp = SDL_LoadBMP(CURR_DIR"/access.bmp");
	lock = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	signal(SIGALRM, handle_alrm);

	for(ever) {
		usleep(10000);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					ch = event.key.keysym.sym;
					if(ch == 'q') {
						memset(passwd, 0, 20);
						i = 0;
					} else {
						passwd[i++] = ch;
						if(i > 19) {
							memset(passwd, 0, 20);
							i = 0;
						}
					}
					if(strcmp(passwd, pwd) == 0)
						kill = 1;
				case SDL_MOUSEBUTTONDOWN:
					display = 1;
					break;
				case SDL_MOUSEMOTION:
					mouse++;
					break;
				}
		}
		if(kill) {
			break;
		} else if(display || mouse > 20) {
			display_screen();
			display = 0;
			mouse = 0;
		}
	}

	SDL_Quit();
	return 0;
}
