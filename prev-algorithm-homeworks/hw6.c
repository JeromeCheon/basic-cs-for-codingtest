/**
	This Program is to make a directed graph and to compute a transpose of the original graph.
	The source data file will be given as a txt file of 7x7 matrix by instructor.
	The goal is to build 2 features of transpose of G : 1. Adj array / 2. Adj list
	******* made by JaeHong Cheon, 21400749, 2020-1 Algorithm analysis class. ********
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void){
	int i, j, k ; // for loop variables.
	int vertices = 0 ; // read how many vertices exist in the file then allocate memories dynamically.
	char buf[256] ;
	FILE *fp = fopen("data.txt", "r") ; // reading the external data file
	if(fp != NULL){ // Once reading file successful, then count how many nodes are in.
		while(!feof(fp)){ // read until encounter End Of File point
			if(fgets(buf, sizeof(buf), fp) == NULL)
				break;
			vertices++ ;
			printf("%s \n", buf) ;
		}
	}
	vertices -= 1 ; // substract the alphabetical row
	printf("nodes : %d \n", vertices) ;

	fclose(fp) ;
	// make something
	return 0;
}

