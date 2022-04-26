#include <stdlib.h>;
#include <stdio.h>;

typedef int Status;
typedef struct ENode//Graph adjacency list definition
{
    int adjVex;//A vertex adjacent to any vertex u
    int w;//The weight of the edge
    struct ENode* nextArc;//Point to the next edge node
}ENode;
typedef struct LGraph
{
    int n;//The current number of vertices of the graph
    int e;//The current number of edges of the graph
    ENode **a;//Point to a one-dimensional array of pointers
}LGraph;

int Choose(int *d, int *s,int n) //Select a node with the smallest weight to join the array s[] every time 
{
    int i,minpos,min;
    min=INFTY;
    minpos=-1;
    for(i=0;i<n;i++)
    {
        if(d[i]<min&&!s[i])
        {
            min=d[i];
            minpos=i;
        }
    }
    return minpos;
 } 
 Status Dijkstra(int v,int *d,int *path,LGraph *lg)//Dijkstra's algorithm to find the path 
 {
     int i,j,k,w;
     ENode *p;
    p=lg->a[v];//Work pointer 
     int *s;

     if(v<0||v>lg->n-1)
     {
         return ERROR;
     }

     s=(int*)malloc(sizeof(int)*lg->n);
     for(i=0;i<lg->n ;i++)
     {
         s[i]=0;
         path[i]=-1;
         d[i]=INFTY;
     }

     while(p)//Initialize 
     {
         d[p->adjVex ]=p->w ;
         if(p->adjVex!=v&&d[p->adjVex ]<INFTY)
         {
             path[p->adjVex ]=v;
         }
         p=p->nextArc ;
    } //Initialize each array 
     s[v]=1;
     d[v]=0;
     for(i=1;i<lg->n ;i++)
     {

         k=Choose(d,s,lg->n );
         if(k==-1)
         {
             continue;
         } //Determine whether a valid node is selected 
         s[k]=1;
         p=lg->a[k];
         if(p==NULL)
         {
             continue ;
         }
         while(p)
         {
             if(!s[p->adjVex ]&&d[k]+p->w <d[p->adjVex ])//Update d and path 
             {
                 d[p->adjVex ]=d[k]+p->w ;
                 path[p->adjVex ]=k;
             }
             p=p->nextArc ;
         }
     }
     return OK;
 }
 void fun(LGraph *lg)//This function is used to output the path 
 {
     int v,u;
     printf("please input u and v:\n");
    scanf("%d %d",&u,&v);
    int d[lg->n];
    int path[lg->n];
    Dijkstra(u,d,path,lg);
     printf("path: ");
     if(path[v]==-1)
     {
         printf("None\n");
         return ; 
     }
     while (path[v]!=-1)
      {
        printf("%d <--- ",v);
         v=path[v];
     }
     printf("%d\n",u);
 }
 
 
int main()
{
	
	return 0;
}