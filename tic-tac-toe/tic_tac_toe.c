/* @File   	: tic_tac_toe.c
 * @Auhor  	: Aman Kumar
 * @Brief   	: This program simulates the tic tac toe game.
 */ 

/*header file*/
#include<stdio.h>

/*macros for colored output on std-out*/
#define RED "\x1b[01;31m"
#define GREEN "\x1b[01;32m"
#define RESET "\x1b[01;0m"
#define YELLOW "\x1b[01;33m"
#define CYAN "\x1b[01;36m"

/*global variables*/
int flag=0,cpu;
char board_array_value[9]={32,32,32,32,32,32,32,32,32};
int X_index[5]={0},O_index[4]={0};/*to store options entered by both player*/

/*function declarations*/
void display_result(int);
void display_board(void);
int fill_by_cpu(void);

int main()
{
    int box_num[9],box_index;
    int i,j,a,b,c;/*variables for iteration*/
    int m=0,n=0;/*to count indices of X_index[] and O_index[], respectively*/
    system("clear");/*clears screen*/

    printf(GREEN"\n\n!!..WELCOME TO TIC-TAC-TOE GAME..!!\n\n"RESET);
    printf(CYAN"INFORMATION ABOUT THE GAME:\n");
    printf(GREEN"Tic-tac-toe is a game for two players, "YELLOW"X"GREEN" and "YELLOW"O"GREEN",\nwho take turns marking the spaces in a 3×3 grid.\n");
    printf("The player who succeeds in placing three respective marks in a horizontal,\nvertical, or diagonal,\n");
    printf("wins the game.\n"RESET);
    printf(GREEN"\n    >>--GAME-BOARD--<<\n\n"RESET);
    display_board();/*displays empty grid*/
    printf(CYAN"\n\nHit return key to start the game: "RESET);
    getchar();

    /*to select game mode(two player/CPU)*/
    system("clear");
    printf(GREEN"\n    >>--GAME-BOARD--<<\n\n"RESET);
    display_board();/*displays empty grid*/
    REPEAT:
    printf(RED"\n\n<<<GAME MODE>>>\n\n");
    printf(GREEN"Press 0:"CYAN" Two Player Mode.");
    printf(GREEN"\nPress 1:"CYAN" CPU Mode.");
    printf(YELLOW"\nChoose Game Mode: "RESET);
    if(scanf(" %d",&cpu) < 0);
    if(cpu!=1 && cpu!=0) {
        goto REPEAT;
    }

    /*start game play*/
    system("clear");
    printf(GREEN"\n    >>--GAME-BOARD--<<\n\n"RESET);
    display_board();/*displays empty grid*/

    for(i=0;i<9;i++) {
        back:
        if(i%2==0) {
againx:
	    	printf(YELLOW"\nPlayer-X:"CYAN" Enter box number(1~9) to input mark-X: "RESET);
            if(scanf(" %d",&box_num[i]) < 0);
			if(box_num[i]<0 || box_num[i]>9) goto againx;
            X_index[m]=box_num[i];
            m++;
        } else {
            if(cpu) {
                box_num[i]=fill_by_cpu();
                O_index[n]=box_num[i];
                n++;
                goto LOGIC;
            } else {
again0:
				printf(CYAN"\nPlayer-O:"YELLOW" Enter box number(1~9) to input mark-O: "RESET);
                if(scanf(" %d",&box_num[i]) < 0);
				if(box_num[i]<0 || box_num[i]>9) goto again0;
            }
        }
	
        /*To check the correct index-value(1~9)*/
        if(box_num[i]>9 || box_num[i]<1) {
            printf(RED"\nInvalid box number..\n"RESET);
            goto back;/*if incorrect, then go back*/
        }
	
        /*To check for non-repeatation of the box index-value*/
        for(j=0;j<i;j++) {
            if (box_num[i]==box_num[j]) {
            printf(RED"\nBox number already used..\n"RESET);
            goto back;/*if index already used, then go back*/
            }
        }
        LOGIC:
        /*setting marks to game_board for corresponding player*/
        box_index=box_num[i]-1;
        if (i%2==0) {
            board_array_value[box_index]='X';
        } else {
            board_array_value[box_index]='O';
        }

        system("clear");/*clear the screen*/	
        printf(GREEN"\n    >>--GAME-BOARD--<<\n\n"RESET);
        display_board();/*displays the current status of the game*/

        /*checking for rows to match*/
        for(a=0,b=1,c=2;a<=6;a+=3,b+=3,c+=3) {
            if ((board_array_value[a]==board_array_value[b]) && \
                ( board_array_value[a]==board_array_value[c])) {
                display_result(a);
                if (flag==1) {
                    return; 
                }
            }    
        }

        /*checking for columns to match*/
        for(a=0,b=3,c=6;a<3;a++,b++,c++) {
            if ((board_array_value[a]==board_array_value[b]) && \
                ( board_array_value[a]==board_array_value[c])) {
                display_result(a);
                if (flag==1) {
                    return; 
                }
            }    
        }

        /*checking for diagonals to match*/
        if (((board_array_value[0]==board_array_value[4]) && (board_array_value[0]==board_array_value[8])) || \
            ((board_array_value[2]==board_array_value[4]) && (board_array_value[2]==board_array_value[6]))) {
            display_result(4);
            if (flag==1) {
                return; 
            }
        }

    }
    printf(YELLOW"\nOOPS..!! MATCH DRAW..!!\n\n"RESET);
} 

