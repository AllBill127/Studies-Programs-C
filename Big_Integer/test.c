//Sukūrė Aleksandras Bilevičius stud. ID: 2015859

#include <assert.h>
#include <stdio.h>
#include "big_int.h"


int main()
{
    big_int* a, *b, *sub, *mul;

    a = new_big_int("-3");
    assert(a != errNo_3);
    assert(a != errNo_1);
        print_big_int(a);

/*    //Test if big_int not in int format
    b = new_big_int("1fhjn3");
    assert(b != errNo_3);
    assert(a != errNo_1);
*/

    b = new_big_int("-2");
    assert(b != errNo_3);
    assert(b != errNo_1);
        print_big_int(b);

    sub = sub_big_int(a, b);
    assert(sub != errNo_1);
    assert(sub != errNo_2);
        print_big_int(sub);
    sub = sub_big_int(b, a);
    assert(sub != errNo_1);
    assert(sub != errNo_2);
        print_big_int(sub);
    free_big_int(sub);

    mul = mul_big_int(a, b);
    assert(mul != errNo_1);
    assert(mul != errNo_2);
        print_big_int(mul);
    mul = mul_big_int(mul, a);
    assert(mul != errNo_1);
    assert(mul != errNo_2);
        print_big_int(mul);
    free_big_int(mul);

    free_big_int(a);
    free_big_int(b);

    printf("END");

    return 0;
}
