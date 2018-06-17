/*Rafael Belchior*/
/*João Calisto*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/*********************************/
/*SUPORT STRUCTURES AND FUNCTIONS*/
/*********************************/

/*Structure to indentify the edge*/
typedef struct edge {
	int value;
	struct edge *next;
} *Edge;

/*Representation of an edge and its information regarding a DFS search*/
typedef struct node {
	int index;
	int depth, low, visited;
	struct node *parent;
	Edge edges;
} *nodeLink;

/*Representation of the graph*/
typedef struct graph {
	int nVertix, nEdge;
	int minFundamental, maxFundamental, nFundamental;
	nodeLink *nodes;
} *Graph;

Edge newEdge(int value, Edge next){
	Edge e = malloc(sizeof(struct edge));
	e->value = value;
	e->next = next;
	return e;
}

nodeLink newNode(int index){
	nodeLink x = malloc(sizeof(struct node));
	x->index = index;
	x->visited = FALSE;
	x->parent = NULL; 
	x->edges = NULL;
	return x;
}

Graph GRAPHinit(int nVertix, int nEdge){
	int i;
	Graph g = malloc(sizeof(struct graph));
	g->nVertix = nVertix;
	g->nEdge = nEdge;
	g->minFundamental = -1;
	g->maxFundamental = -1;
	g->nodes = malloc((nVertix + 1) * sizeof(nodeLink));
	for(i = 0; i <= nVertix; i++)
		g->nodes[i] = newNode(i);
	return g;
}

/*Creates a bidiretional link between the edges*/
void createConnection(Graph g, int vertix1, int vertix2){
	nodeLink node = g->nodes[vertix1];
	node->edges = newEdge(vertix2, node->edges);

	node = g->nodes[vertix2];
	node->edges = newEdge(vertix1, node->edges);

}

int getMinimum(int a, int b){
	if(a >= b) return b;
	return a;
}

/*********************************/
/*********************************/

void getFundamentals(Graph g, nodeLink node, int d){
	nodeLink auxNode;
	Edge auxEdge;

	int childCount, isArticulation;


	node->visited = TRUE;
	node->depth = d;
	node->low = d;
	childCount = 0;
	isArticulation = FALSE;

	for(auxEdge = node->edges; auxEdge != NULL; auxEdge = auxEdge->next){
		auxNode = g->nodes[auxEdge->value];
		if(auxNode->visited == FALSE){
			auxNode->parent = node;
			getFundamentals(g, auxNode, d+1);
			childCount++;
			if(auxNode->low >= node->depth)
				isArticulation = TRUE;
			node->low = getMinimum(node->low, auxNode->low);
		}
		else if (auxNode != node->parent)
			node->low = getMinimum(node->low, auxNode->depth);
	}
	if((node->parent != NULL && isArticulation) || (node->parent == NULL && (childCount > 1))){
		if(node->index < g->minFundamental || g->minFundamental == -1)
			g->minFundamental = node->index;
		if(node->index > g->maxFundamental || g->maxFundamental == -1)
			g->maxFundamental = node->index;
		g->nFundamental++;
	}
}




int main() {
	int i, nVertix, nEdge, raiz;

    int input1, input2;


 	scanf("%d %d", &nVertix, &nEdge);

    Graph g = GRAPHinit(nVertix, nEdge);
    
    scanf("%d %d", &input1, &input2);
    raiz = input1;

	/*Saves a connection between person number input1 and person number input2*/
    createConnection(g, input1, input2);

    for(i = 2; i <= nEdge; i++) {
        scanf("%d %d", &input1, &input2);
       
        createConnection(g, input1, input2); 
    }

    getFundamentals(g, g->nodes[raiz],1);
    printf("%d\n%d %d\n", g->nFundamental, g->minFundamental, g->maxFundamental);
    

  return 0;
}