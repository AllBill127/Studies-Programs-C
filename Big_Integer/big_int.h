//Sukūrė Aleksandras Bilevičius stud. ID: 2015859

#ifndef BIG_INT
#define BIG_INT

//======== Possible error codes ========
#define errNo_1     (big_int*)-1        // when malloc fails
#define errNo_2     (big_int*)-2        // when realloc fails
#define errNo_3     (big_int*)-3        // when string is not in int format
#define errNo_4     (big_int*)-4        // when devision by zero


typedef struct BigInt
{
    unsigned long *data;
    unsigned int len;
    unsigned char sign;
}big_int;

//======== Public functions of big_int =========
big_int* new_big_int(char* line);
void free_big_int(big_int* num);
void print_big_int(big_int* num);
big_int* add_big_int(big_int* a, big_int* b);
big_int* sub_big_int(big_int* a, big_int* b);
big_int* mul_big_int(big_int* a, big_int* b);
big_int* div_big_int(big_int* a, big_int* b);

#endif
