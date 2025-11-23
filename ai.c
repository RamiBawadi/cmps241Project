#include <stdio.h>
#include "game.h"
#include "ai.h"

#include <stdlib.h>
#include <time.h>
#include <limits.h>

// pruning means cuttiong off branches of the game that we dont need to explore because we already know they cant lead to a better result

bool getisPlayerB_Ai()
{
    bool validIN = false;

    while(!validIN)
    {
        printf("\n");
        int res;
        printf("Do you want to play agaist:\n\t0 : AI\n\t1 : Another Player\n\nPlease select a number (0,1) : ");

        if(scanf("%d", &res) != 1)
        {
            printf("Invalid number, please try again.\n");
            clear_input_buffer();
            continue;
        }

        if(res == 0)
        {
            validIN = true;
            return true;
        }

        else if (res == 1)
        
        {
            validIN = true;
            return false;

        }

        else
        {
            printf("Invalid number, please try again.\n");
            clear_input_buffer();
        }
    }

    printf("Unreach");
    return false;
}


#define MAX_DEPTH 6
#define WIN_SCORE 1000000
static const char BOT = '#';


static char detectHumanSymbol(char board[ROWS][COLS])
{
    for(int r = 0; r < ROWS; r++)
        for(int c = 0; c < COLS; c++)
        {
            char cell = board[r][c];
            if(cell != '*' && cell != BOT && cell != '\0')
                return cell;
        }

    return 'A';
}


// idea: Creates a temporary copy of the board used by Minimax for simulation without modifying the real board
static void copyBoard(char dst[ROWS][COLS], char src[ROWS][COLS])
{
    for(int r = 0; r < ROWS; r++)
        for(int c = 0; c < COLS; c++)
            dst[r][c] = src[r][c];
}


// idea: Scores a 4-cell sequence (horizontal/vertical/diagonal) to estimate the strength of the board

static int scoreWindow(char w[4], char me, char opp)
{
    int meCount = 0, oppCount = 0, empty = 0;

    for(int i = 0; i < 4; i++)
    {
        if (w[i] == me) meCount++;
        else if (w[i] == opp) oppCount++;
        else if (w[i] == '*') empty++;

    }

    if (meCount == 4) return 100000;   // immediate win
    if (meCount == 3 && empty == 1) return 1000;
    if (meCount == 2 && empty == 2) return 50;

    if (oppCount == 3 && empty == 1) return -900; // must block
    if (oppCount == 4) return -100000;

    return 0;
}


// idea: Heuristic evaluation of the board. Higher score means better position for the AI. Used by Minimax
static int evaluateBoard(char b[ROWS][COLS], char me, char opp)
{
    int score = 0;
    int center = COLS / 2;

    for(int r = 0; r < ROWS; r++)
        if (b[r][center] == me) score += 6;

    // Horizontal
    for(int r = 0; r < ROWS; r++)
        for (int c = 0; c <= COLS - 4; c++)
        {
            char w[4] = {b[r][c], b[r][c+1], b[r][c+2], b[r][c+3]};
            score += scoreWindow(w, me, opp);
        }

    // Vertical
    for(int c = 0; c < COLS; c++)
        for (int r = 0; r <= ROWS - 4; r++)
        {
            char w[4] = {b[r][c], b[r+1][c], b[r+2][c], b[r+3][c]};
            score += scoreWindow(w, me, opp);
        }

    // Diagonal down-right
    for(int r = 0; r <= ROWS - 4; r++)
        for(int c = 0; c <= COLS - 4; c++)
        {
            char w[4] = {b[r][c], b[r+1][c+1], b[r+2][c+2], b[r+3][c+3]};
            score += scoreWindow(w, me, opp);
        }

    // Diagonal up-right
    for(int r = 3; r < ROWS; r++)
        for(int c = 0; c <= COLS - 4; c++)
        {
            char w[4] = {b[r][c], b[r-1][c+1], b[r-2][c+2], b[r-3][c+3]};
            score += scoreWindow(w, me, opp);
        }

    return score;
}

// Move ordering for better pruning
static const int ORDER[COLS] = {3,2,4,1,5,0,6};



