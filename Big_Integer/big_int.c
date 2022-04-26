//Sukūrė Aleksandras Bilevičius stud. ID: 2015859

//Big Intiger subtraction and multiplication only (division also)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "big_int.h"


int is_num(char* line);
big_int* new_malloc();
void reverse_big_int(big_int* num);
int compare_big_int(big_int* a, big_int* b);


//================= Private functions =================

//Check if given string fits intiger format
//Return 0 if not int format
//Return 1 if int format
int is_num(char* line)
{
    int line_len = strlen(line);
    for(int i = 0; i < line_len; ++i)
        if((i == 0) && (line[i] != '+' && line[i] != '-'))
        {
            if(line[i] < '0' || line[i] > '9')
                return 0;
        }
        else if ((i != 0) && (line[i] < '0' || line[i] > '9'))
            return 0;

    return 1;
}

//Allocates memory for big int struct
big_int* malloc_big_int()
{
    big_int* ptr = (big_int*)malloc(sizeof(big_int));
    if(ptr == NULL)
        return errNo_1;
    return ptr;
}

//Reverse big int
void reverse_big_int(big_int* num)
{
    int tmp, len = num->len - 1;
    for(int i = 0; (len - i) > (len / 2); ++i)
    {
        tmp = num->data[i];
        num->data[i] = num->data[len - i];
        num->data[len - i] = tmp;
    }
}

//Compare two big ints
// 1 = true if a > b
// 0 = false if a > b
// 0 = false if a = b
int compare_big_int(big_int* a, big_int* b)
{
    if(a->len > b->len)
        return 1;
    else if(b->len > a->len)
        return 0;
    else
        for(int i = 0; i < a->len; ++i)
        {
            if(a->data[i] > b->data[i])
                return 1;
            else if(a->data[i] < b->data[i])
                return 0;
        }

    return 0;
}


//================== Public functions =================

//Free memory assigned to big int
void free_big_int(big_int* num)
{
    free(num->data);
    free(num);
}

//New big int from string
big_int* new_big_int(char* line)
{
    //Return NULL pointer if given string does not fit intiger format
    if(is_num(line) == 0)
        return errNo_3;


    big_int* num = malloc_big_int();
    if(num == errNo_1)
        return errNo_1;


    int skip = 0;               // finds were the number starts
    if(line[0] == '-')          // finds number sign
    {
        num->sign = '-';
        skip++;
    }
    else if(line[0] == '+')
    {
        num->sign = '+';
        skip++;
    }
    else
        num->sign = '+';


    //Skip 0 in the begining
    while(line[skip] == '0')
        skip++;


    num->len = strlen(line) - skip;    // length of number without sign and 0 from the beginning


    if(num->len == 0)
    {
        num->len++;
        num->data = (unsigned long*)malloc(num->len * sizeof(int));
        if(num->data == NULL)
            return errNo_1;
        num->data[0] = 0;
    }
    else
    {
        num->data = (unsigned long*)malloc(num->len * sizeof(int));
        if(num->data == NULL)
            return errNo_1;
        for(int i = 0; i < num->len; ++i)
            num->data[i] = (unsigned long)line[skip  + i] - '0';
    }

    return num;
}

//Print big int to terminal
void print_big_int(big_int* num)
{
    //Dont print sign if num = 0 or sign is '+'
    if(num->sign == '-' && num->data[0] != 0)
        printf("%c", num->sign);

    for(int i = 0; i < num->len; ++i)
        printf("%c", (num->data[i] + '0'));

    printf("\n");
}

