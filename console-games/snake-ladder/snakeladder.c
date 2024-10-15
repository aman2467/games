/**
 * gcc snakeladder.c -o snakeladder
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RED "\x1b[01;31m"
#define GRN "\x1b[01;30m"
#define N "\x1b[m"

static int arr[100];
static int usr1, usr2;
static int snake_start[] = {28, 36, 53, 63, 76, 88, 96, 99};
static int snake_end[] = {9, 3, 11, 20, 6, 49, 47, 60};
static int ladder_start[] = {5, 12, 15, 31, 40, 46, 56, 71};
static int ladder_end[] = {27, 33, 43, 69, 78, 74, 98, 92};

static void
show_status(void)
{
	printf("Player "RED"RED"N": %d\n", usr1);
	printf("Player "GRN"GREEN"N": %d\n", usr2);

	if (usr1 == 100) {
		printf("\nPlayer "RED"RED"N" has won the game..!!\n");
		exit(0);
	}
	if (usr2 == 100) {
		printf("\nPlayer "GRN"GREEN"N" has won the game..!!\n");
		exit(0);
	}
}

static void
move_on_board(int *usr)
{
	int i;

	for (i = 0; i < 8; i++) {
		if (*usr == snake_start[i]) {
			*usr = snake_end[i];
			break;
		} else if (*usr == ladder_start[i]) {
			*usr = ladder_end[i];
			break;
		}
	}
}

static void
display_box(int i)
{
	if (arr[i - 1] == usr1)
		printf("|___"RED"%3d"N"___|", arr[i - 1]);
	else if (arr[i - 1] == usr2)
		printf("|___"GRN"%3d"N"___|", arr[i - 1]);
	else
		printf("|___%3d___|", arr[i - 1]);
}

static void
display_board(void)
{
	int i;

	for (i = 0; i < 8; i++) {
		printf("Ladder is at %2d which takes you up to %d\t\t",
		       ladder_start[i],
		       ladder_end[i]);
		printf("Snake is at %2d which takes you down to %d\n",
		       snake_start[i],
		       snake_end[i]);
	}
	printf("\n\n");
	for (i = 0; i < 10; i++)
		printf(" _________ ");

	printf("\n");
	for (i = 1; i <= 100; i++) {
		display_box(i);
		if (i % 10 == 0)
			printf("\n");
	}
}

static void
play(int *usr)
{
	int new_chance;

	srand(getpid()*rand());
	new_chance = rand() % 7;
	if ((*usr + new_chance) <= 100)
		*usr = *usr + new_chance;
	move_on_board(usr);
	system("clear");
	display_board();
	printf("Well..You have got %d this roll..\n", new_chance);
	show_status();
}

static void
init_board(void)
{
	int i, j;

	for (i = 0, j = 100; i < 100; i++, j--)
		arr[i] = j;
}

int
main(void)
{
	init_board();
	system("clear");
	display_board();

	while (1) {
		printf("\nPlayer "RED"RED"N": Hit Enter to roll the die..");
		getchar();
		play(&usr1);
		printf("\nPlayer "GRN"GREEN"N": Hit Enter to roll the die..");
		getchar();
		play(&usr2);
	}
	return 0;
}
