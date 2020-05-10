/** 
  	HW4 of Algorithm 2020-1 
 	This source file implemented nothing (modify later)
	my program works for Greedy and DP 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int tot_weight ;
int max_benefit ; 

typedef struct _Item {
	int benefit ; // benefit range : 1 ~ 300
	int weight ; // weight range : 1 ~ 100 
	float b_w ; // to contain benefit/weight
	int bound;  
}Item;

void
mergeSort(Item *item, int start, int end){
	int mid ;
	if( start < end){
		mid = (start+end) / 2  ;
		mergeSort(item, start, mid) ;
		mergeSort(item, mid+1, end) ;
		merge(item, start, mid, end) ;
	}
}

void
merge(Item *item, int start, int mid, int end){
	int i = start, j = mid+1;
	int k = i , l = 0;
	Item *tmp = (Item*)calloc( end+1 ,sizeof(Item) ) ;
	while(i <= mid && j <= end){
		if(item[i].b_w <= item[j].b_w)
			tmp[k++] = item[i++] ;
		else
			tmp[k++] = item[j++] ;
	}
	while(i <= mid)
		tmp[k++] = item[i++] ;
	while(j <= end)
		tmp[k++] = item[j++] ;

	printf("Sorted : \n") ;
	for(l = start ; l <=end ; l++){
		item[l] = tmp[l] ;
		printf("%.3f ", item[l].b_w )  ;
	}
	printf("\n") ;	
	free(tmp) ;
}

void
grdyMethod(Item *item, int count, int W){
	int i = 0, j = 0 ;
	float value = 0.f ; // to contain benefit/weight
	clock_t start, end ; // Variables for time counting
	float proctime = 0.f ;
	mergeSort(item, 1, count) ;
}

void 
dpMethod(Item *item, int count, int W){
	int i = 0, j =0 ;
	clock_t start, end ; // Variables for time counting
	float proctime = 0.f ;
	int	**dp = calloc( W+1 ,sizeof(int *)) ; // storing the values in dp table. idx:0~W+1 
	for( i = 0 ; i < W+1 ; i++){
		dp[i] = calloc(count+1 ,sizeof(int)) ; // initialising dp table.
	}
	printf("finished init dp table \n");
	start = clock() ;
	for( i = 1 ; i < count+1 ; i++){
		for( j = 1 ; j < W+1 ; j++){
			if(item[i].weight <= j){
				if(item[i].benefit + dp[(j-item[i].weight)][i-1] > dp[j][i-1])
					dp[j][i] = item[i].benefit + dp[(j-item[i].weight)][i-1] ;
				else
					dp[j][i] = dp[j][i-1] ;
			}
			else
				dp[j][i] = dp[j][i-1] ;
		}
	}
	end = clock() ;
	proctime = (float)(end-start) / 1000 ;
	printf("%.3f, %d \n", proctime, dp[W][count]) ;

	free(dp) ;
}

// Make sure that at the main function, 
// the output should make .txt file through FILE operation. 

int
main(){
	int i, j, k ; 
	int W = 0 ; // (number of items) * 40 ;
	int numOfitems = 0 ; // user input through scanf
	int** dpresult = { 0x0, } ;
	//clock_t start, end ; // Variables for time counting
	//float proctime = 0.f ;

	printf("Hi. This is 0-1 knapsack program using 3 methods.\n") ;
	printf("please input the number of items. : ") ;
	scanf("%d", &numOfitems) ;

	W = numOfitems * 40 ;
	Item *items = (Item*)calloc( numOfitems+1 ,sizeof(Item) ) ; 
	for ( i = 1 ; i <= numOfitems ; i++){
		items[i].benefit = rand() % 300 +1 ;
		items[i].weight = rand() % 100 +1 ;
		items[i].b_w = (float)(items[i].benefit/items[i].weight) ;
		items[i].bound = 0 ;
		printf("test %d. \n",i ) ;
	}
	
	printf("before dp \n") ;	
	//start = clock() ;
	dpMethod(items, numOfitems, W) ;
	grdyMethod(items, numOfitems, W) ;
	//end = clock() ;
	// proctime = (float)(end-start) ; // %.3f dealing with round like this
	// needs to do millisecond
	free(items) ;

	return 0 ;
}

