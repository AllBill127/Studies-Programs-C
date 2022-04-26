/*Sudaryti vienpusį sąrašą (Linked List). Parašyti procedūrą, kuri išmeta numeriu nurodytą elementą. Jeigu
tokio elemento nėra, turi būti išvestas atitinkamas pranešimas.*/

#include <stdio.h>
#include <stdlib.h>


//====== Structs ======
struct element												// makes an struct for one sided array elements
{
	int var;												// variable to store the elemnts' value
	struct element *nextPtr;								// pointer to the next array element
};

typedef struct element Element;								// shortens the struct element type to just Element


//====== Function declarations ======
void makeList(Element **beginList);
int getInt(int *inputCase);
void printList(Element **beginList);
void delIndexElement(Element **beginList);
void freeMemory(Element *headNode);


int main()
{
	Element *firstPtr = NULL;								// makes and stores a pointer to the first element

	makeList(&firstPtr);

	printList(&firstPtr);

	delIndexElement(&firstPtr);

	printList(&firstPtr);

	freeMemory(firstPtr);

	return 0;
}


//====== Functions ======
void makeList (Element **beginList)
{
	Element *temp = *beginList;								// for the first time temp is pointing to the first element of the list
	Element *elementBefore = NULL;							// for the first time previous element has an empty pointer

	printf("Enter list elements (whole numbers) one by one. To finish the input enter 0\n");

	//=============== Geting a number from user ============
	int inputCase, number;

	while(1)												// input loop continues until 0 is entered
	{
		while(inputCase != 1)
		{
			number = getInt(&inputCase);

			if(inputCase == 0)        // inputCase = 0 is BAD CASE
				printf("Bad input. Make sure you enter only whole numbers \n");
		}
		inputCase = 0;

		if(number == 0)										// if inputed number is 0 the break while loop and stop input
			break;

	//=============== Adding element to the linked list ============
		temp = (Element*)malloc(sizeof(Element));			// Allocates memory for a new struct sized element and assigns temp a pointer to it
		temp->var = number;									// saves number value to current element which temp is pointing to

		if(*beginList == NULL)								// If first element is NULL then...
			*beginList = temp;								// pointer to begining of list is changed to allocated element pointed to by temp
		else
			elementBefore->nextPtr = temp;					// if at least one element (the begining) exists then previous elements pointer to next element is assigned to currently allocated element (temp)

		temp->nextPtr = NULL;								// current elements pointer to next element is NULL
		elementBefore = temp;								// changes previous element to current (pointed to by temp) and proceeds to read new data and create next element
	}
}


int getInt(int *inputCase)
{
    int number;
    char enter;

    while(scanf("%9d%c", &number, &enter) != 2 || enter != '\n')
    {
		while(getchar() != '\n');  // Clears the incorrect input stream

		return *inputCase = 0;
    }

    *inputCase = 1;
    return number;
}


void printList(Element **beginList)
{
	Element *temp = *beginList;
	int number, count = 1;

	printf("\n================ Linked list elements =============\n");
	do
	{
		number = temp->var;
		printf("Element %d is: %d\n", count, number);
		count++;
		temp = temp->nextPtr;
	}
	while(temp->nextPtr != NULL);

	number = temp->var;
	printf("Element %d is: %d\n", count, number);
}


void delIndexElement(Element **beginList)
{
	Element *temp = *beginList;
	int count = 0, number;

	Retry:
	printf("\nEnter which element would you like to delete\n");

	//=============== Geting a number from user ============
	int index, inputCase = 0;

	while(inputCase != 1)
	{
		index = getInt(&inputCase);

		if(inputCase == 0)        // inputCase = 0 is BAD CASE
			printf("Bad input. Make sure you enter only whole numbers \n");
	}

	//=============== Deleting element by given index ============
	if(index == 1)
	{
		number = temp->var;
		*beginList = temp->nextPtr;
		free(temp);
	}
	else
	{
		//=============== Go to element before the indexed one ============
		while(index != count+2)
		{
			if(index <= 0 || temp->nextPtr->nextPtr == NULL)			// checks if index is not less than 0 and...
			{															// if next element is pointing to element with a NULL pointer then print error
				printf("Chosen element exceeds the size of the list");
				count = 0;
				inputCase = 0;
				temp = *beginList;
				goto Retry;
			}

			count++;
			temp = temp->nextPtr;
		}

		Element *prevEl = temp;								// stores a pointer to element previous to deleted one

		//=============== Go one step forward to the indexed element ============
		temp = temp->nextPtr;

		number = temp->var;
		count++;

		prevEl->nextPtr = temp->nextPtr;					// changes previous element pointer to next element so that it skips the 'deleted' element
		free(temp);											// deletes memory used by indexed and now unlinked element
	}

	printf("Deleted element %d was: %d\n", count+1, number);
}


void freeMemory(Element *headNode)
{
	Element *temp;

	while(headNode != NULL)
	{
		temp = headNode;

		headNode = headNode->nextPtr;

		free(temp);
	}

	printf("\n====== Memory freed ======\n");
}
