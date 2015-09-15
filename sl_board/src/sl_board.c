/* ==========================================================================
 * @file    : sl_board.c
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
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <sl_board.h>

void prepare_database(Game *thisgame)
{
	SDL_Surface *temp;
	unsigned char colorkey;
	char file[100] = {0};

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/sl_board.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->board = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/players.bmp");
	temp = SDL_LoadBMP(file);
	thisgame->player_surf = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	strcpy(file, UTILS_DATADIR);
	strcat(file,"/dice.bmp");
	temp = SDL_LoadBMP(file);
	colorkey = SDL_MapRGB(temp->format, 200, 200, 200);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey);
	thisgame->dice = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

}

void draw_dice(Game *thisgame, int dice)
{
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Surface *overlay = thisgame->dice;

	src.x = (dice-1)*48;
	src.y = 0;
	src.w = overlay->w/6;
	src.h = overlay->h;

	dest.x = 1024+100;
	dest.y = 360;
	dest.w = overlay->w/6;
	dest.h = overlay->h;

	SDL_BlitSurface(overlay, &src, thisgame->play_screen, &dest);
	SDL_Flip(thisgame->play_screen);
	thisgame->player[thisgame->curr_player].dice_val = 0;
}

int roll_dice(Game *thisgame)
{
	int i = 0;
	int score = 0;

	for(i = 0; i < 20; i++) {
		score = rand()%6 + 1;
		draw_dice(thisgame, score);
		usleep(5000*i);
	}
	sleep(1);

	return score;
}

void draw_board(Game *thisgame, int surface, int score)
{
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Surface *overlay;
	int x, y, blit = 1;
	char color_key;

	if((score/10)%2 != 0) {
		if(score%10 == 0) {
			x = (9*BLOCK_WIDTH);
			y = ((10-(score/10))*BLOCK_HEIGHT);
		} else {
			x = ((10-(score%10))*BLOCK_WIDTH);
			y = ((9-(score/10))*BLOCK_HEIGHT);
		}
	} else {
		if(score%10 == 0) {
			x = 0;
			y = ((10-(score/10))*BLOCK_HEIGHT);
		} else {
			x = (((score%10)-1)*BLOCK_WIDTH);
			y = ((9-(score/10))*BLOCK_HEIGHT);
		}
	}

	switch(surface) {
		case BOARD:
			overlay = thisgame->board;
			src.x = 0;
			src.y = 0;
			src.w = overlay->w;
			src.h = overlay->h;

			dest.x = 0;
			dest.y = 0;
			dest.w = overlay->w;
			dest.h = overlay->h;
			break;
		case PLAYER_1:
			overlay = thisgame->player_surf;
			src.x = 0;
			src.y = 0;
			src.w = overlay->w/2-2;
			src.h = overlay->h/2;

			dest.x = STARTX + x;
			dest.y = STARTY + y;
			dest.w = overlay->w/2-2;
			dest.h = overlay->h/2;
			break;
		case PLAYER_2:
			overlay = thisgame->player_surf;
			src.x = overlay->w/2;
			src.y = 0;
			src.w = overlay->w/2-2;
			src.h = overlay->h/2;

			dest.x = STARTX + x+2;
			dest.y = STARTY + y;
			dest.w = overlay->w/2-2;
			dest.h = overlay->h/2;
			break;
		case PLAYER_3:
			overlay = thisgame->player_surf;
			src.x = 0;
			src.y = overlay->h/2;
			src.w = overlay->w/2-2;
			src.h = overlay->h/2;

			dest.x = STARTX + x+4;
			dest.y = STARTY + y;
			dest.w = overlay->w/2-2;
			dest.h = overlay->h/2;
			break;
		case PLAYER_4:
			overlay = thisgame->player_surf;
			src.x = overlay->w/2;
			src.y = overlay->h/2;
			src.w = overlay->w/2-2;
			src.h = overlay->h/2;

			dest.x = STARTX + x+6;
			dest.y = STARTY + y;
			dest.w = overlay->w/2-2;
			dest.h = overlay->h/2;
			break;
		case GAME_OVER:
			blit = 0;
			switch(score) {
				case PLAYER_1:
					color_key = SDL_MapRGB(thisgame->play_screen->format,
							       0, 0, 255);
					break;
				case PLAYER_2:
					color_key = SDL_MapRGB(thisgame->play_screen->format,
							       0, 255, 0);
					break;
				case PLAYER_3:
					color_key = SDL_MapRGB(thisgame->play_screen->format,
							       255, 0, 0);
					break;
				case PLAYER_4:
					color_key = SDL_MapRGB(thisgame->play_screen->format,
							       255, 255, 0);
					break;
			}
			SDL_FillRect(thisgame->play_screen, NULL, color_key);
			break;
	}
	if(blit)
		SDL_BlitSurface(overlay, &src, thisgame->play_screen, &dest);
}

int sl_lut(int out)
{
	int score;

	switch(out) {
		/* Ladders */
		case 3:
			score = 21;
			break;
		case 8:
			score = 30;
			break;
		case 28:
			score = 84;
			break;
		case 58:
			score = 77;
			break;
		case 75:
			score = 86;
			break;
		case 80:
			score = 100;
			break;
		case 90:
			score = 91;
			break;
		/* Snakes */
		case 17:
			score = 13;
			break;
		case 52:
			score = 29;
			break;
		case 57:
			score = 40;
			break;
		case 62:
			score = 22;
			break;
		case 88:
			score = 18;
			break;
		case 95:
			score = 51;
			break;
		case 97:
			score = 79;
			break;
		default:
			score = out;
			break;
	}

	return score;
}

