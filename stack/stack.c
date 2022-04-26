#include "stack.h"
#include <stdlib.h>
#include <stdio.h>


int initializeS(Stack *s)
{
    if(s != NULL)
    {
        s -> count = 0;
        s -> top = NULL;
        return 1;
    }
    return 0;

}

int isEmptyS(Stack *s)
{
	if(s->count == 0)
		return 1;
	return 0;
}

int pushS(Stack *s, my_typeS data)
{
    NodeS * temp;
    temp = malloc(sizeof(NodeS));

    if( temp != NULL)
    {
        temp -> nodeData = data;
        if(isEmptyS(s) == 1)
        {
            s -> top = temp;
        }
        else
        {
			temp -> next = s -> top;
            s -> top = temp;
        }
        s -> count++;
        return 1;
    }
    else
        return 0;


}

my_typeS popS(Stack *s)
{
    if(isEmptyS(s) == 0)
    {
        NodeS * temp;
        my_typeS number = s -> top -> nodeData;
        temp = s -> top;
        s -> top = s -> top -> next;
        s -> count--;
        free(temp);
        return(number);
    }
    else
    {
        return MISTAKE;
    }

}

my_typeS peekS(Stack *s)
{
    if(isEmptyS(s) == 0)
    {
        my_typeS number = s -> top -> nodeData;
        return(number);
    }
    else
    {
        return MISTAKE;
    }
}

int isFullS(Stack *s)
{
    NodeS * temp;
    temp = malloc(sizeof(NodeS));
    if(temp == NULL)
    {
        free(temp);
        return 1;
    }
    else
    {
        free(temp);
        return 0;
    }
}

void displayS(Stack *s)
{
    if(isEmptyS(s) == 0)
    {
        NodeS *temp;
        temp = s -> top;

		for(int i = 0; i < getSizeS(s); ++i)
        {
            printf(PRINT, temp -> nodeData);
			printf("<- ");
            temp = temp -> next;
        }
        printf("Stack top\n");
    }
    else
        return;
}

void destroyS(Stack *s)
{
    NodeS * tmp;
    while (s -> top != NULL)
    {
		if(s -> count == 0)
			break;
			
		tmp = s -> top -> next;
		free(s -> top);
		s -> top = tmp;
		s -> count--;
    }
}

int getSizeS(Stack *s)
{
	return s -> count;
}
