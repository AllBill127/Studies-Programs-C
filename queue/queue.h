//studento nr. 2016036

#ifndef queue_h
#define queue_h

typedef double my_typeQ;
#define PRINT "%lf " //used in display function, change if you have changed the data type

#define MISTAKE -77

#include "queueStructs.h"

int initializeQ(Queue *q); //creates an empty queue, return 1 - if success, 0 - if not

int isEmptyQ(Queue *q); // returns 1 - if empty, return 0 - if not

int enqueueQ(Queue *q, my_typeQ data); // insert an element at the end of the queue, 1 - if success, 0 - if not

my_typeQ dequeueQ(Queue *q); //Remove and return the first element of the queue, if the queue is empty returns MISTAKE

my_typeQ getFirstQ(Queue *q); //Return the first element without removing it, if the queue is empty returns MISTAKE

int isFullQ(Queue *q); //Return 1 - if full, 0 if not;

void displayQ(Queue *q); //Display the queue

void destroyQ(Queue *q);

#endif