/* @function    : check the board array value and displays the result
 * @param       : index value
 * @returns     : void
 * @description : function checks the marks set by players and depend 
 *                on the marks as per players, prints the result of 
 *                the game.
 */ 
void display_result(int index)
{
    if(board_array_value[index]=='X') {
        printf(GREEN"\nCONGRATULATIONS..!! PLAYER-X HAS WON THE GAME..!!\n\n"RESET);
        flag=1;
    } else if (board_array_value[index]=='O') {
        if(cpu) printf(GREEN"CPU HAS WON THE GAME..!!\n\n"RESET);
        else printf(GREEN"\nCONGRATULATIONS..!! PLAYER-O HAS WON THE GAME..!!\n\n"RESET);
        flag=1;
    }
}

/* @function    : display the board_grid
 * @param       : void
 * @returns     : void
 * @description : function prints the board view of the current status
 *                of the game.
 */ 

void display_board(void)
{
    int i,j;
    for(i=0,j=0;i<2;i++,j+=2) {/*prints first two rows*/
        printf(RED"\t %c "RESET"|"RED" %c "RESET"|"RED" %c \n"RESET"\t___|___|___\n",board_array_value[i+j],board_array_value[i+j+1],board_array_value[i+j+2]);
    }
    /*prints last row*/
    printf(RED"\t %c "RESET"|"RED" %c "RESET"|"RED" %c \n"RESET"\t   |   |   \n",board_array_value[6],board_array_value[7],board_array_value[8]);
}

/* @function    : selects the appropriate index
 * @param       : void
 * @returns     : index number
 * @description : function selects automatically an index which is appropriate
 *                as per the current game board and returns the index.
 */ 
