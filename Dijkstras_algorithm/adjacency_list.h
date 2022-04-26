 #ifndef __ADJACENCY_LIST_H___
 #define __ADJACENCY_LIST_H___

 #define COLOR_WHITE     1
 #define COLOR_GRAY      2
 #define COLOR_BLACK     3

 typedef struct vertex_node_ {
     int val;                         /* Holds an int. Can we extended, if needed. */
     void *data;                      /* Holds satellite data associated with this vertex */
     unsigned char       color;       /* For visited/unvisited state. */
     unsigned int        set_number;  /* For partitioned Graphs. See Kruskal's algorithm. */
     struct edge_node_   *list_enode; /* This is a linked list for all connected edges. */
     struct vertex_node_ *next_vnode; /* This is the vertical linked list for all vertices. */
 } vertex_node;

 typedef struct edge_node_ {
     void              *parent_vnode; /* Pointer to the parent vertex node. */
     int               weight;        /* Weight of the link. */
     struct edge_node_ *next_enode;   /* This is a linked list. */
 } edge_node;

 /* Add a Vertex to an Adjacency List */
 void adjlist_add_vertex(vertex_node **graph_root, int current_value, void *data); 

 /* Add an Edge to the Adjacency List */
 void adjlist_add_edge(vertex_node *graph_root, int vertexA, int vertexB, int weight);

 /* Free the Adjacency List */
 void adjlist_free(vertex_node **graph_root);

 /* Print the Adjacency List */
 void adjlist_print(vertex_node *graph_root); 
 #endif /* __ADJACENCY_LIST_H___ */