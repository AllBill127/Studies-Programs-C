//========== Counts and prints 5 most repeated words in inputed text ==========
//========== Program ends when enter is pressed three times in a row ==========

#include <stdio.h>
#include <string.h>

#define MAX_WORDS 500
#define MAX_LETTERS 30

struct words
{
    char word[MAX_LETTERS];     //Word symbols
    int count;                  //Number of repetitions
};

//================== Funkcijos ====================
void Atranka(int wordNum);
int Match(char word[MAX_LETTERS], int len);
void Rikiavimas();


//============== Globalus kintamieji ==============
struct words Text[MAX_WORDS], Rep[MAX_WORDS];
int repLen = 0;


//=================== Main ==================
int main()
{
// Nuskaitymas
    printf("Parasykite iki 500 zodziu ilgio teksta \n");
    printf("Noredami uzbaigti tris kartus spauskite ENTER \n");


    int exitCount = 0, wordNum = 0, charNum = 0, space = 0;
    char c;

    while (exitCount < 3)
    {
        scanf("%c", &c);

    // Exit condition
        if (c == '\n')
        {
            exitCount++;

            if (exitCount == 1)         // Jeigu viena karta paspaustas ENTER, tai pradeda nauja zodi
            {
                wordNum++;
                charNum = 0;
            }

            if (exitCount == 2)
            {
                printf("Jeigu dar karta paspausite ENTER programa pereis prie skaiciavimu. \n");
                printf("Jeigu norite testi, ivedinekite zodzius toliau. \n");
            }
            continue;
        }
        else
            exitCount = 0;

    // One word input
        if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            if (space > 0)
            {
                wordNum++;
                charNum = 0;
                space = 0;
            }
            Text[wordNum].word[charNum] = c;
            Text[wordNum].count++;
            charNum++;
        }
        else
        {
            if (Text[0].count > 0)
                ++space;
        }
    }


// Skaiciavimai
    Atranka(wordNum);
    Rikiavimas();


// Isvedimas
    printf ("5 Dazniausiai pasikartojantys zodziai: \n");
    int equal = 1;
    while(Rep[4].count == Rep[4+equal].count)
        equal++;

    for (int i = 0; i < 4+equal; i++)
    {
        printf ("%s pasikartoja %d kartus \n", Rep[i].word, Rep[i].count);
    }

    return 0;
}


void Atranka(int wordNum)
{
    for (int i = 0; i < wordNum; ++i)
    {
        int k = Match(Text[i].word, repLen);
        if (k >= 0)
        {
            Rep[k].count++;
        }
        else
        {
            Rep[repLen] = Text[i];
            Rep[repLen].count = 1;
            repLen++;
        }
    }
}

int Match(char word[MAX_LETTERS], int len)
{
    for (int i = 0; i < len; ++i)
        if (strcmp(Rep[i].word, word) == 0)
            return i;

    return -1;
}

void Rikiavimas()
{
    struct words temp;
    for (int i = 0; i < repLen; ++i)
        for (int j = i; j < repLen; ++j)
            if(Rep[i].count < Rep[j].count)
            {
                temp = Rep[j];
                Rep[j] = Rep[i];
                Rep[i] = temp;
            }
}



/*
As esu grybukas, gyvenantis po labai sutresusiu, nugriuvusiu medziu, kuris yra gudziame mi
ske
Sis - Azuolyno miskas stovi jau 900 metu

Jeigu dar karta paspausite ENTER programa pereis prie skaiciavimu.
Jeigu norite testi, ivedinekite zodzius toliau.
Man labai patinka gyventi siame miske po nugriuvusiu medziu


a a a a a b b b b b - - -  - - - 7 a s 7 8 7 & * 7  &
c c c c c x x x - - i i i j j j s
*/