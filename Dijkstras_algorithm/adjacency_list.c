#include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include "adjacency_list.h"

 bool adjlist_print_debug = true;

 /* Add a Vertex to an Adjacency List */
 void adjlist_add_vertex (vertex_node **graph_root, int current_value, void *data) {
     vertex_node *new_vnode, *vnode; 

     if (!graph_root)
         return;

     vnode = *graph_root;
     while (vnode && vnode->next_vnode) {
         if (vnode->val == current_value) {
             /* If the value already exists, then return. */
             printf("Vertex already exists. No need to add\n");
             return;
         }
         vnode = vnode->next_vnode;
     }

     if (adjlist_print_debug) 
         printf("[%s] Adding Vertex Node (%d)\n", __FUNCTION__, current_value);
     new_vnode = (vertex_node *)malloc(sizeof(vertex_node));
     if (!new_vnode) {
         fprintf(stderr, "[%s] Malloc failed \n", __FUNCTION__);
         return;
     }
     new_vnode->val = current_value;
     new_vnode->list_enode = NULL;
     new_vnode->next_vnode = NULL;
     new_vnode->color = 0;
     new_vnode->data = data;

     if (!*graph_root) {
         *graph_root = new_vnode;
         return;
     }
     vnode->next_vnode = new_vnode;
 }

 /* Add an Edge to a Vertex */
 void adjlist_internal_add_edge_to_vertex (vertex_node *first_vnode, 
                                           vertex_node *second_vnode, 
                                           int val_second_vertex, int weight) { 
     edge_node *enode, *prev_enode, *new_enode;

     if (!first_vnode || !second_vnode)
         return;

     prev_enode = enode = first_vnode->list_enode;
     while (enode) {
         if (((vertex_node *)enode->parent_vnode)->val == val_second_vertex) {
             if (adjlist_print_debug) 
                 printf("Edge is already added to this Vertex. No need to add\n");
             return;
         }
         prev_enode = enode;
         enode = enode->next_enode;
     }

     new_enode = (edge_node *)malloc(sizeof(edge_node));
     if (!new_enode) {
         fprintf(stderr, "[%s] Malloc failed \n", __FUNCTION__);
         return;
     }
     new_enode->parent_vnode = second_vnode;
     new_enode->next_enode = NULL;
     new_enode->weight = weight;
     if (prev_enode) {
         prev_enode->next_enode = (void *)new_enode;
     } else {
         /* prev_enode is NULL means new_enode is the first enode for this vertex */
         first_vnode->list_enode = (void *)new_enode;
     }
 }

 /* Add an Edge to the Adjacency List */
 void adjlist_add_edge (vertex_node *graph_root, int vertexA, int vertexB, int weight) {
     vertex_node *vertex_nodeA = NULL; 
     vertex_node *vertex_nodeB = NULL; 
     vertex_node *vnode;

     if (!graph_root)
         return;

     /* 
      * First, find the vnode for both vertices in the edge. We will 
      * add an enode to both of these vnodes.
      */
     for (vnode = graph_root; vnode != NULL; vnode = vnode->next_vnode) {
         if (vnode->val == vertexA) {
             vertex_nodeA = vnode;
         }
         if (vnode->val == vertexB) {
             vertex_nodeB = vnode;
         }
         if ((vertex_nodeA) && (vertex_nodeB)) {
             break;
         }
     }

     if (!vertex_nodeA) {
         printf("[%s] vertex_nodeA [%d] not found in list of vertices\n", __FUNCTION__, vertexA);
         return;
     }

     if (!vertex_nodeB) {
         printf("[%s] vertex_nodeB [%d] not found in list of vertices\n", __FUNCTION__, vertexB);
         return;
     }

     if (adjlist_print_debug) 
         printf("[%s] Adding Edge (%d-%d) to the adj list\n", __FUNCTION__, vertexA, vertexB);

     /* Add Edge to VertexA. When added, this edge would point to VertexB */
     if (adjlist_print_debug) 
         printf("\t[%s] Adding Edge (%d-%d) to Vertex Node (%d) \n", 
                     __FUNCTION__, vertexA, vertexB, vertexA);
     adjlist_internal_add_edge_to_vertex(vertex_nodeA, vertex_nodeB, vertexB, weight); 

     /* Add Edge to VertexB. When added, this edge would point to VertexA */
     if (adjlist_print_debug) 
         printf("\t[%s] Adding Edge (%d-%d) to Vertex Node (%d) \n", 
                     __FUNCTION__, vertexA, vertexB, vertexB);
     adjlist_internal_add_edge_to_vertex(vertex_nodeB, vertex_nodeA, vertexA, weight); 
 }

 /* Free the Adjacency List */
 void adjlist_free (vertex_node **graph_root) {
     vertex_node *vnode = NULL;
     vertex_node *prev_vnode = NULL;
     edge_node *enode = NULL;
     edge_node *prev_enode = NULL;

     if (!graph_root)
         return;

     vnode = *graph_root;
     /* Walk the list of vertex nodes. */
     while (vnode) { 
         /* For each vertex node, walk the list of edge nodes. */
         enode = vnode->list_enode;
         while (enode) {
             prev_enode = enode;
             enode = enode->next_enode;
             free(prev_enode);
         }
         prev_vnode = vnode;
         vnode = vnode->next_vnode;
         free(prev_vnode);
     }
     *graph_root = NULL;
 }

 /* Print the Adjacency List */
 void adjlist_print (vertex_node *graph_root) {
     vertex_node *vnode = NULL;
     edge_node *enode = NULL;

     printf("[%s] Print the Adjacency List", __FUNCTION__); 
     if (!graph_root) {
         printf("\n[%s] Nothing to print. Return\n", __FUNCTION__); 
         return;
     }

     for (vnode = graph_root; vnode != NULL; vnode = vnode->next_vnode) {
         printf("\n\tVertex [%3d][Color: %d]:", vnode->val, vnode->color);
         for (enode = vnode->list_enode; enode != NULL; enode = enode->next_enode) {
             printf(" -- Edge [%3d (%1d)]", ((vertex_node *)enode->parent_vnode)->val, enode->weight);
         }
     }
     printf("\n");
 }