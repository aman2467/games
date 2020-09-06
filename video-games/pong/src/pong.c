/* ==========================================================================
 * @file    : pong.c
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
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <pong.h>

void prepare_database(PONG *thisgame)
{
	SDL_Surface *temp;
	char file[100] = {0};

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/game_start.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->start_screen = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/game_info.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->info_screen = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/game_over.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->end_screen = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
}

void draw_overlay(PONG *thisgame, int flag)
{
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Surface *overlay;
	int alpha = 0, delay = 0;
	int show_lives = 0, i;

	/* Reset Background */
	SDL_FillRect(thisgame->play_screen , NULL ,
		     SDL_MapRGB(thisgame->play_screen->format, 0, 0, 0));

	switch(flag) {
		case START_SCREEN:
			overlay = thisgame->start_screen;
			delay = 70000;
			break;
		case LIVES_SCREEN:
			show_lives = 1;
		case INFO_SCREEN:
			overlay = thisgame->info_screen;
			delay = 100000;
			break;
		case RESTART_SCREEN:
		case END_SCREEN:
			overlay = thisgame->end_screen;
			delay = 70000;
			break;
	}

	src.x = 0;
	src.y = 0;
	src.w = overlay->w;
	src.h = overlay->h;

	dest.x = (thisgame->play_screen->w/2) - (overlay->w/2);
	dest.y = (thisgame->play_screen->h/2) - (overlay->h/2);
	dest.w = overlay->w;
	dest.h = overlay->h;

	/* Display overlay screen */
	while(alpha < 50) {
		SDL_SetAlpha(overlay, SDL_SRCALPHA, alpha);
		SDL_BlitSurface(overlay, &src,
				thisgame->play_screen, &dest);
		if(show_lives) {
			SDL_Rect pos;

			pos.w = 20;
			pos.h = 20;
			pos.x = 400;
			pos.y = 100;

			for(i = 0; i < thisgame->lives; i++) {
				pos.y += 25;
				SDL_FillRect(overlay, &pos,
				     SDL_MapRGB(overlay->format,
						0, 255/(i+1), 0));
			}
			for(i = 0; i < MAX_LIVES - thisgame->lives; i++) {
				pos.y += 25;
				SDL_FillRect(overlay, &pos,
				     SDL_MapRGB(overlay->format,
						0, 0, 0));
			}
		}
		SDL_Flip(thisgame->play_screen);
		usleep(delay);
		alpha++;
	}
	if(show_lives && thisgame->sound.enable) {
		Mix_Resume(1);
	}
}

void init_audio(PONG *thisgame)
{
	char file[100] = {0};

	Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, 2, 1024);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/audio/chance_left.wav");
	thisgame->sound.loosechance = Mix_LoadWAV(file);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/audio/bat_hit.wav");
	thisgame->sound.hitbat = Mix_LoadWAV(file);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/audio/brick_hit.wav");
	thisgame->sound.hitbrick = Mix_LoadWAV(file);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/audio/theme.wav");
	thisgame->sound.theme = Mix_LoadWAV(file);

	Mix_Volume(1, thisgame->sound.volume);
	Mix_PlayChannel(1, thisgame->sound.theme, -1);

	if(!thisgame->sound.enable) {
		Mix_Pause(-1);
	}
}

