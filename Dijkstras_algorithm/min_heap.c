#include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include "min_heap.h"

 int heap_init (heap_t *heap, int (*compare_func)(const void *, const void *)) {
     if (!heap) {
         return -1;
     }

     heap->heap_array = NULL; 
     heap->heap_max_size = heap->heap_current_size = 0; 
     heap->compare_func = compare_func;
     return 0;
 }

 void heap_heapify_from_top (heap_t *heap, int counter) {
     void *temp_val = NULL;
     int child_counter;
     int has_left_child = 0;
     int has_right_child = 0;

     if ((2 * counter + 1) < heap->heap_current_size)
         has_left_child = 1;

     if ((2 * counter + 2) < heap->heap_current_size)
         has_right_child = 1;

     /* Now, let us find the lowest of the two children */
     if (has_left_child && has_right_child) { 
         /* If compare_fun returns negative value, then the left child is smaller */
         if (heap->compare_func(heap->heap_array[2* counter + 1], 
                         heap->heap_array[2*counter + 2]) < 0) {
             child_counter = 2 * counter + 1;
         } else {
             child_counter = 2 * counter + 2;
         }
     } else if (has_left_child) {
         child_counter = 2 * counter + 1;
     } else if (has_right_child) {
         child_counter = 2 * counter + 2;
     } else {
         return; 
     }

     /* If compare_func returns negative value, then the child is smaller */
     if (heap->compare_func(heap->heap_array[child_counter], heap->heap_array[counter]) < 0) { 
         temp_val = heap->heap_array[counter];
         heap->heap_array[counter] = heap->heap_array[child_counter];
         heap->heap_array[child_counter] = temp_val;
         heap_heapify_from_top(heap, child_counter); 
     }
     return;
 }

 void *heap_extract (heap_t *heap, int *err) { 
     void *min_node = NULL;
     *err = 0;

     if (heap->heap_current_size == 0 ) {
         *err = -1;
         return NULL;
     }

     min_node = heap->heap_array[0];

     heap->heap_array[0] = heap->heap_array[heap->heap_current_size-1];
     heap->heap_current_size--;

     if (heap->heap_current_size != 1) {
         heap_heapify_from_top(heap, 0);
     }
     return min_node;
 }

 void heap_insert_heapify_from_bottom (heap_t *heap, int counter) {
     int parent = (int) floor((double)counter/2);
     void *temp_val;

     if (counter == 0) {
         return;
     }

     /* If compare_func returns negative value, then the child is smaller */
     if (heap->compare_func(heap->heap_array[counter], heap->heap_array[parent]) < 0) {
         temp_val = heap->heap_array[counter];
         heap->heap_array[counter] = heap->heap_array[parent];
         heap->heap_array[parent] = temp_val;
     }
     heap_insert_heapify_from_bottom(heap, parent);
 }

 int heap_add (heap_t *heap, void *value) {
     if (heap->heap_current_size == heap->heap_max_size) {
         heap->heap_max_size += SIZE_OF_BLOCK_ALLOCATION;
         heap->heap_array = (void*)realloc(heap->heap_array, 
                         heap->heap_max_size * sizeof(void *));
         if (!heap->heap_array) {
             return -1;
         }
     }
     heap->heap_array[heap->heap_current_size] = value;
     heap_insert_heapify_from_bottom(heap, heap->heap_current_size);
     heap->heap_current_size++;
     return 0;
 }

 int heap_delete_recursively (heap_t *heap, int counter, void *value) {
     int child_counter; 

     if ((counter < 0) || (counter >= heap->heap_current_size))
         return -1;

     if (heap->compare_func(value, heap->heap_array[counter]) == 0) {
         heap->heap_array[counter] = heap->heap_array[heap->heap_current_size-1];
         heap->heap_current_size--;

         heap_heapify_from_top(heap, counter);
         return 0;
     }

     child_counter = 2 * counter + 1;
     if (child_counter >= heap->heap_current_size) {
         return -1;
     }

     if (heap_delete_recursively(heap, child_counter, value) == 0 ) {
         return 0;
     }

     child_counter = 2 * (counter + 1);
     if (child_counter >= heap->heap_current_size) {
         return -1;
     }

     if (heap_delete_recursively(heap, child_counter, value) == 0) {
         return 0;
     }
     return -1;
 }

 int heap_delete (heap_t *heap, void *value) {
     return (heap_delete_recursively(heap, 0, value)); 
 }
 
 int heap_compare (const void *value1, const void *value2) {
     if ( *(int *)value1 < *(int *)value2 ) {
         return -1;
     } else if ( *(int *)value1 > *(int *)value2 ) {
         return 1;
     } else {
         return 0;
     }
 }
 
 void heap_print (heap_t *heap) {
     int counter = 0;

     printf("\nPrinting the Heap: \n");
     if (heap == NULL){
         return;
     }

     printf("Heap-size: %d (Max-size: %d)\n", 
             heap->heap_current_size, heap->heap_max_size);
     for (counter = 0; counter < heap->heap_current_size; counter++) {
         if (counter == 0) { 
             printf("%d(p=NULL) ", *(int *)(heap->heap_array[counter]));
         } else {
             printf("%d(p=%d) ", 
                     *(int *)(heap->heap_array[counter]), 
                     *(int *)(heap->heap_array[(int) floor((double)(counter-1)/2)]));
         }
     }
     printf("\n");
 }