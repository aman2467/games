/* ==========================================================================
 * @file    : sl_board.h
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

#ifndef _SL_BOARD_H
#define _SL_BOARD_H

#include <SDL/SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define STARTX 50
#define STARTY 40

#define BLOCK_WIDTH 97
#define BLOCK_HEIGHT 67

#define ever ;;
#define UTILS_DATADIR "PATH"

#define MAX_PLAYERS 4

typedef struct player {
	char name[20];
	bool play_again;
	int score;
	int dice_val;
} Player;

typedef struct game {
	bool iscompleted;
	bool update_board;
	int state;
	int tot_players;
	int curr_player;
	Player player[MAX_PLAYERS];
	Uint8 *key;
	SDL_Surface* start_screen;
	SDL_Surface* end_screen;
	SDL_Surface* board;
	SDL_Surface* dice;
	SDL_Surface* play_screen;
	SDL_Surface* player_surf;
} Game;

enum game_state {
	STOP = 0,
	RUN,
};

enum surface {
	BOARD = 0,
	PLAYER_1,
	PLAYER_2,
	PLAYER_3,
	PLAYER_4,
	GAME_OVER,
};

Game *initGame(int autoplay);
void prepare_database(Game *thisgame);
void destroyGame(Game *thisgame);
void detect_key_strokes(Game *thisgame);
void update_board(Game *thisgame);
void draw_board(Game *thisgame, int surface, int score);
int roll_dice(Game *thisgame);
void draw_dice(Game *thisgame, int score);
int isCompleted(Game *thisgame);

#endif