PONG *initPong(int autoplay)
{
	PONG *newGame = malloc(sizeof(struct game));

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	newGame->play_screen = SDL_SetVideoMode(SCREEN_WIDTH,
					   SCREEN_HEIGHT,
					   SCREEN_BPP,
					   SDL_SWSURFACE);
	SDL_WM_SetCaption("My Pong", NULL);
	SDL_ShowCursor(SDL_DISABLE);
	prepare_database(newGame);

	newGame->iscompleted = 0;
	newGame->state = PAUSE;
	newGame->lives = MAX_LIVES;
	newGame->level = 1;
	newGame->level_score = 0;
	newGame->total_score = 0;
	newGame->brick_hits = 0;
	newGame->key = SDL_GetKeyState(NULL);
	newGame->bat.node.w = BAT_WIDTH;
	newGame->bat.node.h = BAT_HEIGHT;
	newGame->bat.node.x = (SCREEN_WIDTH - BAT_WIDTH)/2 ;
	newGame->bat.node.y = BAT_START - BAT_HEIGHT;
	newGame->autoplay = autoplay;

	newGame->ball.node.w = BALL_WIDTH;
	newGame->ball.node.h = BALL_HEIGHT;
	newGame->ball.node.x = SCREEN_WIDTH / 2;
	newGame->ball.node.y = (BRICK_SPACER + SCREEN_HEIGHT) / 2;
	newGame->ball.direction.x = 1;
	newGame->ball.direction.y = 1;
	newGame->sound.enable = 1;
	newGame->sound.volume = 20;

	resetBricks(newGame);
	init_audio(newGame);

	draw_overlay(newGame, START_SCREEN);
	draw_overlay(newGame, INFO_SCREEN);

	return newGame;
}

void restartPong(PONG *thisgame)
{

}

void destroyPong(PONG *thisgame)
{
	Mix_FreeChunk(thisgame->sound.hitbat);
	Mix_FreeChunk(thisgame->sound.hitbrick);
	Mix_FreeChunk(thisgame->sound.loosechance);
	Mix_FreeChunk(thisgame->sound.theme);
	free(thisgame);
	Mix_CloseAudio();
	SDL_Quit();
}

void detect_user_key_strokes(PONG *thisgame)
{
	while (SDL_PollEvent(&thisgame->event)) {
		switch (thisgame->event.type) {
			case SDL_QUIT:
				thisgame->state = STOP;
				break;
			}
	}
	thisgame->key = SDL_GetKeyState(NULL);

	/* Check key to move BAT left */
	if (thisgame->key[SDLK_LEFT] &&
	     thisgame->bat.node.x > SCREEN_LEFT) {
		thisgame->bat.node.x -= BAT_SPEED;
	/* Check key to move BAT right */
	} else if (thisgame->key[SDLK_RIGHT] &&
		    thisgame->bat.node.x < SCREEN_RIGHT) {
		thisgame->bat.node.x += BAT_SPEED;
	/* Check key to play/pause Game */
	} else if (thisgame->key[SDLK_SPACE]) {
		if(thisgame->state == PAUSE) {
			thisgame->state = RUN;
			if(thisgame->sound.enable) {
				Mix_Resume(-1);
			}
			usleep(100000);
		} else {
			if(thisgame->sound.enable) {
				Mix_Pause(-1);
			}
			thisgame->state = PAUSE;
			sleep(1);
		}
	/* Toggle Full screen */
	} else if(thisgame->key[SDLK_f]) {
		int flags = thisgame->play_screen->flags;
		thisgame->play_screen = SDL_SetVideoMode(SCREEN_WIDTH,
							 SCREEN_HEIGHT,
							 SCREEN_BPP,
							 thisgame->play_screen->flags^SDL_FULLSCREEN);
		if(NULL == thisgame->play_screen)
			thisgame->play_screen = SDL_SetVideoMode(SCREEN_WIDTH,
								 SCREEN_HEIGHT,
								 SCREEN_BPP,
								 flags);
	/* Check key to enable/disable sound */
	} else if(thisgame->key[SDLK_m]) {
		if(thisgame->sound.enable == 1) {
			thisgame->sound.enable = 0;
			Mix_Pause(-1);
		} else {
			thisgame->sound.enable = 1;
			Mix_Resume(-1);
		}
		usleep(100000);
	/* Check key to exit Game */
	} else if(thisgame->key[SDLK_ESCAPE]) {
		thisgame->state = STOP;
	}
}

void level_delay(int currlevel)
{
	usleep((MAX_LEVEL - currlevel) * 2000);
}

