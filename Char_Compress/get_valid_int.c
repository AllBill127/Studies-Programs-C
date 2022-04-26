#include <stdio.h>
#include "get_valid_int.h"

int getInt(int *inputCase);

int Get_Valid_Int()
{
	int number;
	int ret;        // Input CASE; ret = 0 is BAD INPUT; ret = 1 is CORRECT INPUT
	while(ret != 1)
    {
        number = getInt(&ret);

        if(ret == 0)        // ret = 0 is BAD CASE
            printf("Bad input. Make sure you enter only whole numbers \n");
    }
	
	return number;
}

int getInt(int *inputCase)
{
    int number;
    char enter;

    while(scanf("%u%c", &number, &enter) != 2 || enter != '\n')
    {
        while(getchar() != '\n');  //Valo input'a

        return *inputCase = 0;
    }
	
	if(number < 0)
		return *inputCase = 0;

    *inputCase = 1;
    return number;
}