// idea: Core AI algorithm. Simulates game states up to MAX_DEPTH using Minimax with Alpha–Beta pruning
static int minimax(char b[ROWS][COLS], int depth, int alpha, int beta,
                   int maximize, char me, char opp, int *bestColOut)
{
    if (checkWinCondition(b))
    {
        int eval = evaluateBoard(b, me, opp);
        if (eval > 0) return WIN_SCORE - (MAX_DEPTH - depth);
        if (eval < 0) return - WIN_SCORE + (MAX_DEPTH - depth);
        return 0;
    }

    if (depth == 0)
        return evaluateBoard(b, me, opp);

    int hasMove = 0;

    for (int c = 0; c < COLS; c++)
        if (getAvailbleY(b, c) != -1) { hasMove = 1; break; }
    if (!hasMove) return 0;

    if (maximize)
    {
        int bestScore = -INT_MAX, bestCol = -1;

        for (int i = 0; i < COLS; i++)
        {
            int col = ORDER[i];
            int y = getAvailbleY(b, col);
            if (y == -1) continue;

            char tmp[ROWS][COLS];
            copyBoard(tmp, b);
            insertAt(tmp, y, col, me);

            int score = minimax(tmp, depth-1, alpha, beta, 0, me, opp, NULL);

            if (score > bestScore) { bestScore = score; bestCol = col; }
            if (bestScore > alpha) alpha = bestScore;
            if (alpha >= beta) break;
        }

        if (bestColOut) *bestColOut = bestCol;
        return bestScore;
    }

    else
    {
        int bestScore = INT_MAX, bestCol = -1;

        for (int i = 0; i < COLS; i++)
        {
            int col = ORDER[i];
            int y = getAvailbleY(b, col);
            if (y == -1) continue;

            char tmp[ROWS][COLS];
            copyBoard(tmp, b);
            insertAt(tmp, y, col, opp);

            int score = minimax(tmp, depth-1, alpha, beta, 1, me, opp, NULL);

            if (score < bestScore) { bestScore = score; bestCol = col; }
            if (bestScore < beta) beta = bestScore;
            if (alpha >= beta) break;
        }

        if (bestColOut) *bestColOut = bestCol;
        return bestScore;
    }
}


// idea: hard ai move function This is the function used by the game to have the AI automatically choose its move
void ValidateInput_Ai(char board[ROWS][COLS])
{
    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }

    char me = BOT;
    char opp = detectHumanSymbol(board);

    int bestCol = -1;

    minimax(board, MAX_DEPTH, -INT_MAX, INT_MAX, 1, me, opp, &bestCol);

    if (bestCol == -1)
    {
        for (int c = 0; c < COLS; c++)
        {
            int y = getAvailbleY(board, c);
            if (y != -1)
            {
                insertAt(board, y, c, me);
                return;
            }
        }
    }

    else
    {
        int y = getAvailbleY(board, bestCol);
        insertAt(board, y, bestCol, me);
    }
}



void reportStrategyComplexity(void)
{
    int rows = ROWS;
    int cols = COLS;
    int depth = MAX_DEPTH;

    // At each minimax node we:
    // 1) copy board  -> O(rows*cols)
    // 2) insert move -> O(1)
    // 3) maybe checkWin or evaluate -> O(rows*cols)
    
   
    long approxCopyCost  = rows * cols;          // copyBoard
    long approxEvalCost  = 12L * rows * cols;    
    long approxNodeCost  = approxCopyCost + approxEvalCost;

    // Worst case minimax explores ~ cols^depth nodes
    // Best case alpha-beta explores ~ cols^(depth/2) nodes
    // We'll compute integer powers safely:

    long worstNodes = 1;
    for(int i = 0; i < depth; i++)
        worstNodes *= cols;

    long bestNodes = 1;
    for(int i = 0; i < (depth / 2); i++)
        bestNodes *= cols;

    //Tot work
    long worstWork = worstNodes * approxNodeCost;
    long bestWork  = bestNodes  * approxNodeCost;

    printf("\n========== Hard Bot Complexity Report ==========\n");
    printf("Board Size   : %d rows x %d columns\n", rows, cols);
    printf("Search Depth : %d plies (moves ahead)\n", depth);
    printf("Branching    : up to %d moves per node\n\n", cols);

    printf("Per-node cost (copy + eval/check) ≈ %ld steps\n\n", approxNodeCost);

    printf("Worst-case (no pruning):\n");
    printf("  Nodes explored ≈ C^D = %ld\n", worstNodes);
    printf("  Time Complexity ≈ O(C^D * R*C)\n");
    printf("  Estimated work ≈ %ld steps\n\n", worstWork);

    printf("Best-case (ideal alpha–beta pruning):\n");
    printf("  Nodes explored ≈ C^(D/2) = %ld\n", bestNodes);
    printf("  Time Complexity ≈ O(C^(D/2) * R*C)\n");
    printf("  Estimated work ≈ %ld steps\n", bestWork);

    printf("=========================================================\n\n");
}

