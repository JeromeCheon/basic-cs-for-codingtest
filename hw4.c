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
float max_benefit ; 

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
		if(item[i].b_w >= item[j].b_w) // descending sorting order of bi/wi
			tmp[k++] = item[i++] ;
		else
			tmp[k++] = item[j++] ;
	}
	while(i <= mid)
		tmp[k++] = item[i++] ;
	while(j <= end)
		tmp[k++] = item[j++] ;

	for(l = start ; l <=end ; l++){
		item[l] = tmp[l] ;
	}
	free(tmp) ;
}

void
grdyMethod(Item *item, int count, int W){
	int i = 0, capacity = W ;
	float value = 0.f ; // to contain benefit/weight
	clock_t start, end ; // Variables for time counting
	float proctime = 0.f ;

	start = clock() ;
	while(1){ // for setting the time limit as 15 mins
		if((clock() - start) / CLOCKS_PER_SEC > 900.f){
			printf("Time limit exceed in Greedy Method! Abnormally terminated. \n") ;
			break ;
		}
		mergeSort(item, 1, count) ;
		// After sorting, add the items sequencially til not over the W
		i = 1 ;
		while( i <= count && W > tot_weight ){
			if(item[i].weight < capacity){
				max_benefit += item[i].benefit ;
				tot_weight += item[i].weight ;
				capacity -= item[i].weight ;
			}
			else{ // item[i].weight >= capacity
				max_benefit += item[i].b_w * capacity ;
				tot_weight += capacity ;
				break ;
			}
			i++ ;
		}
		end = clock() ;
		proctime = (float)(end-start) / 1000 ;
		printf("fractional proc/max_b : %.3f / %.3f \n", proctime, max_benefit );
		max_benefit = 0.f; tot_weight = 0; // initialising global variables as 0 for branch and boud 

		break ;
	}
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
	start = clock() ;

	while(1){ // for setting the time limit as 15 mins
		if((clock() - start)  > 900.0f){
			printf("Time limit exceed in DP Method! Abnormally terminated. \n") ;
			break ;
		}
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
		proctime = 1000 * (float)(end-start) / CLOCKS_PER_SEC ;
		printf("%.3f, %d \n", proctime, dp[W][count]) ;

		free(dp) ;
		break ;
	}
}
void
bbMethod(Item *item, int count, int W){
	int i, j, k;
	
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
		items[i].b_w = (float)items[i].benefit/items[i].weight ;
		items[i].bound = 0 ;
	}

	printf("dp: ") ;	
	//start = clock() ;
	dpMethod(items, numOfitems, W) ;
	printf("\n") ;
	printf("greedy : \n") ;
	grdyMethod(items, numOfitems, W) ;
	printf("\n") ;
	//end = clock() ;
	// proctime = (float)(end-start) ; // %.3f dealing with round like this
	// needs to do millisecond
	free(items) ;

	return 0 ;
}

