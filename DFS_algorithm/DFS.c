/**
7. Duota informacija, kokios žmonių poros yra pažįstamos. 
Patikrinti, ar, pavyzdžiui, Jonas gali perduoti Marytei laiškelį per pažįstamus. 
(grafo realizacija paremta kaimynystės sąrašais; naudoti paieškos į gylį metodą)
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
	int dest;
	struct Edge* next;
} Edge;

typedef struct Graph {
	int numVertices;
	int* visited;
	Edge** adjLists;						// Pointer to an array of Edge pointers for a vertex
} Graph;

Edge* createEdge(int dest)					// Create new Edge to destination and poin to next Edge for this vertex
{
	Edge* newNode = (Edge*) malloc(sizeof(Edge));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

Graph* createGraph(int vertices)			// Create new graph with number of vertices, array of lists for edges, array for marking visited vertices
{
	Graph* graph = (Graph*) malloc(sizeof(Graph));
	graph->numVertices = vertices;

	graph->adjLists = (Edge**) malloc(vertices * sizeof(Edge*));

	graph->visited = (int*) malloc(vertices * sizeof(int));

	for (int i = 0; i < vertices; i++)
	{
		graph->adjLists[i] = NULL;
		graph->visited[i] = 0;
	}
	return graph;
}

void addEdge(Graph* graph, int src, int dest)		// Add doublesided edge
{
	Edge* newNode = createEdge(dest);				// Add edge from src to dest
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;

	newNode = createEdge(src);						// Add edge from dest to src
	newNode->next = graph->adjLists[dest];
	graph->adjLists[dest] = newNode;
}

void printGraph(Graph* graph)						// Print adjacency lists of every vertex in graph
{
	for (int v = 0; v < graph->numVertices; ++v)
	{
		Edge* temp = graph->adjLists[v];
		printf("\n Adjacency list of vertex %d\n ", v);
		while (temp)
		{
			printf("%d -> ", temp->dest);
			temp = temp->next;
		}
		printf("\n");
	}
}


//========================= Function prototypes =======================
void readFile(Graph** graph, int* vCount, int* person1, int* person2);
void DFS(Graph* graph, int vertex);


int main()
{
	int vCount, person1, person2;
	Graph* graph;
	
	readFile(&graph, &vCount, &person1, &person2);
	//printGraph(graph);
	
	if(graph != NULL)
	{
        DFS(graph, person1);					// Go through all connections of person1 and mark visited vertices
		
		if(graph->visited[person2] == 1)		// If person2 vertex was visited then person1 is connected to person2
            printf("Letter can be sent\n");
        else
            printf("Letter can't be sent\n");
    }
	else
		printf("Graph is empty\n");

    return 0;
}


//=========================== Functions ===========================
void readFile(Graph** graph, int* vCount, int* person1, int* person2)
{
	FILE *data = NULL;
    data = fopen("data.txt", "r");

    if (data != NULL)
	{
		int eCount;
		fscanf(data, "%d %d", vCount, &eCount);			// get vertex and edge count
		*graph = createGraph(*vCount);
		
		int src, dest;
		for (int i = 0; i < eCount; ++i)
		{
            fscanf(data, "%d %d", &src, &dest);
            addEdge(*graph, src, dest);
        }
		
		fscanf(data,"\n%d %d", person1, person2);
	}
	else
        printf("File failed to open\n");
}

void DFS(Graph* graph, int vertex)
{
	Edge* temp = graph->adjLists[vertex];				// Get list of edges for vertex
	
	graph->visited[vertex] = 1;							// Mark current vertex as visited

	while (temp != NULL)								// Go through all edges for current vertex
	{
		int adjVertex = temp->dest;

		if (graph->visited[adjVertex] == 0)				// If Vertex is not visited get its edges and check through them
			DFS(graph, adjVertex);

		temp = temp->next;
	}
}


