// Programa, kuri duotame tekste keletą iš eilės einančių vienodų simbolių (išskyrus
// tarpus) pakeičia vienu tokiu simboliu, $ ir skaičiumi buvusių simbolių (pvz., tekstas
// "aaa bbuvvvvo" turi būti pakeistas į "a$3 b$2uv$4o").

//@@ Programa failu pavadinimus turi pasiimti is argumentu eilutes
//@@ Turi buti numatyti atvejai kai pavadinimai arg. eiluteje nepateikti ...
//@@ ... duomenu/rezultatu failo atidaryti/sukurti nepavyko
//@@ Eilutes max ilgis 255 simboliai
//@@ ??? Programa gali apdoroti kiekviena failo eilute atskirai

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "write_backwards.h"
#include "get_valid_int.h"

#define MAX_BUFFER_SIZE 20

int main(int argc, char **argv)		// char **argv a pointer to char * in which a string is stored; // char *argv[] is an array of char * pointers in which a string is stored
{
	//========================Failu vardu gavimas====================
	
    char *data, *rez;       // Pointers to file names
    if(argc == 3)       	// If exactly two argument were given their names are assigned to file name pointers (first argc is for main function)
    {
        data = argv[1];
        rez = argv[2];
    }
    else
    {
        printf("You forgot to write the data and results file names as arguments for the program. \n");
        printf("Write the file names in (name1.txt name2.txt) format bellow and press enter to continue.\n");
        char x1[50], x2[50];
        scanf("%s %s", x1, x2);
        data = x1;                  // Assigns inputted names to file name pointers
        rez = x2;                   //
    }
    printf("Files to be used: %s %s\n", data, rez);
	
	//========================Formatuojamos eilutes pasirinkimas======================
	
	//printf("Enter the line number you want to reformat: ");
	//int line = Get_Valid_Int();

	//=======================Failo atidarymas ir duomenu nuskaitymas==================
	
    FILE *read;
    read = fopen(data, "r");
    if(read == NULL)
    {
        printf("Data file failed to open. Shutting down the program... \n");
        exit(1);
    }

	//=======================Failo atidarymas ir duomenu isvedimas==================

    FILE *write;
    write = fopen(rez, "w");
    if(write == NULL)
    {
        printf("Results file failed to open. Shutting down the program... \n");
        exit(1);
    }
	
	char Input[MAX_BUFFER_SIZE];
	char LineOverflow[MAX_BUFFER_SIZE];
	
	while(fgets(Input, MAX_BUFFER_SIZE, read) != NULL)		// Skaito po viena duomenu failo eilute iki '\n' arba MAX_BUFFER_SIZE, jeigu nepasiekta failo pabaiga
	{
		printf("Text line taken: %s\n", Input);
		if(strlen(Input)+1 == MAX_BUFFER_SIZE)				// Jeigu nuskaityta eilute buvo MAX_BUFFER_SIZE ilgio tai viska po jos ignoruoja iki '\n'
		{
			char c = fgetc(read);
			while(c != '\n')								// Nuskaito po viena simboli is per ilgos eilutes dalies iki kol pasiekia '\n'
			{
				if(c == EOF)								// Jeigu ignoruojant per ilga eilutes dali pasiekiama failo pabaiga, ignoravimas sustabdomas
				{
					break;
				}
				c = fgetc(read);
			}
		}
		
		char *Output = Write_Backwards(Input, strlen(Input));	// Gauna pointer'i i atminties vieta nuo kurios yra saugomas pertvarkyta eilute 
		
		printf("Reformated line: %s", Output);
		fprintf(write, "%s", Output);
		free(Output);										// frees the memory originally allocated in the Write_Backwards function by malloc to reuse when calling the function again
	}
	printf("End of file\n");
	
    fclose(read);											// Uzdaro pasirinkta faila
	fclose(write);
	
	printf("Program ended successfully\n");

    return 0;
}

