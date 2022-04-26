#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "write_backwards.h"

char * Write_Backwards(char A[], int len)
{
	char *Output;
	int wordLen = 0, mrk = 0;
	
	Output = malloc((len+1) * sizeof(char));					// priskiria nuskaitytos eilutes dydzio atminties gabala +1, jeigu nebutu '\n' ir ji reiktu iterpti
	
	for(int i = 0; i <= len; ++i)								// pereina per kiekviena eilutes simboli
	{
		if(A[i] == ' ' || A[i] == '\n' || i == len)				
		{
			for(int j = 0; j < wordLen; ++j)
			{
				Output[mrk] = A[i-j-1];
				mrk++;
			}
			
			Output[mrk] = A[i];
			mrk++;
			wordLen = 0;
		}
		else
		{
			wordLen++;
		}
	}
	
	if(Output[mrk-1] == '\0' && Output[mrk-2] != '\n')			// Jeigu nuskaityta ir pertvarkyta eilute neturejo '\n' simbolio pabaigoje, ji priverstinai iterpia 
	{
		Output[mrk-1] = '\n';
		Output[mrk] = '\0';
	}
	
	return Output;
}