//Add two big ints
big_int* add_big_int(big_int* a, big_int* b)
{
    // check if addition or subtraction
    big_int* sum;
    if(a->sign == b->sign)
    {
        sum = malloc_big_int();
        if(sum == errNo_1)
        return errNo_1;

        sum->len = (a->len > b->len ? a->len : b->len) + 1;
        sum->data = (unsigned long*)malloc(sum->len * sizeof(int));
        if(sum->data == NULL)
            return errNo_1;


        //Determine sum sign
        if(a->sign == '+')
            sum->sign = '+';
        else
            sum->sign = '-';


    //Add two numbers
        int i = a->len - 1, j = b->len - 1, k = sum->len - 1;
        int carry = 0, temp_sum;

        while(i >= 0 || j >= 0 || carry > 0)
        {
            if(i >= 0 && j >= 0)
                temp_sum = a->data[i] + b->data[j];
            else if(i >= 0)
                temp_sum = a->data[i];
            else if(j >= 0)
                temp_sum = b->data[j];
            else
                temp_sum = 0;

            temp_sum += carry;
            carry = temp_sum / 10;
            sum->data[k] = temp_sum % 10;

            i--; j--; k--;
        }
        sum->data[k] = carry;


        //If sum did not carry over one extra number drop extra 0
        if(sum->data[0] == 0)
        {
            reverse_big_int(sum);
            sum->len--;
            sum->data = (unsigned long*)realloc(sum->data, sum->len * sizeof(int));
            if(sum->data == NULL)
                return errNo_2;
            reverse_big_int(sum);
        }


        return sum;
    }
    else
    {
        //Determine sign when sum is subtraction
        char sumSIGN;
        if(compare_big_int(a, b) == 1)
            if(a->sign == '+')
                sumSIGN = '+';
            else
                sumSIGN = '-';
        else
            if(a->sign == '-')
                sumSIGN = '+';
            else
                sumSIGN = '-';


        //Save original signs and make current a, b signs '+'
        char sign1 = a->sign, sign2 = b->sign;
        a->sign = '+'; b->sign = '+';

        sum = sub_big_int(a, b);
        sum->sign = sumSIGN;


        //Return original signs to big ints
        a->sign = sign1; b->sign = sign2;


        return sum;
    }
}

//Subtract two big ints
big_int* sub_big_int(big_int* a, big_int* b)
{
    big_int *sub, *sk1, *sk2;
    //Determine if subtraction or addition by signs
    if(a->sign == b->sign)
    {
        sub = malloc_big_int();
        if(sub == errNo_1)
        return errNo_1;

        //If a != b (sub != 0), find bigger number and determine sub sign and length ===========================================================
        if(compare_big_int(a, b) == 0 && compare_big_int(b, a) == 0)
        {
            sub->sign = '+';

            sub->len = a->len;
            sk1 = a; sk2 = b;
        }
        else if(compare_big_int(a, b) == 1)
        {
            if(a->sign == '+')
                sub->sign = '+';
            else
                sub->sign = '-';

            sub->len = a->len;
            sk1 = a; sk2 = b;
        }
        else
        {
            if(a->sign == '+')
                sub->sign = '-';
            else
                sub->sign = '+';

            sub->len = b->len;
            sk1 = b; sk2 = a;
        }

        sub->data = (unsigned long*)malloc((sub->len)* sizeof(int));
        if(sub->data == NULL)
            return errNo_1;


    //Subtract two numbers
        reverse_big_int(sk1);
        reverse_big_int(sk2);

        int len = sk1->len;
        int carry = 0, temp_sub, temp_sk1, temp_sk2;
        for(int i = 0; i < len; ++i)
        {
            if(sk2->len <= i )
            {
                temp_sk1 = sk1->data[i];
                temp_sk2 = 0;
            }
            else
            {
                temp_sk1 = sk1->data[i];
                temp_sk2 = sk2->data[i];
            }


            if((temp_sk1 - carry  - temp_sk2) < 0)
            {
                temp_sub = 10 + temp_sk1 - carry - temp_sk2;
                carry = 1;
            }
            else
            {
                temp_sub = temp_sk1 - carry - temp_sk2;
                carry = 0;
            }

            sub->data[i] = temp_sub;
        }


        //Find sub len and realloc to drop 0
        while(sub->data[sub->len - 1] == 0)
            if(sub->len == 1)
                break;
            else
                sub->len--;

        sub->data = (unsigned long*)realloc(sub->data, sub->len * sizeof(int));
        if(sub->data == NULL)
                return errNo_2;


        reverse_big_int(sk1);
        reverse_big_int(sk2);
        reverse_big_int(sub);

        return sub;
    }
    else
    {
        //Determine sign when sub is addition
        char tmpSIGN;
        if(a->sign == '-')
            tmpSIGN = '-';
        else
            tmpSIGN = '+';


        //Save original signs and make current a, b signs '+'
        char sign1 = a->sign, sign2 = b->sign;
        a->sign = '+'; b->sign = '+';

        sub = add_big_int(a, b);
        sub->sign = tmpSIGN;

        //Return original signs to big ints
        a->sign = sign1; b->sign = sign2;

        return sub;
    }
}

