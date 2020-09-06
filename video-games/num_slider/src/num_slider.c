/* ==========================================================================
 * @file    : num_slider.c
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

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <num_slider.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

void prepare_database(Game *thisgame)
{
	SDL_Surface *temp;
	char file[100] = {0};

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/numbers.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->board.surface = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	memset(file, 0, 100);
	strcpy(file, UTILS_DATADIR);
	strcat(file,"/game_over.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->info_screen = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
}

void draw_box(Game *thisgame, int num, int x, int y)
{
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Surface *overlay = thisgame->board.surface;

	src.x = num*NUMBER_WIDTH;
	src.y = 0;
	src.w = NUMBER_HEIGHT;
	src.h = overlay->h;

	dest.x = x + BOUNDARY + GAP*(x/NUMBER_WIDTH);
	dest.y = y + BOUNDARY + GAP*(y/NUMBER_HEIGHT);
	dest.w = overlay->w;
	dest.h = overlay->h;

	SDL_BlitSurface(overlay, &src, thisgame->play_screen, &dest);
}

void shuffle(Game *thisgame)
{

	struct timeval tv;
	int n = thisgame->board.size;
	int t, i, j;

	gettimeofday(&tv, NULL);
	thisgame->start_time = tv.tv_sec;
	srand48(tv.tv_usec);
	alarm(TIMEOUT);

	if (n > 1) {
		for (i = n - 1; i > 0; i--) {
			j = (unsigned int)(drand48()*(i+1));
			t = thisgame->board.arr[j];
			thisgame->board.arr[j] = thisgame->board.arr[i];
			thisgame->board.arr[i] = t;
			if(t == 0) {
				thisgame->board.index_zero = i;
			}
		}
	}
}

void reset_board(Game *thisgame)
{
	int i;

	for(i = 0; i < 5; i++) {
		shuffle(thisgame);
		usleep(100000);
		draw_board(thisgame);
	}
}

Game *initGame(void)
{
	int i = 0;
	Game *thisgame = malloc(sizeof(struct game));

	SDL_Init(SDL_INIT_VIDEO);
	thisgame->play_screen = SDL_SetVideoMode(SCREEN_WIDTH,
					   SCREEN_HEIGHT,
					   32,
					   SDL_SWSURFACE);

	/* Reset Background */
	SDL_FillRect(thisgame->play_screen , NULL ,
		     SDL_MapRGB(thisgame->play_screen->format, 0, 0, 0));
	prepare_database(thisgame);
	thisgame->state = RUN;
	thisgame->pressed_key = KEY_NONE;
	thisgame->board.size = BOARD_SIZE;
	for(i = 0; i < thisgame->board.size; i++) {
		thisgame->board.arr[i] = i;
	}
	shuffle(thisgame);
	draw_board(thisgame);

	return thisgame;
}

void destroyGame(Game *thisgame)
{
	free(thisgame);
	SDL_Quit();
}

void playpause_game(Game *thisgame)
{

	if(thisgame->state == RUN) {
		thisgame->state = PAUSE;
		SDL_FillRect(thisgame->play_screen , NULL ,
			     SDL_MapRGB(thisgame->play_screen->format, 255, 255, 0));
		SDL_Flip(thisgame->play_screen);
		sleep(1);
	} else {
		thisgame->state = RUN;
		SDL_FillRect(thisgame->play_screen , NULL ,
			     SDL_MapRGB(thisgame->play_screen->format, 0, 0, 0));
		SDL_Flip(thisgame->play_screen);
		thisgame->pressed_key = KEY_SPACE;
		usleep(300000);
	}
}

