#include "queue.h"
#include <stdlib.h>
#include <stdio.h>


int initializeQ(Queue *q)
{
    if(q != NULL)
    {
        q -> count = 0;
        q -> front = NULL;
        q -> rear = NULL;
        return 1;
    }
    return 0;

}

int isEmptyQ(Queue *q)
{
	if(q->count == 0)
		return 1;
	return 0;
	
				/*if(q -> front == NULL)
					return 1;
				return 0;*/
}

int enqueueQ(Queue *q, my_typeQ data)
{
    NodeQ * temp;
    temp = malloc(sizeof(NodeQ));

    if( temp != NULL)
    {
        temp -> nodeData = data;
        if(isEmptyQ(q) == 1)
        {
            q -> front = q -> rear = temp;
        }
        else
        {
            q -> rear -> next = temp;
            q -> rear = temp;
            q -> rear -> next = NULL;
        }
        q -> count++;
        return 1;
    }
    else
        return 0;


}

my_typeQ dequeueQ(Queue *q)
{
    if(isEmptyQ(q) == 0)
    {
        NodeQ * temp;
        my_typeQ number = q -> front -> nodeData;
        temp = q -> front;
        q -> front = q -> front -> next;
        q -> count--;
        free(temp);
        return(number);
    }
    else
    {
        return MISTAKE;
    }

}

my_typeQ getFirstQ(Queue *q)
{
    if(isEmptyQ(q) == 0)
    {
        my_typeQ number = q -> front -> nodeData;
        return(number);
    }
    else
    {
        return MISTAKE;
    }
}

int isFullQ(Queue *q)
{
    NodeQ * temp;
    temp = malloc(sizeof(NodeQ));
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

void displayQ(Queue *q)
{
    if(isEmptyQ(q) == 0)
    {
        NodeQ *temp;
        temp = q -> front;
        while(temp != NULL)
        {
            printf(PRINT, temp -> nodeData);
            temp = temp -> next;
        }
        printf("\n");
    }
    else
        return;
}

void destroyQ(Queue *q)
{
    NodeQ * tmp;
    while (q->front != NULL)
    {
		tmp = q->front->next;
		free(q->front);
		q->front = tmp;
		q->count--;
		
			   /*tmp = q->front;
			   q->front = q->front->next;
			   free(tmp);*/
    }
}
