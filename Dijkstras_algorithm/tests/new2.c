#include <stdio.h>
#include <stdlib.h>


/**
* Graph and adjacency list implementation
*/
// A structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
	char owner;
	int price;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency list
struct AdjList
{
    struct AdjListNode *head;
};
 
// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices
// in graph)
struct Graph
{
    int V;
	int E;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =
		(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph =
        (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
	graph->E = E;
 
    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array =
		(struct AdjList*) malloc(V * sizeof(struct AdjList));
 
    // Initialize each adjacency list as empty by
    // making head NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, char owner, int price)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
	newNode->owner = owner;
	newNode->price = price;
	graph->array[src].head = newNode;
}
 
// A utility function to print the adjacency list
// representation of graph
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\nAdjacency list of vertex %d\n", v);
        while (pCrawl)
        {
            printf(" %d -> %d; owner: %c; price: %d;\n", v, pCrawl -> dest, pCrawl -> owner, pCrawl -> price);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}


/**
* Priority queue implementation
*/
// C code to implement Priority Queue
// using Linked List
#include <stdio.h>
#include <stdlib.h>
 
// Node
typedef struct Qnode {
    struct AdjListNode* data;
 
    // Lower values indicate higher priority
    int priority;
 
    struct Qnode* next;
 
} QNode;
 
// Function to Create A New Node
QNode* newQNode(struct AdjListNode* d, int p)
{
    QNode* temp = (QNode*)malloc(sizeof(QNode));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
 
    return temp;
}
 
// Return the value at head
struct AdjListNode* peek(QNode** head)
{
    return (*head)->data;
}
 
// Removes the element with the
// highest priority form the list
void pop(QNode** head)
{
    QNode* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}
 
// Function to push according to priority
void push(QNode** head, struct AdjListNode* d, int p)
{
    QNode* start = (*head);
 
    // Create new Node
    QNode* temp = newQNode(d, p);
 
    // Special Case: The head of list has lesser
    // priority than new node. So insert new
    // node before head node and change head node.
    if ((*head)->priority > p) {
 
        // Insert New Node before head
        temp->next = *head;
        (*head) = temp;
    }
    else {
 
        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL &&
            start->next->priority < p) {
            start = start->next;
        }
 
        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }
}
 
// Function to check is list is empty
int isEmpty(QNode** head)
{
    return (*head) == NULL;
}


/**
* Dijkstras algorithm implementation
*/
void Dijkstra(struct Graph* graph, int root, char traveler)
{	
	int V = graph->V;
	int visited[V] = {0};
	int dist[V][2];					// 1. Assign dist[v] for all nodes = INT_MAX
	for(int i = 0; i < V; ++i)
	{
		dist[i] = 999999;
	}
	dist[root] = 0;					// 2. Assign dist[root] = 0

	QNode* pq;						// 3. Add all edges(nodes) to a priority queue.
	int price;
	for (int i = 0; i < V; ++i)
    {
        struct AdjListNode* pCrawl = graph->array[i].head;
		if(i == 0)
		{
			price = pCrawl->price;
			if(pCrawl->owner == traveler)
				price /= 2;
			pq = newQNode(pCrawl, price);
			pCrawl = pCrawl->next;
        }
		
		while (pCrawl)
        {
			price = pCrawl->price;
			if(pCrawl->owner == traveler)
				price /= 2;
			pq = push(&pq, pCrawl, price);
			pCrawl = pCrawl->next;
        }
    }
	
	while(isEmpty(&pq))				// 4. Loop on the queue as long as it's not empty.
	{
		int minVertex, minDist = 999999;	// 4.1 In every loop, choose the node with the minimum distance from the root node in the queue
		for(int i = 0; i < V; ++i)
		{
			if(minDist > dist[i])
				minDist = dist[i];
		}
	}
	

	// Add root vertex edges to pQueue. cheapes price = highest priority
	/*if(graph->array[root])
	{
		struct AdjListNode* temp = graph->array[root];
		int price = temp->price;
		if(temp->owner == traveler)
			price /= 2;
		QNode* pq = newQNode(temp, price);
		temp = temp->next;
		
		while(temp)
		{
			price = temp->price;
			if(temp->owner == traveler)
				price /= 2;
			push(&pq, temp, price);
			temp = temp->next;
		}
		
		while()
		{
			
		}
	}
	
	// Mark root vertex as visited 
	visited[root] = 1;*/
}


//========================= Function prototypes ==================
void readData(struct Graph** graph, int* V, int* edges, int* s, int* g, char* t);

int main()
{
	int vertices, edges, start, goal; char traveler;
	struct Graph* graph;
	
	readData(&graph, &vertices, &edges, &start, &goal, &traveler);
	
				// Create a Priority Queue
				struct AdjListNode* temp;
				temp = graph->array[1].head;
				QNode* pq = newQNode(temp, temp->price);
				temp = temp->next;
				while(temp)
				{
					push(&pq, temp, temp->price);
					temp = temp->next;
				}
			 
				while (!isEmpty(&pq)) {
					printf("%d ", (peek(&pq))->dest);
					pop(&pq);
				}
	
	printGraph(graph);
	
    struct AdjList* rez = 
		(struct AdjList*) malloc(vertices * sizeof(struct AdjList));
	rez->head = NULL;
	
	//addEdge(graph, start, start, 0, 0);
	/*if(!cheapestPath(graph, start, goal, rez))
		printf("Path does not exist:\n");
	else
	{
		printf("\nPath found\n");
			
		reverseList(&rez->head);
		struct AdjListNode* pCrawl = rez->head;
		while(pCrawl)
		{
			printf("%d\n", pCrawl->dest);
			pCrawl = pCrawl->next;
		}
	}*/
 
    return 0;
}


//============================== Functions ========================
void readData(struct Graph** graph, int* V, int* edges, int* s, int* g, char* t)
{
	FILE *data = NULL;
	data = fopen("data.txt", "r");
	
	if(data != NULL)
	{
		fscanf(data, "%d %d\n", V, edges);
		*graph = createGraph(*V);
		
		int src, dest, price; char owner;
		for(int i = 0; i < *edges; ++i)
		{
			fscanf(data, "[%d, %d] %c %d\n", &src, &dest, &owner, &price);
			addEdge(*graph, src, dest, owner, price);
		}
		
		fscanf(data, "\n%d -> %d %c", s, g, t);
	}
}