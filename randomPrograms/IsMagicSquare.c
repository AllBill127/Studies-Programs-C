/*15. Įvesti sveiką skaičių N. Įvesti N*N sveikų skaičių, kurie sudarys kvadratinę matricą.
Patikrinti, ar ji yra magiškas kvadratas, t.y. visų stulpelių, eilučių ir įstrižainių sumos
vienodos.*/

#include <stdio.h>


//====== Function declarations ======
int Input(int *inputCase);
int isMagic(int **A, int size);


//====== Main ======
int main()
{
    printf("Input the size of a square matrix and then input whole numbers into it: \n");

    //====== Input validation =========
    int size;
    while (1)
    {
        int ret;

        size = Input(&ret);

        if(ret == 0)        // ret = 0 is BAD CASE
            printf("Bad input. Make sure you enter only whole numbers \n");
        else
            break;
    }


    //======= Matrix creation =========
    int Matrix[size][size];

    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
        {
            //====== Input validation =======
            int num;
            while(1)
            {
                int ret;

                num = Input(&ret);

                if(ret == 0)        // ret = 0 is BAD CASE
                    printf("Bad input. Make sure you enter only whole numbers \n");
                else
                    break;
            }

            Matrix[j][i] = num;
        }


    //======== Output =========
    printf("\nYou created a matrix: \n");

    for(int i = 0; i < size; ++i)
    {
        printf("| ");
        for(int j = 0; j < size; ++j)
        {
            printf("%d ", Matrix[j][i]);
        }
        printf("| \n");
    }

    int *M[size];       // Creates a pointer array with matrix first element adresses
    for(int i = 0; i < size; ++i) M[i] = Matrix[i];

    if(isMagic(M, size) == 1)
        printf("Created matrix is magical \n");
    else
        printf("Created matrix is not magical \n");

    return 0;
}


//====== Functions ======
int Input(int *inputCase)
{
    int number;
    char enter;

    while(scanf("%d%c", &number, &enter) != 2 || enter != '\n')
    {
        while(getchar() != '\n');       //Valo input'a

        return *inputCase = 0;
    }

    *inputCase = 1;
    return number;
}

int isMagic(int **A, int size)
{
    int sum, temp1 = 0;

    for(int i = 0; i < size; ++i)       // Checks if Sum of columns match
    {
        sum = 0;
        for(int j = 0; j < size; ++j)
            sum += A[i][j];

        if(sum != temp1 && i != 0)
            return 0;
        else
            temp1 = sum;
    }

    for(int i = 0; i < size; ++i)       // Checks if Sum of rows match column sums
    {
        sum = 0;
        for(int j = 0; j < size; ++j)
            sum += A[j][i];

        if(sum != temp1)
            return 0;
    }

    sum = 0;
    for(int i = 0; i < size; ++i)       // Checks if Sum of first diagonal matches column sums
        sum += A[i][i];

    if(sum != temp1)
        return 0;

    sum = 0;
    for(int i = 0; i < size; ++i)       // Checks if Sum of second diagonal matches column sums
        sum += A[i][size-i-1];

    if(sum != temp1)
        return 0;


    return 1;       // If every sum matched up returns 1
}