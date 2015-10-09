/* ==========================================================================
 * @file    : num_slider.h
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

#ifndef _NUM_SLIDER_H
#define _NUM_SLIDER_H

#include <SDL/SDL.h>

#define TIMEOUT 300	/* Timeout in seconds */
#define NUMBER_WIDTH 100
#define NUMBER_HEIGHT 100

#define BOUNDARY 5
#define GAP 5
#define ROW_SIZE 4
#define COLUMN_SIZE 4
#define BOARD_SIZE ROW_SIZE*COLUMN_SIZE

#define SCREEN_WIDTH COLUMN_SIZE*NUMBER_WIDTH + 2*BOUNDARY + GAP*(COLUMN_SIZE -1)
#define SCREEN_HEIGHT ROW_SIZE*NUMBER_HEIGHT + 2*BOUNDARY + GAP*(ROW_SIZE -1)

#define ever ;;
#define UTILS_DATADIR "PATH"

typedef struct {
	int arr[BOARD_SIZE];
	int size;
	int index_zero;
	SDL_Surface* surface;
} Board;

typedef struct game {
	int state;
	int pressed_key;
	int start_time;
	char curr_time[6];
	int time_out;
	Uint8 *key;
	Board board;
	SDL_Surface* play_screen;
	SDL_Surface* info_screen;
} Game;

enum game_state {
	STOP = 0,
	RUN,
	PAUSE,
};

enum surface {
	BOARD = 0,
	GAME_OVER,
};

enum pressed_key {
	KEY_NONE = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_SPACE,
};

Game *initGame(void);
void destroyGame(Game *thisgame);
void prepare_database(Game *thisgame);
void draw_box(Game *thisgame, int num, int x, int y);
void shuffle(Game *thisgame);
void reset_board(Game *thisgame);
void playpause_game(Game *thisgame);
void get_user_input(Game *thisgame);
int isvalid(int index, int key);
void game_over(int sig);
void update_game(Game *thisgame);
void draw_winpage(Game *thisgame);
void checkcompleted(Game *thisgame);
void update_timer(Game *thisgame);
void draw_board(Game *thisgame);

#endif
