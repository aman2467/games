/* ==========================================================================
 * @file    : pong.h
 *
 * @description : This file contains common definitions and declaration for
 *           the application.
 *
 * @author  : Aman Kumar (2015)
 *
 * @copyright   : The code contained herein is licensed under the GNU General
 *		Public License. You may obtain a copy of the GNU General
 *		Public License Version 2 or later at the following locations:
 *              http://www.opensource.org/licenses/gpl-license.html
 *              http://www.gnu.org/copyleft/gpl.html
 * ========================================================================*/

#ifndef _PONG_H
#define _PONG_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

//#define BIG_SCREEN

#ifndef BIG_SCREEN
#define BRICK_ROWS 5
#define START 0
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#else
#define BRICK_ROWS 8
#define START 325
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#endif

#define BRICK_COLUMNS 12
#define TOTAL_BRICKS BRICK_ROWS*BRICK_COLUMNS

#define SCREEN_BPP 32
#define SCREEN_LEFT 0
#define SCREEN_RIGHT (SCREEN_WIDTH - BAT_WIDTH)
#define SCREEN_TOP 0

#define BAT_WIDTH 80
#define BAT_HEIGHT 10
#define BAT_START (SCREEN_HEIGHT - BAT_HEIGHT)
#define BAT_SPEED 10

#define BALL_WIDTH 12
#define BALL_HEIGHT 12
#define BALL_SPEED 3

#define BRICK_WIDTH 40
#define BRICK_HEIGHT 15
#define BRICK_SPACE_WIDTH (15 + BRICK_WIDTH)
#define BRICK_SPACE_HEIGHT (15 + BRICK_HEIGHT)
#define BRICK_SPACER (7*BRICK_HEIGHT)

#define ever ;;
#define UTILS_DATADIR "PATH"

#define MAX_LIVES 3
#define MAX_LEVEL 5

typedef struct {
	int x;
	int y;
} Direction;

typedef struct {
	Direction direction;
	SDL_Rect node;
} Ball;

typedef struct {
	SDL_Rect node;
} Batpad;

typedef struct {
	int enable;
	int volume;
	Mix_Chunk *loosechance;
	Mix_Chunk *theme;
	Mix_Chunk *hitbat;
	Mix_Chunk *hitbrick;
} Gamesound;

typedef struct game {
	bool iscompleted;
	int state;
	int lives;
	int level;
	int level_score;
	int total_score;
	int brick_hits;
	bool autoplay;
	Uint8 *key;
	Batpad bat;
	Ball ball;
	SDL_Rect bricks[BRICK_ROWS][BRICK_COLUMNS];
	Gamesound sound;
	SDL_Event event;
	SDL_Surface* start_screen;
	SDL_Surface* play_screen;
	SDL_Surface* end_screen;
	SDL_Surface* info_screen;
	SDL_Surface* restart_screen;
	SDL_Surface* lives_screen;
} PONG;

enum game_state {
	STOP = 0,
	RUN,
	PAUSE,
	RESTART,
};

enum screen_overlay {
	START_SCREEN = 0,
	END_SCREEN,
	LIVES_SCREEN,
	RESTART_SCREEN,
	INFO_SCREEN,
};

PONG *initPong(int autoplay);
void init_audio(PONG *thisgame);
void draw_overlay(PONG *thisgame, int flag);
void prepare_database(PONG *thisgame);
void destroyPong(PONG *thisgame);
void restartPong(PONG *thisgame);
void detect_user_key_strokes(PONG *thisgame);
void update_game(PONG *thisgame);
void draw_game(PONG *thisgame);
void updateBall(PONG *thisgame);
void resetBricks(PONG *thisgame);
bool isHit(SDL_Rect rect1, SDL_Rect rect2);

#endif
