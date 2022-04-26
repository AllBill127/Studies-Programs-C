 #ifndef __HEAP_H___
 #define __HEAP_H___

 #define SIZE_OF_BLOCK_ALLOCATION  10 

 typedef struct heap_ {
     void **heap_array;     /* An array of void pointers */
     int heap_max_size;     /* Maximum allocated size for heap */ 
     int heap_current_size; /* Number of elements in the heap */ 
     int (*compare_func)(const void *, const void *); /* To compare "void *" elements */
 } heap_t;

 int heap_init(heap_t *heap, int (*compareFunc)(const void *, const void *));
 int heap_add(heap_t *heap, void *value); 
 void *heap_extract(heap_t *heap, int *err);
 int heap_delete(heap_t *heap, void *value); 
 int heap_compare (const void *value1, const void *value2);
 void heap_print (heap_t *heap);
 #endif /* __HEAP_H___ */