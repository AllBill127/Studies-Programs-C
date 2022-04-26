#ifndef stack_h
#define stack_h

typedef double my_typeS;
#define PRINT "%lf " //used in display function, change if you have changed the data type

#define MISTAKE -77

#include "stackStructs.h"

int initializeS(Stack *s); //creates an empty stack, return 1 - if success, 0 - if not

int isEmptyS(Stack *s); // returns 1 - if empty, return 0 - if not

int pushS(Stack *s, my_typeS data); // insert an element at the top of the stack, 1 - if success, 0 - if not

my_typeS popS(Stack *s); //Remove and return the top element of the stack, if the stack is empty returns MISTAKE

my_typeS peekS(Stack *s); //Return the top element without removing it, if the stack is empty returns MISTAKE

int isFullS(Stack *s); //Return 1 - if full, 0 if not;

void displayS(Stack *s); //Display the stack

void destroyS(Stack *s);

int getSizeS(Stack *s);

#endif
