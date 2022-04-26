/**
4. Duotas kelių sąrašas: miestas, miestas, kelio savininkas, kaina. Rasti pigiausią kelionės iš vieno
duoto miesto į kitą duotą miestą maršrutą ir jo kainą (kainą sudaro tik važiavimo keliu kaina, be
to, savininkas duoda 50% nuolaidą, jei atvažiuota jam priklausančiu keliu). Numatyti atvejį, kad
toks maršrutas neegzistuoja. (grafo realizacija paremta kaimynystės sąrašais)
*/


// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>
 
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
    struct AdjList* array;
	int* visitedNodes;
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
struct Graph* createGraph(int V)
{
    struct Graph* graph =
        (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array =
      (struct AdjList*) malloc(V * sizeof(struct AdjList));
	  
	graph->visitedNodes =
		(int*) malloc(V * sizeof(int));
 
    // Initialize each adjacency list as empty by
    // making head NULL, set visited status to 0
    int i;
    for (i = 0; i < V; ++i)
	{
        graph->array[i].head = NULL;
		graph->visitedNodes[i] = 0;
	}
 
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


//========================= Function prototypes ==================
void readData(struct Graph** graph, int* V, int* edges, int* s, int* g, char* t);
int cheapestPath(struct Graph* graph, int currentVertex, int goal, struct AdjList* rez);
int cheapestAdjVertex(struct Graph* graph, int currentVertex);
void addToRez(struct AdjList* rez, int vertex);
void removeFromRez(struct AdjList* rez, int vertex);
void reverseList(struct AdjListNode** head_ref);


//=============================== Main ===========================
int cost = 0;

int main()
{
	int vertices, edges, start, goal; char traveler;
	struct Graph* graph;
	
	readData(&graph, &vertices, &edges, &start, &goal, &traveler);
	
	printGraph(graph);
	
    struct AdjList* rez = 
		(struct AdjList*) malloc(vertices * sizeof(struct AdjList));
	rez->head = NULL;
	
	//addEdge(graph, start, start, 0, 0);
	if(!cheapestPath(graph, start, goal, rez))
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
	}
 
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

//Dijkstras algorithm
int cheapestPath(struct Graph* graph, int currentVertex, int goal, struct AdjList* rez)
{	
	//If the current vertex is the goal, add it to rezList and return from recursion
	if(currentVertex == goal)
	{
		graph->visitedNodes[currentVertex] = 1;
		addToRez(rez, currentVertex);
		return 1;
	}
	else
	{
		//In every loop, choose the cheapest adjacent vertex
		int nextVertex = cheapestAdjVertex(graph, currentVertex);
		if(nextVertex == -1)
			return 0;
		
		//mark current vertex as visited;
		graph->visitedNodes[currentVertex] = 1;
		
		addToRez(rez, currentVertex);
		
	
		if(cheapestPath(graph, nextVertex, goal, rez))
		{
			if(cost > (nextVertex = cheapestAdjVertex(graph, currentVertex)))
			{
				printf("found cheaper\n");
			}
			else
				return 1;
		}
		removeFromRez(rez, currentVertex);
		graph->visitedNodes[currentVertex] = 1;
		
	}
	printf("Fuk go back\n");
	return 0;
}

int cheapestAdjVertex(struct Graph* graph, int currentVertex)
{
	int min = 9999999, nextVertex = -1;
	
	struct AdjListNode* pCrawl = graph->array[currentVertex].head;
	while(pCrawl)
	{
		if(min > pCrawl->price && graph->visitedNodes[pCrawl->dest] == 0)
		{
			min = pCrawl->price;
			nextVertex = pCrawl->dest;
		}
		pCrawl = pCrawl->next;
	}
	printf("%d\n", nextVertex);
	return nextVertex;
}

void addToRez(struct AdjList* rez, int vertex)
{
	struct AdjListNode* newNode =
		(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = vertex;
	newNode->next = rez->head;
	rez->head = newNode;
	
			/*newNode->dest = vertex;
			newNode->next = NULL;
			
			struct AdjListNode* lastNode = rez->head;
			while(lastNode->next)
			{
				lastNode = lastNode->next;
			}
			lastNode->next = newNode;*/
}

void removeFromRez(struct AdjList* rez, int vertex)
{
	struct AdjListNode* temp;
	temp = rez->head;
	rez->head = rez->head->next;
	free(temp);
}

void reverseList(struct AdjListNode** head_ref)
{
    struct AdjListNode* prev = NULL;
    struct AdjListNode* current = *head_ref;
    struct AdjListNode* next = NULL;
    while (current != NULL) {
        // Store next
        next = current->next;
 
        // Reverse current node's pointer
        current->next = prev;
 
        // Move pointers one position ahead.
        prev = current;
        current = next;
    }
    *head_ref = prev;
}