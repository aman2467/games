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
#include <SDL/SDL.h>
#include <pong.h>

PONG *initPong(int autoplay)
{
	PONG *newGame = malloc(sizeof(struct game));

	SDL_Init(SDL_INIT_VIDEO);
	newGame->screen = SDL_SetVideoMode(SCREEN_WIDTH,
					   SCREEN_HEIGHT,
					   SCREEN_BPP,
					   SDL_SWSURFACE|SDL_RESIZABLE);
	SDL_WM_SetCaption("My Pong", NULL);

	newGame->iscompleted = 0;
	newGame->state = PAUSE;
	newGame->lives = MAX_LIVES;
	newGame->level = 0;
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

	loadBricks(newGame);

	return newGame;
}
void destroyPong(PONG *thisgame)
{
	free(thisgame);
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
			usleep(100000);
		} else {
			thisgame->state = PAUSE;
			sleep(1);
		}
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
		thisgame->lives--;
	}

	if ((thisgame->ball.node.y + BALL_HEIGHT) >= SCREEN_HEIGHT) {
		thisgame->ball.node.w = BALL_WIDTH;
		thisgame->ball.node.h = BALL_HEIGHT;
		thisgame->ball.node.x = SCREEN_WIDTH / 2;
		thisgame->ball.node.y = (BRICK_SPACER + SCREEN_HEIGHT) / 2;

	}

	if (isHit(thisgame->bat.node, thisgame->ball.node)) {
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
				thisgame->bricks[row][column].w = 0;
				thisgame->bricks[row][column].h = 0;
				thisgame->ball.direction.y *= -1;
				thisgame->brick_hits++;
				thisgame->level_score += 10;
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
	thisgame->key = SDL_GetKeyState(NULL);
	if (thisgame->lives <= 0) {
		if(system("xcowsay Game Over..!!") < 0) {
			printf("Error : system()\n");
		}
		exit(0);
	}
	if(thisgame->iscompleted) {
		char output[100] = {0};
		if(thisgame->level == MAX_LEVEL) {
			sprintf(output, "xcowsay Congratulations, You Completed with total score %d..!!",
				  thisgame->total_score);
			if(system(output) < 0){
				printf("Error : system()\n");
			}
			exit(0);
		} else {
			sprintf(output,"xcowsay Congrates..!! You completed level-%d with score = %d",
				thisgame->level, thisgame->level_score);
			if(system(output) < 0) {
				printf("Error : system()\n");
			}
			/* Change to Next Level */
			thisgame->level++;
			thisgame->total_score += thisgame->level_score;
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
			thisgame->ball.node.y = (BRICK_SPACER + SCREEN_HEIGHT) / 2;
			thisgame->ball.direction.x = 1;
			thisgame->ball.direction.y = 1;

			loadBricks(thisgame);
		}
	}
	updateBall(thisgame);
	level_delay(thisgame->level);
}

void draw_game(PONG *thisgame)
{
	int row, column;

	/* Draw Background */
	SDL_FillRect(thisgame->screen , NULL ,
		     SDL_MapRGB(thisgame->screen->format, 0, 0, 0));
	/* Draw Bat */
	SDL_FillRect(thisgame->screen, &thisgame->bat.node,
		     SDL_MapRGB(thisgame->screen->format, 255, 0, 0));
	/* Draw Ball */
	SDL_FillRect(thisgame->screen, &thisgame->ball.node ,
		     SDL_MapRGB(thisgame->screen->format, 0, 255, 127));
	/* Draw Bricks, if any */
	for (row = 0; row < BRICK_ROWS; row++) {
		for (column = 0; column < BRICK_COLUMNS; column++) {
			if (thisgame->bricks[row][column].w != 0) {
				SDL_FillRect(thisgame->screen,
					     &thisgame->bricks[row][column],
					     SDL_MapRGB(thisgame->screen->format,
							row*20,
							20*(thisgame->level+1),
							column*(20+thisgame->level)));
			}
		}
	}
	/* update the screen */
	SDL_Flip(thisgame->screen);
}

void loadBricks(PONG *thisgame)
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
	PONG *thisgame = NULL;

	if(argc == 2) {
		autoplay = atoi(argv[1]);
	}
	thisgame = initPong(autoplay);

	for(ever) {
		if (thisgame->state) {
			detect_user_key_strokes(thisgame);
			if(thisgame->state != PAUSE) {
				update_game(thisgame);
			}
			draw_game(thisgame);
			level_delay(thisgame->level);
		} else {
			break;
		}
	}

	destroyPong(thisgame);

	return 0;
}
