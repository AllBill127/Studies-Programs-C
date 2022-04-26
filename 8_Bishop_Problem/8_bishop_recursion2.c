//Sukūrė Aleksandras Bilevičius stud. ID: 2015859
//Antras greitesnis variantas 

#include <stdlib.h>
#include <stdio.h>


#define SIZE 8
#define B_COUNT 8
#define EMPTY -1            // EMPTY is empty array value
#define DOM_VALUE 5         //DOM_VALUE is value of dominated square
#define FINAL_SCORE 320     //FINAL_SCORE = 64 * DOM_VALUE


void Solve(int row, int *currentCols, int *rezCols, int count, int *rez1);
int IsValid(int row, int col);
void SetDominance(int row, int col, int *setCellsR, int *setCellsC);
void RemoveDominance(int *setCellsR, int *setCellsC);


//========== Global board for diminance tracking ============
int Board[SIZE][SIZE] = {0};
int steps = 0;


int main()
{
    int *rezCols, *currentCols, *rez1;           //rez1 = 1 when first solution is found
    rezCols = (int*)malloc(B_COUNT * sizeof(int));
    currentCols = (int*)malloc(B_COUNT * sizeof(int));
    int a = 0; rez1 = &a;

    // set default 'empty' values
    for(int i = 0; i < SIZE; ++i)
        currentCols[i] = EMPTY;


    Solve(0, currentCols, rezCols, 0, rez1);

    //======= Print board dominance =========
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
            printf("%d ", Board[i][j]);
        printf("\n");
    }

    //======= Clear board, set solution positions and print solved board ========
    for(int i = 0; i < SIZE; ++i)
        for(int j = 0; j < SIZE; ++j)
            Board[i][j] = 0;

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


void Solve(int row, int *currentCols, int *rezCols, int count, int *rez1)
{
    steps++;

    // saves cells set by setDominance
    int *setRows = (int*)malloc(SIZE*2 * sizeof(int));
    for(int i = 0; i < SIZE*2; ++i)
        setRows[i] = EMPTY;               // Default 'empty' value is -1

    int *setCols = (int*)malloc(SIZE*2 * sizeof(int));
    for(int i = 0; i < SIZE*2; ++i)
        setCols[i] = EMPTY;               // Default 'empty' value is -1


    // Count new board score// Dominated square = 5 => full dominance = 320
    int boardScore = 0;
    for(int i = 0; i < SIZE; ++i)
        for(int j = 0; j < SIZE; ++j)
            boardScore += Board[i][j];


    if(count == 8)
    {
        if(boardScore == FINAL_SCORE)
        {
            for(int i = 0; i < B_COUNT; ++i)
                rezCols[i] = currentCols[i];

            *rez1 = 1;
        }

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
                Solve(row + 1, currentCols, rezCols, count + 1, rez1);
            }

            if(*rez1)               ///================== skips further result search ===================
                return;

            RemoveDominance(setRows, setCols);
            currentCols[count] = EMPTY;
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

    // sets Bishops square
    r = row; c = col;
    if(Board[r][c] != DOM_VALUE)
    {
        Board[r][c] = DOM_VALUE;
        setCellsR[i] = r;
        setCellsC[i] = c;
        i++;
    }

    // sets Bishops dominated squares
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
        if(row != EMPTY && col != EMPTY)
        {
            Board[row][col] = 0;
            setCellsR[i] = EMPTY;
            setCellsC[i] = EMPTY;
        }
    }
}
