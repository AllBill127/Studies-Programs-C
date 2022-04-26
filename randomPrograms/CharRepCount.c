//======== Counts and prints 5 most repeated simbols in a row ==============

#include <stdio.h>
#include <string.h>


#define MAX_SIZE 100

struct character{
    char c;
    int amount;
};

void Atranka(char C[MAX_SIZE], int len);
int Match(char x, int z);
void Rikiavimas();

struct character Rep[MAX_SIZE];
int repLen = 0;



int main()
{
	char C[MAX_SIZE];
	int len;
	
	printf ("Iveskite teksto eilute iki 100 simboliu\n");
	
	scanf("%[^\n]c", C);
	len = strlen(C);

// Ivesties patikrinimas
	printf ("%s \n", C);
	
// Skaiciavimai
	Atranka(C, len);
	Rikiavimas();

// Isvedimas
	printf ("5 Dazniausiai pasikartojantys simboliai: \n");
	if(repLen >= 5)
        repLen = 5;

	for (int i = 0; i < repLen; i++)
	{
		printf ("%c pasikartoja %d kartu \n", Rep[i].c, Rep[i].amount);
	}
	
	return 0;
}



void Atranka(char C[MAX_SIZE], int len)
{
    for (int i = 0; i < len; ++i)
    {
        int k = Match(C[i], repLen);
        if (k >= 0)
        {
            Rep[k].amount++;
        }
        else
        {
            Rep[repLen].c = C[i];
            Rep[repLen].amount = 1;
            repLen++;
        }
    }
}

int Match(char x, int z)
{
    for (int i = 0; i < z; ++i)
        if (Rep[i].c == x)
            return i;

    return -1;
}

void Rikiavimas()
{
    struct character temp;
    for (int i = 0; i < repLen; ++i)
        for (int j = i; j < repLen; ++j)
            if(Rep[i].amount < Rep[j].amount)
            {
                temp = Rep[j];
                Rep[j] = Rep[i];
                Rep[i] = temp;
            }
}

// aaaaabb bbb --- --- 7as787&*7&