//Sukūrė Aleksandras Bilevičius stud. ID: 2015859
//Pirmas paprastas backtracking variantas 

#include <stdlib.h>
#include <stdio.h>


#define SIZE 8
#define B_COUNT 8
#define DOM_VALUE 5         //DOM_VALUE is value of dominated square
#define FINAL_SCORE 320     //FINAL_SCORE = 64 * DOM_VALUE


void Solve(int row, int *currentCols, int *rezCols, int count);
int IsValid(int row, int col);
void SetDominance(int row, int col, int *setCellsR, int *setCellsC);
void RemoveDominance(int *setCellsR, int *setCellsC);


//========== Global board for diminance tracking ============
int Board[SIZE][SIZE] = {0};
int steps = 0;


int main()
{
    int *rezCols, *currentCols;
    rezCols = (int*)malloc(B_COUNT * sizeof(int));
    currentCols = (int*)malloc(B_COUNT * sizeof(int));

    // set default 'empty' values
    for(int i = 0; i < SIZE; ++i)
        currentCols[i] = -1;


    Solve(0, currentCols, rezCols, 0);


    //======= Print board dominance =========
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
            printf("%d ", Board[i][j]);
        printf("\n");
    }

    for(int i = 0; i < B_COUNT; ++i)
    {
        printf("%d row %d col %d \n", i+1, i+1, rezCols[i]+1);
        Board[i][rezCols[i]] = 5;
    }

    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
            printf("%d ", Board[i][j]);
        printf("\n");
    }


    printf("steps taken %d", steps);

    free(currentCols);
    free(rezCols);

    return 0;
}


void Solve(int row, int *currentCols, int *rezCols, int count)
{
    steps++;

    int *setRows = (int*)malloc(SIZE*2 * sizeof(int));
    for(int i = 0; i < SIZE*2; ++i)
        setRows[i] = -1;               // Default empty value is -1

    int *setCols = (int*)malloc(SIZE*2 * sizeof(int));
    for(int i = 0; i < SIZE*2; ++i)
        setCols[i] = -1;


    // Count new board score//
    int boardScore = 0;
    for(int i = 0; i < SIZE; ++i)
        for(int j = 0; j < SIZE; ++j)
            boardScore += Board[i][j];


    if(count == 8)
    {
        if(boardScore == FINAL_SCORE)
            for(int i = 0; i < B_COUNT; ++i)
                rezCols[i] = currentCols[i];

        return;
    }
    else
    {
        for(int col = 0; col < SIZE; ++col)
        {
            currentCols[count] = col;
            if(IsValid(row, col))
            {
                SetDominance(row, col, setRows, setCols);
                Solve(row + 1, currentCols, rezCols, count + 1);
            }

            RemoveDominance(setRows, setCols);
            currentCols[count] = -1;
        }
    }

    free(setCols);
    free(setRows);

    return;
}

int IsValid(int row, int col)
{
    if(Board[row][col] != DOM_VALUE)
    {
        return 1;
    }
    return 0;
}

void SetDominance(int row, int col, int *setCellsR, int *setCellsC)
{
    int r, c, i = 0;
    // sets dominated squares
        r = row; c = col;
        if(Board[r][c] != DOM_VALUE)
        {
            Board[r][c] = DOM_VALUE;
            setCellsR[i] = r;
            setCellsC[i] = c;
            i++;
        }

        r = row-1; c = col-1;
        while(r >= 0 && c >= 0)
        {
            if(Board[r][c] != DOM_VALUE)
            {
                Board[r][c] = DOM_VALUE;
                setCellsR[i] = r;
                setCellsC[i] = c;
                i++;
            }
            r--; c--;
        }

        r = row+1; c = col-1;
        while(r <= SIZE-1 && c >= 0)
        {
            if(Board[r][c] != DOM_VALUE)
            {
                Board[r][c] = DOM_VALUE;
                setCellsR[i] = r;
                setCellsC[i] = c;
                i++;
            }
            r++; c--;
        }

        r = row+1; c = col+1;
        while(r <= SIZE-1 && c <= SIZE-1)
        {
            if(Board[r][c] != DOM_VALUE)
            {
                Board[r][c] = DOM_VALUE;
                setCellsR[i] = r;
                setCellsC[i] = c;
                i++;
            }
            ++r; ++c;
        }

        r = row-1; c = col+1;
        while(r >= 0 && c <= SIZE-1)
        {
            if(Board[r][c] != DOM_VALUE)
            {
                Board[r][c] = DOM_VALUE;
                setCellsR[i] = r;
                setCellsC[i] = c;
                i++;
            }
            r--; c++;
        }
}

void RemoveDominance(int *setCellsR, int *setCellsC)
{
    int row, col;
    for(int i = 0; i < SIZE*2; ++i)
    {
        row = setCellsR[i];
        col = setCellsC[i];
        if(row != -1 && col != -1)
        {
            Board[row][col] = 0;
            setCellsR[i] = -1;
            setCellsC[i] = -1;
        }
    }
}
