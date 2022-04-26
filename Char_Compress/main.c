// Programa, kuri duotame tekste keletą iš eilės einančių vienodų simbolių (išskyrus
// tarpus) pakeičia vienu tokiu simboliu, $ ir skaičiumi buvusių simbolių (pvz., tekstas
// "aaa bbuvvvvo" turi būti pakeistas į "a$3 b$2uv$4o").

//@@ Programa failu pavadinimus turi pasiimti is argumentu eilutes
//@@ Turi buti numatyti atvejai kai pavadinimai arg. eiluteje nepateikti ...
//@@ ... duomenu/rezultatu failo atidaryti/sukurti nepavyko
//@@ Eilutes max ilgis 255 simboliai
//@@ Programa gali apdoroti kiekviena failo eilute atskirai



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char_compress.h"
#include "get_valid_int.h"

#define MAX_BUFFER_SIZE 255

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
	
	printf("Enter the line number you want to reformat: ");
	int line = Get_Valid_Int();

	//=======================Failo atidarymas ir duomenu nuskaitymas==================
	
    FILE *read;
    read = fopen(data, "r");
    if(read == NULL)
    {
        printf("Data file failed to open. Shutting down the program... \n");
        exit(1);
    }

	for(int i = 1; i < line; )
	{						
		char c = fgetc(read);
			if(c == EOF)									// Patikrina ar pasirinkta eilute nera uz failo pabaigos ribu
			{
				printf("End of file\n");
				fclose(read);
				return 0;
			}
			else if(c == '\n')								// Praleidzia tiek nauju eiluciu kad butu nuskaitoma vartotojo pasirinkta eilute
				++i;
	}
    char Input[MAX_BUFFER_SIZE];
	
	fgets(Input, MAX_BUFFER_SIZE, read);					// Nuskaito duota simboliu skaiciu arba viska iki naujos eilutes iskaitant \n simboli
	fclose(read);
	
	printf("Text line taken: %s\n", Input);

	//=======================Failo atidarymas ir duomenu isvedimas==================

    FILE *write;
    write = fopen(rez, "w");
    if(write == NULL)
    {
        printf("Results file failed to open. Shutting down the program... \n");
        exit(1);
    }

    char *Output = Char_Compress(Input);
	
	printf("Reformated line: %s", Output);
	fprintf(write, "%s", Output);
	
	free(Output);											// frees the memory originally allocated in the Char_Compress function by malloc and realloc
    fclose(write);
	
	printf("Program ended successfully\n");

    return 0;
}

