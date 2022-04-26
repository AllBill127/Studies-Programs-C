#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "char_compress.h"

char* Char_Compress(char A[])
{
	char repC, *Output;
    int rep = 1, out = 0;

    int len = strlen(A);
	
	Output = malloc(len * sizeof(char));
	
    for (int i = 0; i < len; ++i)
    {
        if(A[i] != ' ')                     // Jeigu tarpas, tai iveda tarpa, jei ne - tikrina simboli
        {
            if(A[i] == A[i+1])              // Jeigu dabartinis simbolis sutampa su sekanciu - issaugo simboli ir padidina pasikartojimu skaiciu
            {
                rep++;
                repC = A[i];
            }
            else if(rep > 1)                // Jeigu simbolis nesutapo su sekanciu, bet pasikartojimu > 1, tai isveda info apie pasikartojima
            {
                Output[out] = repC;
                out++;
                Output[out] = '$';
                out++;
				
				int digitCount = floor(log10(rep)) + 1;			// Suskaiciuoja kiek skaitmenu buvo skaiciuje. floor numeta dali po kablelio; +1, nes log10 grazina skaiciu viena desimtimi mazesni uz irasyta
				
				for(int j = digitCount-1; j >= 0; --j)			// Isveda skaitmenis kaip simbolius
				{
					Output[out+j] = (rep % 10) + '0';
					rep = rep / 10;
				}
				out += digitCount;

                rep = 1;
            }
            else                            // Jei simbolis nesutampa su sekanciu ir pasikartojimu nebuvo - isveda tikrinama simboli
            {
                Output[out] = A[i];
                out++;
            }
        }
        else
        {
            Output[out] = ' ';
            out++;
        }
    }
	
	if(Output[out-1] != '\n')			// Jeigu nuskaityta ir pertvarkyta eilute neturejo '\n' simbolio pabaigoje, ji priverstinai iterpia 
	{
		Output[out] = '\n';
		out++;
	}
	
	Output[out] = '\0';					// Naudojamas nurodyti teksto pabaiga kai kreipiamasi i printf("%s") ar kitas string funkcijas
	
	Output = realloc(Output, (out+1) * sizeof(char));		// realloc size = out+1, nes reikia sutalpinti '\0' reiksme
	
	return Output;
}