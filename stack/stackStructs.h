#ifndef stackStructs_h
#define stackStructs_h

typedef struct nodeS
{
    my_typeS nodeData;
    struct nodeS *next;

} NodeS;

typedef struct
{
    NodeS *top;
    int count;

} Stack;

#endif