void get_user_input(Game *thisgame)
{
	SDL_Event ev;
	int x, y;
	int index_zero_x, index_zero_y;

	SDL_PollEvent(&ev);
	thisgame->key = SDL_GetKeyState(NULL);

	switch(ev.type) {
		case SDL_QUIT:
			thisgame->state = STOP;
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&x, &y);
			index_zero_x = (NUMBER_WIDTH *
				(thisgame->board.index_zero%COLUMN_SIZE));
			index_zero_y = (NUMBER_HEIGHT *
				(thisgame->board.index_zero/ROW_SIZE));
			if((y - index_zero_y) <= NUMBER_HEIGHT &&
			   (y - index_zero_y) >= 0) {
				if((index_zero_x - x) <= NUMBER_WIDTH &&
				   (index_zero_x - x) >= 0) {
					thisgame->pressed_key = KEY_RIGHT;
				} else if((x - (index_zero_x + NUMBER_WIDTH)) <= NUMBER_WIDTH &&
					  (x - (index_zero_x + NUMBER_WIDTH)) >= 0) {
					thisgame->pressed_key = KEY_LEFT;
				}
			} else if((x - index_zero_x) <= NUMBER_WIDTH &&
				  (x - index_zero_x) >= 0) {
				if((index_zero_y - y) <= NUMBER_HEIGHT &&
				   (index_zero_y - y) >= 0) {
					thisgame->pressed_key = KEY_DOWN;
				} else if((y - (index_zero_y + NUMBER_HEIGHT)) <= NUMBER_HEIGHT &&
					  (y - (index_zero_y + NUMBER_HEIGHT)) >= 0) {
					thisgame->pressed_key = KEY_UP;
				}
			}
			break;
		default:
			break;
	}
	if(thisgame->key[SDLK_ESCAPE]) {
		thisgame->state = STOP;
	} else if(thisgame->key[SDLK_r]) {
		reset_board(thisgame);
	} else if(thisgame->key[SDLK_UP]) {
		thisgame->pressed_key = KEY_UP;
	} else if(thisgame->key[SDLK_DOWN]) {
		thisgame->pressed_key = KEY_DOWN;
	} else if(thisgame->key[SDLK_RIGHT]) {
		thisgame->pressed_key = KEY_RIGHT;
	} else if(thisgame->key[SDLK_LEFT]) {
		thisgame->pressed_key = KEY_LEFT;
	} else if(thisgame->key[SDLK_SPACE]) {
		//playpause_game(thisgame);
	}

}

void update_timer(Game *thisgame)
{
	char caption[16] = {0};
	char title_bar[128] = {0};
	struct timeval tv;
	int time_left, consume;

	if(thisgame->state != PAUSE) {
		gettimeofday(&tv, NULL);
		consume = tv.tv_sec - thisgame->start_time;
		time_left = TIMEOUT - consume;

		if(time_left%60 < 10) {
			sprintf(caption,"0%d:0%d",time_left/60, time_left%60);
		} else {
			sprintf(caption,"0%d:%2d",time_left/60, time_left%60);
		}
		if(strcmp(caption, thisgame->curr_time) != 0) {
			memcpy(thisgame->curr_time, caption, 6);
			sprintf(title_bar,"            Num-Slider {Time left = %s}", caption);
			SDL_WM_SetCaption(title_bar, NULL);
			if(strcmp(caption,"00:00") == 0) {
				thisgame->time_out = 1;
			}
		}
	}
}

void draw_board(Game *thisgame)
{
	int i;

	for(i = 0; i < thisgame->board.size; ++i) {
		draw_box(thisgame,
			 thisgame->board.arr[i],
			 (NUMBER_WIDTH*i)%400,
			 NUMBER_HEIGHT*(i/4));
	}
	SDL_Flip(thisgame->play_screen);
}

int isvalid(int index, int key)
{
	int valid = 1;
	int num, i;

	switch(key) {
		case KEY_RIGHT:
			num = COLUMN_SIZE - 1;
			for(i = 0; i < ROW_SIZE; i++) {
				if(index == num + (i*COLUMN_SIZE)) {
					valid = 0;
				}
			}
			break;
		case KEY_LEFT:
			num = 0;
			for(i = 0; i < ROW_SIZE; i++) {
				if(index == num + (i*COLUMN_SIZE)) {
					valid = 0;
				}
			}
			break;
		default:
			break;
	}

	return valid;
}