int fill_by_cpu(void)
{
    int index[]={1,2,3,4,5,6,7,8,9};/*for CPU to select option and return*/
    int i,j,j_prev=-1,k=4;/*variables for iteration of loops*/
    static int cnt=0;/*to count CPU chance number*/
    int rest[4];/*array to store rest of the options available for CPU to choose*/

/*CPU's first chance*/
    if(cnt==0) {
        if(X_index[0]!=5) {/*if user has not filled the middle one*/
            return index[4+cnt++];/*CPU choosing middle one*/
        } 
        /*if user has filled the middle one*/
        while(1) {
            srand(getpid()+rand());
            i=rand()%9;
            if(i!=4 && i%2==0) {/*CPU choosing any of the corner randomly*/
                return index[i+cnt++];
            }
        }        
    }

/*CPU's second chance*/ 
    if(cnt==1) {
        if(X_index[0]==5) {/*if middle one is choosed by user*/
            if(O_index[0]!=index[9-X_index[1]]) {
                /*CPU blocks the other end in the same line*/
                return index[9-X_index[cnt++]];
            }
            /*CPU sets its own match*/
            if(board_array_value[6]==32 && O_index[0]!=6 && O_index[0]!=2) {
                return index[5+cnt++];
            }
            if(O_index[0]!=6) {
                return index[-1+cnt++];
            }
            return index[1+cnt++];

        }
        if(X_index[0]%2!=0 && X_index[1]%2!=0) {/*if both options entered by user are odd*/
            /*blocking the user*/
            if((X_index[0]==1 || X_index[1]==1) && (X_index[0]==3 || X_index[1]==3)) {
                return index[cnt++];
            }
            if((X_index[0]==1 || X_index[1]==1) && (X_index[0]==7 || X_index[1]==7)) {
                return index[2+cnt++];
            }
            if((X_index[0]==1 || X_index[1]==1) && (X_index[0]==9 || X_index[1]==9)) {
                return index[cnt++];
            }
            if((X_index[0]==3 || X_index[1]==3) && (X_index[0]==9 || X_index[1]==9)) {
                return index[4+cnt++];
            }
            if((X_index[0]==3 || X_index[1]==3) && (X_index[0]==7 || X_index[1]==7)) {
                return index[cnt++];
            }
            if((X_index[0]==7 || X_index[1]==7) && (X_index[0]==9 || X_index[1]==9)) {
                return index[6+cnt++];
            }
        }

        if(X_index[0]%2==0 && X_index[1]%2==0) {/*if both options are even*/
            /*blocking the user*/
            if((X_index[0]==2 || X_index[1]==2) && (X_index[0]==4 || X_index[1]==4)) {
                return index[-1+cnt++];
            }
            if((X_index[0]==2 || X_index[1]==2) && (X_index[0]==6 || X_index[1]==6)) {
                return index[1+cnt++];
            }
            if((X_index[0]==4 || X_index[1]==4) && (X_index[0]==6 || X_index[1]==6)) {
                return index[1+cnt++];
            }
            if((X_index[0]==8 || X_index[1]==8) && (X_index[0]==6 || X_index[1]==6)) {
                return index[7+cnt++];
            }
            if((X_index[0]==8 || X_index[1]==8) && (X_index[0]==4 || X_index[1]==4)) {
                return index[5+cnt++];
            }
            if((X_index[0]==2 || X_index[1]==2) && (X_index[0]==8 || X_index[1]==8)) {
                return index[1+cnt++];
            }
        }
        if(X_index[0]%2!=0 && X_index[1]%2==0) {/*if 1st option is odd and 2nd is even*/
            /*blocking the user*/
            if(((X_index[0]==1 && X_index[1]==2) || (X_index[0]==1 && X_index[1]==4)) || \
              ((X_index[0]==9 && X_index[1]==2) || (X_index[0]==9 && X_index[1]==4))) {
                if(X_index[1]==2) return index[1+cnt++]; return index[5+cnt++];
            }
            if(((X_index[0]==1 && X_index[1]==6) || (X_index[0]==1 && X_index[1]==8)) || \
               ((X_index[0]==9 && X_index[1]==6) || (X_index[0]==9 && X_index[1]==8))) {
                if(X_index[1]==6) return index[1+cnt++]; return index[5+cnt++];
            }
            if(((X_index[0]==3 && X_index[1]==2) || (X_index[0]==3 && X_index[1]==6)) || \
              ((X_index[0]==7 && X_index[1]==2) || (X_index[0]==7 && X_index[1]==6))) {
                if(X_index[1]==2) return index[-1+cnt++]; return index[7+cnt++];
            }
            if(((X_index[0]==3 && X_index[1]==4) || (X_index[0]==3 && X_index[1]==8)) || \
              ((X_index[0]==7 && X_index[1]==4) || (X_index[0]==7 && X_index[1]==8))) {
                if(X_index[1]==4) return index[-1+cnt++]; return index[7+cnt++];
            }
        }
        if(X_index[1]%2!=0 && X_index[0]%2==0) {/*if 1st option is even and 2nd is odd*/
            /*blocking the user*/
            if(((X_index[1]==1 && X_index[0]==2) || (X_index[1]==1 && X_index[0]==4)) || \
              ((X_index[1]==9 && X_index[0]==2) || (X_index[1]==9 && X_index[0]==4))) {
                if(X_index[0]==2) return index[1+cnt++]; return index[5+cnt++];
            }
            if(((X_index[1]==1 && X_index[0]==6) || (X_index[1]==1 && X_index[0]==8)) || \
               ((X_index[1]==9 && X_index[0]==6) || (X_index[1]==9 && X_index[0]==8))) {
                if(X_index[0]==6) return index[1+cnt++]; return index[5+cnt++];
            }
            if(((X_index[1]==3 && X_index[0]==2) || (X_index[1]==3 && X_index[0]==6)) || \
              ((X_index[1]==7 && X_index[0]==2) || (X_index[1]==7 && X_index[0]==6))) {
                if(X_index[0]==2) return index[-1+cnt++]; return index[7+cnt++];
            }
            if(((X_index[1]==3 && X_index[0]==4) || (X_index[1]==3 && X_index[0]==8)) || \
              ((X_index[1]==7 && X_index[0]==4) || (X_index[1]==7 && X_index[0]==8))) {
                if(X_index[0]==4) return index[-1+cnt++]; return index[7+cnt++];
            }
        }

    }

/*CPU's third/last chance*/
    if(cnt>=2) {
        for(i=0,j=0;i<9;i++) {/*checking untouched options between (1~9)*/
            if(board_array_value[i]==32) { 
                rest[j]=i+1; /*storing remaining options in array rest*/
                j++;
            }               
        }

        if(cnt==3) k=2;/*if CPU last chance*/

        /*row-O*/
        for(i=0;i<k;i++) {/*checking for all rows, if CPU can win*/
            for(j=3;j<=9;j_prev=j,j+=3) {
                if(rest[i]<=j && rest[i]>j_prev) {
                    if((board_array_value[j-3]=='O' && board_array_value[j-2]=='O') || \
                       (board_array_value[j-3]=='O' && board_array_value[j-1]=='O') || \
                       (board_array_value[j-2]=='O' && board_array_value[j-1]=='O')) {
                        cnt++;
                        return rest[i];
                    }
                }
            }
        }

        /*column-O*/
        for(i=0;i<k;i++) {/*checking for all columns, if CPU can win*/
            for(j=0;j<=2;j++) {
                if(rest[i]==j+1 || rest[i]==j+4 || rest[i]==j+7 ) {
                    if((board_array_value[j]=='O' && board_array_value[j+3]=='O') || \
                       (board_array_value[j]=='O' && board_array_value[j+6]=='O') || \
                       (board_array_value[j+3]=='O' && board_array_value[j+6]=='O')) {
                        cnt++;
                        return rest[i];
                    }
                }
            }
        }

        /*diagonal-O*/
        if(board_array_value[4]=='O') {/*checking for diagonals, if CPU can win*/
            if(board_array_value[0]=='O') {
                if(board_array_value[8]==32) {
                    cnt++;
                    return index[8];
                }
            }
            if(board_array_value[2]=='O') {
                if(board_array_value[6]==32) {
                    cnt++;
                    return index[6];
                }
            }
            if(board_array_value[6]=='O') {
                if(board_array_value[2]==32) {
                    cnt++;
                    return index[2];
                }  
            }
            if(board_array_value[8]=='O') {
                if(board_array_value[0]==32)  {
                    cnt++;
                    return index[0];
                }
            }
        }
 
        /*row-X*/
        for(i=0;i<k;i++) {/*checking for rows, if user has a chance to win then block the option*/
            for(j=3;j<=9;j_prev=j,j+=3) {
                if(rest[i]<=j && rest[i]>j_prev) {
                    if((board_array_value[j-3]=='X' && board_array_value[j-2]=='X') || \
                       (board_array_value[j-3]=='X' && board_array_value[j-1]=='X') || \
                       (board_array_value[j-2]=='X' && board_array_value[j-1]=='X')) {
                        cnt++;
                        return rest[i];
                    }
                } 
           }
        }

        /*column-X*/
        for(i=0;i<k;i++) {/*checking for columns, if user has a chance to win then block the option*/
            for(j=0;j<=2;j++) {
                if(rest[i]==j+1 || rest[i]==j+4 || rest[i]==j+7 ) {
                    if((board_array_value[j]=='X' && board_array_value[j+3]=='X') || \
                       (board_array_value[j]=='X' && board_array_value[j+6]=='X') || \
                       (board_array_value[j+3]=='X' && board_array_value[j+6]=='X')) {
                        cnt++;
                        return rest[i];
                    }
                }
            }
        }

        /*diagonal-X*/
        /*checking for diagonals, if user has a chance to win then block the option*/
        if(board_array_value[4]=='X') {
            if(board_array_value[0]=='X') {
                if(board_array_value[8]==32) {
                    cnt++;
                    return index[8];
                }
            }
            if(board_array_value[2]=='X') {
                if(board_array_value[6]==32) {
                    cnt++;
                    return index[6];
                }
            }
            if(board_array_value[6]=='X') {
                if(board_array_value[2]==32) {
                    cnt++;
                    return index[2];
                }
            }
            if(board_array_value[8]=='X') {
                if(board_array_value[0]==32) {
                    cnt++;
                    return index[0];
                }
            }
        }
        /*if none of the above condition matches*/
        return rest[0];/*this can happen only at the CPU's last chance*/
    }
    
}/*main*/

