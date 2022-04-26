 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <limits.h>
 #include "adjacency_list.h"
 #include "min_heap.h"

 #define  MAX_ELEMENTS  32 

 /* Represents an edge (has both sides, represented as vnode) and the edge weight. */
 typedef struct dijkstra_edge_nodes_ {
     vertex_node *vnodeA; 
     vertex_node *vnodeB;
     int weight;          /* Weight of the Edge */
 } dijkstra_edge_nodes;

 /* The global variables */
 int arr_distance[MAX_ELEMENTS][3];  /* Double-array that holds all the distances */
 int len_arr_distance;               /* Max Length of the above array */
 vertex_node *graph_root = NULL;     /* Root of the graph */

 int dijkstra_heap_compare (const void *val1, const void *val2) {
     dijkstra_edge_nodes *node1 = (dijkstra_edge_nodes *)val1;
     dijkstra_edge_nodes *node2 = (dijkstra_edge_nodes *)val2;

     if (node1->weight < node2->weight) {
         return -1;
     } else if (node1->weight > node2->weight) {
         return 1;
     } else {
         return 0;
     }
 }

 void dijkstra_heap_print (heap_t *heap) {
     dijkstra_edge_nodes *node, *parent;
     int counter = 0;

     printf("Printing the Heap: \n");
     if ((heap == NULL) || (heap->heap_current_size == 0)) {
         printf("Heap is Empty\n");
         return;
     }  
     for (counter = 0; counter < heap->heap_current_size; counter++) {
         node = (dijkstra_edge_nodes *)heap->heap_array[counter];
         parent = (dijkstra_edge_nodes *)(heap->heap_array[(int) floor((double)(counter-1)/2)]);
         if (counter == 0) {
             printf("\t[i: %d] %2d-%2d (Weight: %2d Parent: NULL)\n",
                counter, node->vnodeA->val, node->vnodeB->val, node->weight);
         } else {
             printf("\t[i: %d] %2d-%2d (Weight: %2d Parent: %d-%d)\n",
                counter, node->vnodeA->val, node->vnodeB->val, node->weight,
                parent->vnodeA->val, parent->vnodeB->val);
         }
     }
     printf("\n");
 }

 void dijkstra_print_distance_array (void) {
     int i;
     printf("Distance Array: ");
     for (i = 0; i < len_arr_distance; i++) {
         printf("%d (p: %d, d: %d)  ", 
             arr_distance[i][0], arr_distance[i][1], arr_distance[i][2]); 
         if (((i+1) % 3) == 0) { /* Add a new line every 3 nodes */
             printf("\n\t\t"); 
         }
     }
     printf("\n"); 
 }

 void dijkstra_add_my_edges_to_heap (vertex_node *vnode, heap_t *heap) {
     dijkstra_edge_nodes *both_enodes;
     edge_node *enode;

     for (enode = vnode->list_enode; enode != NULL; enode = enode->next_enode) {
         if (((vertex_node *)enode->parent_vnode)->color == COLOR_BLACK) {
             continue; /* Already added. Skip it. */
         }
         both_enodes = (dijkstra_edge_nodes *)malloc(sizeof(dijkstra_edge_nodes));
         if (!both_enodes) {
             return;
         }
         both_enodes->vnodeA = vnode;
         both_enodes->vnodeB = enode->parent_vnode;
         both_enodes->weight = enode->weight;
         heap_add(heap, (void *)both_enodes);
     }
 }

 void dijkstra_add_edge_to_distance_array (dijkstra_edge_nodes *both_enode) { 
     vertex_node *vnode = NULL;
     edge_node *enode;
     int new_vnode_index, vnode_index, new_dist, old_dist;

     printf("Selecting Edge (%d-%d) with weight: %d\n",
         both_enode->vnodeA->val, both_enode->vnodeB->val, both_enode->weight);

     vnode = (both_enode->vnodeA->color == COLOR_WHITE) ? 
         both_enode->vnodeA : both_enode->vnodeB;
     new_vnode_index = vnode->misc_index;
     printf("Adding Vertex node %d (Distance Array index: %d) \n", 
         vnode->val, new_vnode_index);

     /* Walk through each attached edge node and then reevaluate the distance */
     for (enode = vnode->list_enode; enode != NULL; enode = enode->next_enode) {
         if (((vertex_node *)enode->parent_vnode)->color == COLOR_WHITE) {
             /* Don't bother with vertices that are not yet added to the tree */
             continue;
         }
         vnode_index = ((vertex_node *)enode->parent_vnode)->misc_index;
         new_dist = arr_distance[vnode_index][2] + enode->weight; 
         old_dist = arr_distance[new_vnode_index][2];
         if (new_dist < old_dist) { 
             printf("New distance (%d) is less than the earlier (%d). Update\n", 
                     new_dist, old_dist);
             arr_distance[new_vnode_index][1] = ((vertex_node *)enode->parent_vnode)->val;
             arr_distance[new_vnode_index][2] = new_dist;
         } else {
             printf("New distance (%d) is more than the earlier (%d). Skip\n", 
                     new_dist, arr_distance[new_vnode_index][2]); 
         }
     }
     dijkstra_print_distance_array(); 
 }

 void dijkstra_run (heap_t *heap, vertex_node *source_vertex) {
     dijkstra_edge_nodes *both_enode;
     vertex_node *vnode, *new_vnode;
     int err, i;

     /* First things first. Let us do some initialization. */
     for (i = 0; i < len_arr_distance; i++) {
         arr_distance[i][0] = 0;
         arr_distance[i][1] = 0; 
         arr_distance[i][2] = INT_MAX;
     }

     for (vnode = graph_root, i = 0; vnode != NULL; vnode= vnode->next_vnode, i++) {
         vnode->misc_index = i;
         vnode->color = COLOR_WHITE;
         arr_distance[i][0] = vnode->val;
     } 

     /* All set. Start the while loop at the source vertex */
     dijkstra_add_my_edges_to_heap(source_vertex, heap);
     source_vertex->color = COLOR_BLACK;
     arr_distance[source_vertex->misc_index][2] = 0;

     while (heap->heap_current_size) {
         /* Print the Heap */
         adjlist_print(graph_root);
         dijkstra_heap_print(heap);

         /* Extract the Minimum */
         both_enode = (dijkstra_edge_nodes *)heap_extract(heap, &err);
         if (!both_enode || !both_enode->vnodeA || !both_enode->vnodeB) {
             if (both_enode) {
                 free(both_enode);
                 break;
             }
         }

         /* Find the unmarked vertex. Add its edges to Heap. */
         if ((both_enode->vnodeA) && (both_enode->vnodeA->color == COLOR_WHITE)) {
             new_vnode = both_enode->vnodeA; 
         } else {
             if ((both_enode->vnodeB) && (both_enode->vnodeB->color == COLOR_WHITE)) {
                 new_vnode = both_enode->vnodeB; 
             }
         }

         if (new_vnode) {
             dijkstra_add_edge_to_distance_array(both_enode);
             new_vnode->color = COLOR_BLACK; /* Done with this node -- mark it black */
             dijkstra_add_my_edges_to_heap(new_vnode, heap);
         }
         free(both_enode);
         new_vnode = NULL;
     }
 }

 int main () {
     heap_t heap; 
     int vertices[] = {1, 2, 3, 4, 5, 6, 7};
     int edges[][3] = {{1, 2, 4}, {1, 3, 1}, {2, 4, 3}, {2, 5, 8}, {3, 4, 2}, {3, 6, 6}, {4, 5, 4}, {5, 7, 2}, {6, 7, 8}};
     vertex_node *source_vertex = NULL, *vnode = NULL; 
     int len_vertices, len_edges, i;

     /* Init the heap */
     heap_init(&heap, dijkstra_heap_compare);

     /* Add vertices. */
     len_vertices = sizeof(vertices)/sizeof(vertices[0]);
     len_arr_distance = len_vertices; 
     for (i = 0; i < len_vertices; i++) {
         adjlist_add_vertex(&graph_root, vertices[i], NULL);
     }

     /* Add the edges. */
     len_edges = sizeof(edges)/sizeof(edges[0]);
     for (i = 0; i < len_edges; i++) {
         adjlist_add_edge(graph_root, edges[i][0], edges[i][1], edges[i][2]);
     }

     /* Run the Dijkstra's Algorithm */
     dijkstra_run(&heap, graph_root);

     dijkstra_heap_print(&heap);

     /* Done with the Adjacency List. Free it */
     adjlist_free(&graph_root);
 }