//Multiply two big ints
big_int* mul_big_int(big_int* a, big_int* b)
{
    big_int* mul = malloc_big_int();
    mul->len = a->len + b->len;
    mul->data = (unsigned long*)malloc(mul->len * sizeof(int));
    if(mul->data == NULL)
        return errNo_1;

    //Determine mul sign
    if(a->sign == b->sign)
        mul->sign = '+';
    else
        mul->sign = '-';


    //Zeroes mul->data
    for(int i = 0; i < mul->len; ++i)
        mul->data[i] = 0;


//Multiply two numbers
    reverse_big_int(a);
    reverse_big_int(b);

    int mul_temp, carry = 0, mul_index, last_carry = 0, carry_index;
    for(int i = 0; i < a->len; ++i)
    {
        mul_index = i;
        for(int j = 0; j < b->len; ++j)
        {
            mul_temp = a->data[i] * b->data[j];

            //Add last carry number at the same index it carried to
            if(carry_index == mul_index)
                carry += last_carry;

            mul_temp += carry;
            carry = mul_temp / 10;

            mul->data[mul_index] += mul_temp % 10;
            carry += mul->data[mul_index] / 10;

            mul->data[mul_index] = mul->data[mul_index] % 10;

            mul_index ++;
        }
        //Saves last carry number for later
        last_carry = carry;
        carry_index = mul_index;
        carry = 0;
    }
    //Add last carry number
    mul->data[carry_index] = last_carry;


    //find mul len and realloc to drop 0
    while(mul->data[mul->len-1] == 0)
        if(mul->len == 1)
            break;
        else
            mul->len--;

    mul->data = (unsigned long*)realloc(mul->data, mul->len * sizeof(int));
    if(mul->data == NULL)
        return errNo_2;


    reverse_big_int(a);
    reverse_big_int(b);
    reverse_big_int(mul);

    return mul;
}

//Divide two big ints
big_int* div_big_int(big_int* a, big_int* b)
{
    //Rezult
    big_int* div = malloc_big_int();
    if(div == errNo_1)
        return errNo_1;
    div->len = a->len;
    div->data = (unsigned long*)malloc(div->len * sizeof(int));
    if(div->data == NULL)
        return errNo_1;

    //Part of a to subtract from
    big_int* a_temp = malloc_big_int();
    if(a_temp == errNo_1)
        return errNo_1;
    a_temp->sign = '+';
    a_temp->len = a->len;
    a_temp->data = (unsigned long*)malloc(a_temp->len * sizeof(int));
    if(a_temp->data == NULL)
        return errNo_1;


    //Determine div sign
    if(a->sign == b->sign)
        div->sign = '+';
    else
        div->sign = '-';


    //Save original b sign and make current '+' for correct subtrraction
    char b_sign = b->sign;
    b->sign = '+';


    //Check if division by 0
    if(b->data[0] == 0)
        return NULL;
    else
    {
        //Zero part of a before checking
        a_temp->data[0] = 0;
        a_temp->len = 1;


    //Divide two numbers
        int count = 0;
        int digit_count = 0; //case when one new digit is added. After that must count how many b in part of a
        div->len = 0;

        //Go over all a digits
        for(int i = 0; i < a->len; )
        {
            //Creating a part that fits least amount of b
            digit_count = 0;
            while(compare_big_int(b, a_temp) == 1 && digit_count == 0)
            {
                //If part of a = 0
                if(a_temp->data[0] == 0)
                {
                    a_temp->data[0] = a->data[i];
                    a_temp->len = 1;
                    i++;
                    digit_count = 1;
                }
                else
                {
                    //if part of a was not 0 and last digit was not reached then add new digit
                    if(i < a->len)
                    {
                        a_temp->data[a_temp->len] = a->data[i];
                        a_temp->len++;
                        i++;
                        digit_count = 1;
                    }
                    else
                        break;
                }
            }


            //Count how many b fits in part of a
            while(compare_big_int(b, a_temp) == 0)
            {
                count++;
                a_temp = sub_big_int(a_temp, b);
            }


            //Input count to div from right to left (reversed)
            div->data[div->len] = count;
            div->len++;
            count = 0;
        }


        //Find div len and realloc to drop 0
        reverse_big_int(div);
        while(div->data[div->len - 1] == 0)
            if(div->len == 1)
                break;
            else
                div->len--;

        div->data = (unsigned long*)realloc(div->data, div->len * sizeof(int));
        if(div->data == NULL)
                return errNo_2;
        reverse_big_int(div);

        //Free big_ints that are no more in use
        free_big_int(a_temp);
    }


    //Return original b sign
    b->sign = b_sign;

    return div;
}

