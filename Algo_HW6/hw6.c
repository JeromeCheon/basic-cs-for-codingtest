/**
  This Program is to make a directed graph and to compute a transpose of the original graph.
  The source data file will be given as a txt file of 7x7 matrix by instructor.
  The goal is to build 2 features of transpose of G : 1. Adj list / 2. Adj list of transpose G
  
  ##### Lack of this program : I could't even resolve the ordering ascending to descending. ###
 ****** made by JaeHong Cheon, 21400749, 2020-1 Algorithm analysis class. ********
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** array = {0, } ; // for storing the edge number/existance from input file
char alphabet[20] ; // for storing the alphabet of vertices
short flag = 0 ; // to identify between nodes and edges when reading the file. 
int vertices = -1 ; // read how many vertices exist in the file then allocate memories dynamically.

/********************* Key Structures of Directed Graph ***********************/
typedef struct _Node{
	int index; // index of current head
	char vertex ;
	struct _Node* next ; // indicate next adjacent list node
} Node ;

typedef struct _adjList{
 	Node* head ;
	Node* tail ;
} adjList ;

typedef struct _digraph{
	int numVtx ;
	adjList* order ;
} digraph ;

/******************* Functions to make adjacent linked list and graph ***********************/
Node*
newNode(int index) {
	Node* new = (Node*)malloc(sizeof(Node)) ;
	new->index = index ;
	new->vertex = alphabet[index] ;
	new->next = NULL ;
	return new ;
}
void 			// will be connected by for loop in main function 
addEdge(digraph* g, int from, int index){
	int i = 0 ;
	if ( array[from][index] ){	// if there is the edge between from Vertex and index Vertex
		Node* new = newNode(index) ;	
		new->next = g->order[from].head ;
		g->order[from].head = new ;
	}
	else{
		// no any edge between vertices
		return ;
	}
}

void
transpose(digraph* g, int from, int index){ // similar with addEdge, but a little bit different
	int i = 0 ;
	if ( array [from][index] ){
		Node* new = newNode(from) ;
		new->next = g->order[index].head ;
		g->order[index].head = new ;
	}
	else 	// no any edges between vertices
		return ;
}

digraph*
graphInit(){
	int i, j ;
	digraph* g = (digraph*)malloc(sizeof(digraph)) ;	
	g->numVtx = vertices ;

	g->order = (adjList*)malloc( (g->numVtx) * sizeof(adjList) ) ;
	for ( i= 0 ; i < (g->numVtx) ; i++){
		g->order[i].head = NULL ;
		g->order[i].tail = NULL ;
	}
	return g ;
}

void
displayGraph(digraph* g){
	int i ;
	for ( i = 0 ; i < g->numVtx ; i++){
		Node* indicate = g->order[i].head ;
		printf( "The array of adjacency list  \'%c\'[%d] ", alphabet[i], i ) ;
		while (indicate != NULL){
			printf(" -> %c", indicate->vertex) ;
			indicate = indicate->next ;
		}
		printf("\n") ;
	}
	printf("\n") ;
}

void
freeGraph(digraph* g){ // deallocate the memory of graph
	int i ;
	Node *cur, *next ;
	for (i = 0; i < g->numVtx ; i++){
		cur = g->order[i].head ;
		while ( cur != NULL ){
			next = cur->next ;
			free(cur) ;
			cur = next ;
		}
	}
}

/************** reading input file and make a matrix for just in case *******************/
char *
removeBlank(char* string){
	int i, j;
	if (flag == 0){			// will be execute if string is the first line of input file 
		for (i = 0, j = 0; i< (int)strlen(string); i++, j++){
			if ( string[i] == ' ' || string[i] == '\t')
				j-- ;
			else
				alphabet[j] = string[i] ;
		}
	}
	char* renew = (char*)malloc(sizeof(string)) ;
	for ( i =0, j =0 ; i < (int)strlen(string); i++, j++){
		// if there are some white spaces at the string, just ignore
		if ( string[i] == ' ' || string[i] == '\t')
			j-- ;	
		// if there are some characters at the string, just ignore as well
		else if((string[i] >= 65 && string[i] <= 90) ){
			j-- ;
		}else
			renew[j] = string[i] ;
	}
	//free(renew) ;
	return renew ;
}

void
getMatrix(char* filename){
	int i ; // for loop variables.
	char buf[256] ;
	char extract[256] ;

	FILE *fp = fopen(filename, "r") ; // reading the external data file
	if(fp != NULL){ // Once reading file successful, then count how many nodes are in.
		while(!feof(fp)){ // read until encounter End Of File point
			if(fgets(buf, sizeof(buf), fp) == NULL)
				break;
			else if ( flag != 1 ){
				strcpy(extract, removeBlank(buf)) ;
				flag = 1 ;
			}else{	
				strcpy(extract, removeBlank(buf)) ;
				if ( vertices >= 0) {
					if ( array == NULL ){
					  array = (int**)malloc(sizeof(int*)*(strlen(extract))) ;
					  for ( i = 0 ; i < strlen(extract)-1 ; i++)
					  	array[i] = (int*) calloc(strlen(extract), sizeof(int)) ;
					}

					for ( i = 0 ; i < (int)strlen(extract) -2 ; i++){
						array[vertices][i] = extract[i] - '0'  ;
					}
				}
				extract[0] = 0x0 ;
			}
			vertices++ ;
		}
	}
	fclose(fp) ;
	buf[0] = 0x0 ;
}

/********************* Main function ***********************/
int
main(void){
	int i, j ;
	getMatrix("hw6_data.txt") ;
	if ( vertices > 20 ){
		printf("Wrong input file. The number of vertex exceeds 20! \n") ;
		exit(-1) ;
	}
	digraph* g = graphInit() ;		// declare and initialize original graph
	digraph* gT = graphInit() ; 	// declare and initialize transpose graph
	printf("At first, Read all Nodes from input file: ") ;
	for ( i= 0; i < vertices ; ++i)
		printf("%c ",alphabet[i]) ;
	printf("\n\n") ;

	for ( i = 0 ; i < vertices ; i++){
		for ( j = 0 ; j < vertices ; j++){
			addEdge(g, i, j) ;
			transpose(gT, i, j) ;
		}
	}
	printf("The original Graph : \n\n") ;
	displayGraph(g) ;
	freeGraph(g) ;
	
	printf("The transpose of Graph : \n\n") ;
	displayGraph(gT) ;
	freeGraph(gT) ;
	alphabet[0] = '\0';
	exit(0);
}


