/* @File   	: snakeladder.c
 * @Auhor  	: Aman Kumar
 * @Brief   : This program simulates the snake-ladder game.
 */ 

/*header file*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void display_box(int);
void display_board(void);
void init_board(void);
void play(int *);
void move_on_board(int *);
void show_status(void);

#define RED "\x1b[01;31m"
#define GREEN "\x1b[01;32m"
#define RES "\x1b[01;0m"


int arr[100];
int usr1,usr2;
int snake_start[]={28,36,53,63,76,88,96,99};
int snake_end[]={9,3,11,20,6,49,47,60};
int ladder_start[]={5,12,15,31,40,46,56,71};
int ladder_end[]={27,33,43,69,78,74,98,92};

main()
{
    init_board();
    display_board();
    while(1) {
        printf("\nPlayer "RED"RED"RES": Hit Enter to roll the die..");
        getchar();
        play(&usr1);
        printf("\nPlayer "GREEN"GREEN"RES": Hit Enter to roll the die..");
        getchar();
        play(&usr2);
    }

}
void play(int *usr)
{
    int new_chance;
repeat:
	srand(time(NULL));
    new_chance=rand()%7;
	if(new_chance == 0) goto repeat;
    *usr=*usr+new_chance;
    move_on_board(usr);
    if(system("clear") < 0);
    display_board();
    printf(GREEN"\nWell..You have got "RED"%d"GREEN" this roll..\n"RES,new_chance);
    show_status();
}
void show_status(void)
{
    printf("Player "RED"RED"RES": %d\n",usr1);
    printf("Player "GREEN"GREEN"RES": %d\n",usr2);
    if(usr1>=100) {
        printf("\nPlayer "RED"RED"RES" has won the game..!!\n");
        exit(0);
    }
    if(usr2>=100) {
        printf("\nPlayer "GREEN"GREEN"RES" has won the game..!!\n");
        exit(0);
    }
}

void move_on_board(int *usr)
{
    int i;
    for(i=0;i<8;i++) {
        if(*usr==snake_start[i]) {
            *usr=snake_end[i];
            break;
        }
        if(*usr==ladder_start[i]) {
            *usr=ladder_end[i];
            break;
        }
    }
    
}
void init_board()
{
    int i,j;
    for(i=0,j=100;i<100;i++,j--) {
        arr[i]=j;
    }
}

void display_board(void)
{
    int i,j;
    for(i=0;i<8;i++) {
        printf(GREEN"Ladder is at %2d which takes you up to %d\t\t",ladder_start[i],ladder_end[i]);
        printf(RED"Snake is at %2d which takes you down to %d\n"RES,snake_start[i],snake_end[i]);
    }
    printf("\n\n");
    for(i=0;i<10;i++) {
        printf(" _________ "); 
    }
    printf("\n");
    for(i=1;i<=100;i++) {
        display_box(i);
        if(i%10==0) {
            printf("\n");
        }
    }
}

void display_box(int i)
{
    if(arr[i-1]==usr1) {
        printf("|___"RED"%3d"RES"___|",arr[i-1]);
    } else if (arr[i-1]==usr2) {
        printf("|___"GREEN"%3d"RES"___|",arr[i-1]);
    } else {
        printf("|___%3d___|",arr[i-1]);
    }
}