void updateBall(PONG *thisgame)
{
	int row, column;

	if (((thisgame->ball.node.y + BALL_HEIGHT) >= SCREEN_HEIGHT) ||
	     ((thisgame->ball.node.y + BAT_HEIGHT) >= SCREEN_HEIGHT)) {
		if(--thisgame->lives) {
			if(thisgame->sound.enable) {
				Mix_Pause(1);
				usleep(1000);
				Mix_PlayChannel(-1, thisgame->sound.loosechance, 0);
			}
			draw_overlay(thisgame, LIVES_SCREEN);
			thisgame->state = PAUSE;
		}
	}

	if ((thisgame->ball.node.y + BALL_HEIGHT) >= SCREEN_HEIGHT) {
		thisgame->ball.node.w = BALL_WIDTH;
		thisgame->ball.node.h = BALL_HEIGHT;
		thisgame->ball.node.x = SCREEN_WIDTH / 2;
		thisgame->ball.node.y = (BRICK_SPACER + SCREEN_HEIGHT) / 2;

	}

	if (isHit(thisgame->bat.node, thisgame->ball.node)) {
		if(thisgame->sound.enable) {
			Mix_PlayChannel(-1, thisgame->sound.hitbat, 0);
		}
		thisgame->ball.direction.y = -1;
	}

	if ((thisgame->ball.node.x + BALL_WIDTH) >= SCREEN_WIDTH) {
		thisgame->ball.direction.x = -1;
	} else if (thisgame->ball.node.x <= 0) {
		thisgame->ball.direction.x = 1;
	}

	if (thisgame->ball.node.y <= 0) {
		thisgame->ball.direction.y = 1;
	}

	for (row = 0; row < BRICK_ROWS; row++) {
		for (column = 0; column < BRICK_COLUMNS; column++) {
			if (isHit(thisgame->ball.node,
					thisgame->bricks[row][column]) &&
			    thisgame->bricks[row][column].w != 0) {
				if(thisgame->sound.enable) {
					Mix_PlayChannel(-1, thisgame->sound.hitbrick, 0);
				}
				thisgame->bricks[row][column].w = 0;
				thisgame->bricks[row][column].h = 0;
				thisgame->ball.direction.y *= -1;
				thisgame->brick_hits++;
				thisgame->level_score += 10;
				thisgame->total_score += 10;
			}
			if(thisgame->brick_hits >= TOTAL_BRICKS) {
				thisgame->iscompleted = 1;
			}
		}
	}

	thisgame->ball.node.x  += (thisgame->ball.direction.x * BALL_SPEED);
	thisgame->ball.node.y += (thisgame->ball.direction.y * BALL_SPEED);
	if(thisgame->autoplay) {
		if(thisgame->ball.node.x > SCREEN_WIDTH - (BALL_WIDTH)) {
			thisgame->bat.node.x  = SCREEN_WIDTH - BAT_WIDTH;
		} else {
			thisgame->bat.node.x  = thisgame->ball.node.x -
				BALL_WIDTH;
		}
		thisgame->bat.node.w  = BAT_WIDTH;
		thisgame->bat.node.h  = BAT_HEIGHT;
	}
}

void update_game(PONG *thisgame)
{
	char output[100] = {0};

	updateBall(thisgame);
	if (thisgame->lives <= 0) {
		if(thisgame->sound.enable) {
			Mix_Pause(1);
			usleep(1000);
			Mix_PlayChannel(-1, thisgame->sound.loosechance, 0);
		}
		sprintf(output,"xcowsay You Loose..!! Your score = %d",
			thisgame->total_score);
		if(system(output) < 0) {
			printf("Error : system()\n");
		}
		if(thisgame->sound.enable) {
			Mix_Resume(1);
		}
		draw_overlay(thisgame, END_SCREEN);
		thisgame->state = STOP;
	}
	if(thisgame->iscompleted) {
		if(thisgame->level == MAX_LEVEL) {
			sprintf(output, "xcowsay Congratulations, You Completed with total score %d..!!",
				  thisgame->total_score);
			if(system(output) < 0){
				printf("Error : system()\n");
			}
			draw_overlay(thisgame, RESTART_SCREEN);
			thisgame->state = RESTART;
			/* Reset Level */
			thisgame->level = 1;
		} else {
			sprintf(output,"xcowsay Congrates..!! You completed level-%d with score = %d",
				thisgame->level, thisgame->level_score);
			if(system(output) < 0) {
				printf("Error : system()\n");
			}
			/* Change to Next Level */
			thisgame->level++;
		}
		thisgame->level_score = 0;
		thisgame->iscompleted = 0;
		thisgame->brick_hits = 0;
		thisgame->lives = MAX_LIVES;
		thisgame->state = PAUSE;
		thisgame->bat.node.w = BAT_WIDTH;
		thisgame->bat.node.h = BAT_HEIGHT;
		thisgame->bat.node.x = (SCREEN_WIDTH - BAT_WIDTH)/2 ;
		thisgame->bat.node.y = BAT_START - BAT_HEIGHT;
		thisgame->ball.node.w = BALL_WIDTH;
		thisgame->ball.node.h = BALL_HEIGHT;
		thisgame->ball.node.x = SCREEN_WIDTH / 2;
		thisgame->ball.node.y = (BRICK_SPACER+SCREEN_HEIGHT)/2;
		thisgame->ball.direction.x = 1;
		thisgame->ball.direction.y = 1;

		resetBricks(thisgame);
	}
	level_delay(thisgame->level);
}