Game *initGame(int tot_players)
{
	int i = 0;
	Game *thisgame = malloc(sizeof(struct game));

	SDL_Init(SDL_INIT_VIDEO);
	thisgame->play_screen = SDL_SetVideoMode(SCREEN_WIDTH,
					   SCREEN_HEIGHT,
					   32,
					   SDL_SWSURFACE|SDL_FULLSCREEN);

	/* Reset Background */
	SDL_FillRect(thisgame->play_screen , NULL ,
		     SDL_MapRGB(thisgame->play_screen->format, 200, 200, 200));
	SDL_WM_SetCaption("Snake & Ladder", NULL);
	SDL_ShowCursor(SDL_DISABLE);

	prepare_database(thisgame);
	thisgame->iscompleted = 0;
	thisgame->update_board = 0;
	thisgame->state = RUN;
	thisgame->tot_players = tot_players;
	thisgame->curr_player = 0;
	for(i = 0; i < tot_players; i++) {
		thisgame->player[i].score = 0;
	}
	srand(getpid());
	draw_board(thisgame, 0, 0);
	SDL_Flip(thisgame->play_screen);

	return thisgame;
}

void destroyGame(Game *thisgame)
{
	free(thisgame);
	SDL_Quit();
}

void detect_key_strokes(Game *thisgame)
{
	int score;
	SDL_PumpEvents();
	thisgame->key = SDL_GetKeyState(NULL);

	/* Check key to update player score */
	if(thisgame->key[SDLK_RETURN]) {
		thisgame->update_board = 1;
		score = roll_dice(thisgame);
		if(thisgame->player[thisgame->curr_player].score+score == 100) {
			thisgame->iscompleted = 1;
			thisgame->player[thisgame->curr_player].score = 100;
		} else if(thisgame->player[thisgame->curr_player].score+score < 100) {
			thisgame->player[thisgame->curr_player].score += score;
		}
		if(score == 6)
			thisgame->player[thisgame->curr_player].play_again = 1;
		thisgame->player[thisgame->curr_player].dice_val = score;
#ifdef _DEBUG
		printf("player = %d, score = %d(%d)\t next_player = %d\n",
		       thisgame->curr_player+1,
		       score,
		       thisgame->player[thisgame->curr_player].score,
		       (thisgame->curr_player+2 > thisgame->tot_players)?1:thisgame->curr_player+2);
#endif
	/* Check key to exit Game */
	} else if(thisgame->key[SDLK_ESCAPE]) {
		thisgame->state = STOP;
	}
}

void update_board(Game *thisgame)
{
	int i, skip = 0;
	int score = 0, new_score = 0;
	int dice_val = thisgame->player[thisgame->curr_player].dice_val;

	draw_board(thisgame, BOARD, 0);
	for(i = 0; i < thisgame->tot_players; i++) {
		if(i != thisgame->curr_player)
			draw_board(thisgame, i+1, thisgame->player[i].score);
	}
	draw_board(thisgame, thisgame->curr_player+1,
		   thisgame->player[thisgame->curr_player].score);

	SDL_Flip(thisgame->play_screen);
	score = thisgame->player[thisgame->curr_player].score;
	if(!thisgame->player[thisgame->curr_player].play_again) {
		new_score = sl_lut(thisgame->player[thisgame->curr_player].score);
	} else {
		new_score = score;
		skip = 1;
		thisgame->player[thisgame->curr_player].play_again = 0;
	}

	if(new_score != score) {
		sleep(1);
		thisgame->player[thisgame->curr_player].score = new_score;
		draw_board(thisgame, BOARD, 0);
		for(i = 0; i < thisgame->tot_players; i++) {
			if(i != thisgame->curr_player)
				draw_board(thisgame, i+1,
					   thisgame->player[i].score);
		}
		draw_board(thisgame, thisgame->curr_player+1,
			   thisgame->player[thisgame->curr_player].score);
		SDL_Flip(thisgame->play_screen);
	}

	if((score >= new_score) && !skip) {
			thisgame->curr_player++;
		if(thisgame->curr_player >= thisgame->tot_players)
			thisgame->curr_player = 0;
	}
	if(new_score == 100) thisgame->iscompleted = 1;
}

int isCompleted(Game *thisgame)
{
	int i;
	int result = 0;

	if(thisgame->iscompleted) {
		for(i = 0; i < thisgame->tot_players; i++) {
			switch(thisgame->player[i].score) {
				case 100:
					draw_board(thisgame, GAME_OVER, i+1);
					SDL_Flip(thisgame->play_screen);
					sleep(2);
					result = 1;
					break;
				default:
					break;
			}
		}
	}
	return result;
}

int main(int argc, char **argv)
{
	int num_players = 2;
	bool quit_game = 0;
	Game *thisgame = NULL;

	if(argc == 2) {
		if(strcmp(argv[1],"-h") == 0) {
			printf("\nUsage %s <no of players>\n", argv[0]);
			exit(0);
		} else {
			num_players = atoi(argv[1]);
		}
	} else if(argc > 2) {
		printf("\nUsage %s <no of players>\n", argv[0]);
		exit(0);
	}
	assert(num_players <= MAX_PLAYERS);
	thisgame = initGame(num_players);

	for(ever) {
		switch(thisgame->state) {
			case RUN:
				detect_key_strokes(thisgame);
				if(thisgame->update_board) {
					update_board(thisgame);
					if(isCompleted(thisgame)) {
						thisgame->state = STOP;
					}
					thisgame->update_board = 0;
				}
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
