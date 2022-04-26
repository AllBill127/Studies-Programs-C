//studento nr. 2016036

#ifndef queueStructs_h
#define queueStructs_h

struct nodeQ
{
    my_typeQ nodeData;
    struct nodeQ *next;

};

typedef struct nodeQ NodeQ;

typedef struct
{
     NodeQ *front;
     NodeQ *rear;
     int count;

} Queue;

#endif