void draw_game(PONG *thisgame)
{
	int row, column;

	/* Draw Background */
	SDL_FillRect(thisgame->play_screen , NULL ,
		     SDL_MapRGB(thisgame->play_screen->format, 0, 0, 15));
	/* Draw Bat */
	SDL_FillRect(thisgame->play_screen, &thisgame->bat.node,
		     SDL_MapRGB(thisgame->play_screen->format, 255, 0, 0));
	/* Draw Ball */
	SDL_FillRect(thisgame->play_screen, &thisgame->ball.node ,
		     SDL_MapRGB(thisgame->play_screen->format, 0, 255, 255));
	/* Draw Bricks, if any */
	for (row = 0; row < BRICK_ROWS; row++) {
		for (column = 0; column < BRICK_COLUMNS; column++) {
			if (thisgame->bricks[row][column].w != 0) {
				SDL_FillRect(thisgame->play_screen,
					     &thisgame->bricks[row][column],
					     SDL_MapRGB(thisgame->play_screen->format,
						       255, 255, 255));
			}
		}
	}
	/* update the play_screen */
	if(thisgame->state != STOP)
		SDL_Flip(thisgame->play_screen);
}

void resetBricks(PONG *thisgame)
{
	int row, column;
	for (row = 0; row < BRICK_ROWS; row++) {
		for (column = 0; column < BRICK_COLUMNS; column++) {
			thisgame->bricks[row][column].w = BRICK_WIDTH;
			thisgame->bricks[row][column].h = BRICK_HEIGHT;
			thisgame->bricks[row][column].x =
				BRICK_SPACE_WIDTH * column + START;
			thisgame->bricks[row][column].y =
				BRICK_SPACER + BRICK_SPACE_HEIGHT * row;
		}
	}
}

bool isHit(SDL_Rect rect1, SDL_Rect rect2)
{
	bool hit = 1;

	if((rect1.y + rect1.h) <= rect2.y) {
		hit = 0;
	} else if(rect1.y >= (rect2.y + rect2.h)) {
		hit = 0;
	} else if((rect1.x + rect1.w) <= rect2.x) {
		hit = 0;
	} else if(rect1.x >= (rect2.x + rect2.w)) {
		hit = 0;
	}

	return hit;
}

int main(int argc, char **argv)
{
	int autoplay = 0;
	bool quit_game = 0;
	PONG *thisgame = NULL;

	if(argc == 2) {
		autoplay = atoi(argv[1]);
	}

	thisgame = initPong(autoplay);

	for(ever) {
		detect_user_key_strokes(thisgame);

		switch(thisgame->state) {
			case RUN:
				update_game(thisgame);
			case PAUSE:
				draw_game(thisgame);
				break;
			case RESTART:
				restartPong(thisgame);
				break;
			case STOP:
			default:
				destroyPong(thisgame);
				quit_game = 1;
				break;
		}

		if(quit_game) break;
		level_delay(thisgame->level);
	}

	return 0;
}
