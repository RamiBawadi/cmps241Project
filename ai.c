#include <stdio.h>
#include "game.h"
#include "ai.h"


#include <stdlib.h> // For rand()
#include <time.h>   // For time() (from stack overflow)

bool getisPlayerB_Ai(){
    bool validIN = false;

    while (!validIN) { 
        printf("\n");
        int res;
        printf("Do you want to play agaist:\n\t0 : AI\n\t1 : Another Player\n\nPlease select a number (0,1) : ");
        if (scanf("%d", &res) != 1) {
            printf("Invalid number, please try again.\n");
            clear_input_buffer();
            continue;
        }

        if(res == 0){
            validIN = true;
            return false;
        }
        else if(res == 1){
            validIN = true;
            return true;
        }
        else{
            printf("Invalid number, please try again.\n");
            clear_input_buffer();
            continue;
        }
    }
}

void ValidateInput_Ai(char board[ROWS][COLS]){
    int col, y;
    bool validIN = false;
    srand(time(NULL)); //set the time as seed for the random number gen;

    while (!validIN) { 
        //choose a colum
        int randomInt = rand(); //choose a random number (very big range)
        int max = 6;
        int min = 0;
        col = (randomInt % (max - min + 1)) + min;
       
        if (col < 0 || col >= COLS) {
            //out of range, try again
            continue;
        }

        y = getAvailbleY(board, col);
        if (y == -1) {
            //collum full try again
        } else {
            validIN = true;
            insertAt(board, y, col, '#');
        }
    }
}