void draw_winpage(Game *thisgame)
{
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Surface *overlay = thisgame->info_screen;

	sleep(1);
	SDL_FillRect(thisgame->play_screen , NULL ,
		     SDL_MapRGB(thisgame->play_screen->format, 0, 0, 0));

	src.x = 0;
	src.y = 0;
	src.w = overlay->w;
	src.h = overlay->h;

	dest.x = (SCREEN_WIDTH - overlay->w)/2;
	dest.y = (SCREEN_HEIGHT - overlay->h)/2;
	dest.w = overlay->w;
	dest.h = overlay->h;

	SDL_BlitSurface(overlay,
			&src,
			thisgame->play_screen,
			&dest);
	SDL_Flip(thisgame->play_screen);
	sleep(2);
	thisgame->state = STOP;
}

void checkcompleted(Game *thisgame)
{
	int i;

	for(i = 1; i < thisgame->board.size; i++) {
		if(thisgame->board.arr[i-1] != i)
			break;
	}
	if(i == thisgame->board.size || thisgame->time_out)
		draw_winpage(thisgame);
}

void update_game(Game *thisgame)
{
	int tmp, num;

	switch(thisgame->pressed_key) {
		case KEY_DOWN:
			if(thisgame->board.index_zero >= COLUMN_SIZE) {
				tmp = thisgame->board.index_zero;
				num = thisgame->board.arr[tmp - COLUMN_SIZE];
				thisgame->board.index_zero = tmp - COLUMN_SIZE;
				thisgame->board.arr[tmp] = num;
				thisgame->board.arr[tmp - COLUMN_SIZE] = 0;
			}
			break;
		case KEY_UP:
			if(thisgame->board.index_zero < BOARD_SIZE-COLUMN_SIZE) {
				tmp = thisgame->board.index_zero;
				num = thisgame->board.arr[tmp + COLUMN_SIZE];
				thisgame->board.index_zero = tmp + COLUMN_SIZE;
				thisgame->board.arr[tmp] = num;
				thisgame->board.arr[tmp + COLUMN_SIZE] = 0;
			}
			break;
		case KEY_RIGHT:
			if(isvalid(thisgame->board.index_zero, KEY_LEFT)) {
				tmp = thisgame->board.index_zero;
				num = thisgame->board.arr[tmp - 1];
				thisgame->board.index_zero = tmp - 1;
				thisgame->board.arr[tmp] = num;
				thisgame->board.arr[tmp - 1] = 0;
			}
			break;
		case KEY_LEFT:
			if(isvalid(thisgame->board.index_zero, KEY_RIGHT)) {
				tmp = thisgame->board.index_zero;
				num = thisgame->board.arr[tmp + 1];
				thisgame->board.index_zero = tmp + 1;
				thisgame->board.arr[tmp] = num;
				thisgame->board.arr[tmp + 1] = 0;
			}
			break;
		case KEY_SPACE:
			break;
		default:
			thisgame->pressed_key = KEY_NONE;
			break;
	}
	if(thisgame->pressed_key != KEY_NONE) {
		draw_board(thisgame);
		thisgame->pressed_key = KEY_NONE;
		usleep(300000);
	}
	update_timer(thisgame);
	checkcompleted(thisgame);
}

int main(int argc, char **argv)
{
	bool quit_game = 0;
	Game *thisgame = NULL;
#if 0
	int lock_fd, ret;
#endif

	if(argc == 2) {
		if(strcmp(argv[1],"-h") == 0) {
			printf("\nUsage : %s\n", argv[0]);
			printf("\n      : %s -h\n", argv[0]);
			exit(0);
		}
	} else if(argc > 2) {
		printf("\nUsage : %s\n", argv[0]);
		printf("\n      : %s -h\n", argv[0]);
		exit(0);
	}
#if 0
	lock_fd = open("/var/run/num_slider.pid", O_RDONLY);
	if(lock_fd < 0) {
		printf("Error : Opening file\n");
		exit(0);
	}
	ret = flock(lock_fd, LOCK_EX | LOCK_NB);
	/* check if this is the only instance */
	if(ret != 0) {
		exit(0);
	}
#endif
	thisgame = initGame();

	for(ever) {
		switch(thisgame->state) {
			case RUN:
				get_user_input(thisgame);
				update_game(thisgame);
				break;
			case STOP:
			default:
				destroyGame(thisgame);
				quit_game = 1;
				break;
		}

		if(quit_game) break;
		usleep(8000);
	}

	return 0;
}
