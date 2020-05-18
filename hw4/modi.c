/**
 HW4 of Algorithm 2020-1 by Jae Hong Cheon, 21400749, CSEE, HGU
 This source file implemented knapsack problem (0-1 : DP, B and B / fractional : Greedy)
 My program works for Greedy, DP, and Branch and Bound
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int tot_weight ;
float max_benefit ; // for greedy
int qLength ;

typedef struct _Item { // Knapsack item structure
    int benefit ; // benefit range : 1 ~ 300
    int weight ; // weight range : 1 ~ 100
    float b_w ; // to contain benefit/weight
}Item;

typedef struct _Vertex { // This structure is for Branch and Bound method
    int index ;     // index number of the Vertex
    int acc_benefit ;   // accumulated benefits 'til at that Vertex
    int acc_weight ; // accumulated weight 'til at that Vertex
    int bound ;     // Upper bound on the benefit we could achieve by expanding beyond this vertex
}Vertex;

typedef struct _cirQ {
    //int front ;
    //int rear ;
    Vertex *arr ;
}cirQ;

Vertex *PQ = NULL ;

int counting = 0;

Vertex
initQ(){
    int i ;
    pQueue = (Vertex*)calloc(sizeof(Vertex)*qLength) ;
    //pq->front = 0 ;
    //pq->rear = 0;
    for (i=0; i < qLength; i++) {
        pq->arr = (Vertex*)calloc(qLength, sizeof(Vertex)) ;
    }
    return pQueue ;
}
void
freeQ(){
    free(PQ) ;
    PQ = NULL ;
    count = 0 ;
    printf("free queue! ") ;
}
int
QisEmpty(cirQ *pq){
    return (count == 0) ;
}
int
isFull(){
    return (count == qLength -1) ;
}
/*
int
NextPosIdx(int pos){
    if(pos == qLength-1){
      //여기가 안넘어가네.
        printf("NextPosIdx pos == qLength-1");
        return 0;
    }
        
    else
        return pos+1 ;
}
 */
void
enqueue(Vertex* node){
    int i ;
    int insertIndex = 0;
    
    if (isFull()) {
        printf("full!\n") ;
        return ;
    }
    for(i = 0 ; i < count ; i++){
        if(pQueue[i].arr)
    }
    printf("start Enqueue something \n") ;
    pq->rear += NextPosIdx(pq->rear) ;
    pq->arr[pq->rear].index = node->index ;
    pq->arr[pq->rear].acc_weight = node->acc_weight ;
    pq->arr[pq->rear].acc_benefit = node->acc_benefit ;
    pq->arr[pq->rear].bound = node->bound ;
    printf("index %d, acc_weight %d, acc_benefit %d, bound %d\n",pq->arr[pq->rear].index,pq->arr[pq->rear].acc_weight,pq->arr[pq->rear].acc_benefit,pq->arr[pq->rear].bound ) ;
}

Vertex
dequeue(cirQ *pq){
    if (QisEmpty(pq)) {
        printf("Queue is Empty! \n") ;
        exit(-1) ;
    }
    pq->front = NextPosIdx(pq->front) ;
    return pq->arr[pq->front] ;
}

int
getbound(Vertex node, Item *item, int count, int W){
    if (node.acc_weight >= W) {
        return 0 ;
    }
    int current_bound = node.acc_benefit ;
    int idx = node.index + 1 ;
    int totweight = node.acc_weight ;
    
    while ((idx < count) && (totweight + item[idx].weight <= W)) {
        totweight += item[idx].weight ;
        current_bound += item[idx].benefit ;
        idx++ ;
    }
    if (idx < count /* Unlike while condition, weight > W */) {
        current_bound += (W - totweight) * (int)item[idx].b_w ;
    }
    return current_bound ;
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
mergeSort(Item *item, int start, int end){
    int mid ;
    if( start < end){
        mid = (start+end) / 2  ;
        mergeSort(item, start, mid) ;
        mergeSort(item, mid+1, end) ;
        merge(item, start, mid, end) ;
    }
}



void grdyMethod(Item *item, int count, int W) ;
void dpMethod(Item *item, int count, int W) ;
void bbMethod(Item *item, int count, int W) ;



// Make sure that at the main function,
// the output should make .txt file through FILE operation.

int
main(){
    int i, j, k ;
    int W = 0 ; // (number of items) * 40 ;
    int numOfitems = 0 ; // user input through scanf
    int** dpresult = { 0x0, } ;
    //clock_t start_main, end_main ; // Variables for time counting
    //float proctime = 0.f ;
    
    printf("Hi. This is 0-1 knapsack program using 3 methods.\n") ;
    printf("please input the number of items. : ") ;
    scanf("%d", &numOfitems) ;
    qLength = numOfitems ;
    W = numOfitems * 40 ;
    Item *items = (Item*)calloc( numOfitems+1 ,sizeof(Item) ) ;
    for ( i = 1 ; i <= numOfitems ; i++){
        items[i].benefit = rand() % 300 +1 ;
        items[i].weight = rand() % 100 +1 ;
        items[i].b_w = (float)items[i].benefit / items[i].weight ;
        
    }
    
    printf("dp: ") ;
    //start_main = clock() ;
    dpMethod(items, numOfitems, W) ;
    printf("\n") ;
    printf("greedy : ") ;
    grdyMethod(items, numOfitems, W) ;
    printf("\n") ;
    printf("Branch and Bound : ") ;
    bbMethod(items, numOfitems, W) ;
    //end_main = clock() ;
    // proctime = (float)(end_main-start_main) ; // %.3f dealing with round like this
    // needs to do millisecond
    free(items) ;
    
    return 0 ;
}
void
bbMethod(Item *item, int count, int W){
    int i, j, k;
    clock_t start_bb, end_bb ; // Variables for time counting
    float proctime = 0.f ;
    
    start_bb = clock() ;
    int tot_benefit = 0;
    
        mergeSort(item, 1, W) ;
        // make a queue of node
        cirQ q ;
        
        initQ(&q) ;
        
        Vertex u, v ;
        
        u.index = -1 ;
        u.acc_benefit = 0 ;
        u.acc_weight = 0 ;
        enqueue(&q, &u) ;
        while (!QisEmpty(&q)) {
            u = dequeue(&q) ;
            // If it is starting node, assign level 0
            if (u.index == -1)
                v.index = 0;
            //if there is nothing on next level
            if (u.index == count-1)
                continue;
            
            v.index = u.index + 1 ;
            
            v.acc_weight = u.acc_weight + item[v.index].weight ;
            v.acc_benefit = u.acc_benefit + item[v.index].benefit ;
            
            if (v.acc_weight <= W && v.acc_benefit > tot_benefit)
                tot_benefit = v.acc_benefit;
            
            v.bound = getbound(v, item, count, W);

            if (v.bound > tot_benefit)
                enqueue(&q, &v) ;

            v.acc_weight = u.acc_weight;
            v.acc_benefit = u.acc_benefit;
            
            v.bound = getbound(v, item, count, W);
            if (v.bound > tot_benefit)
                enqueue(&q, &v) ;
            
            
        }
        end_bb = clock() ;
        proctime = (float)(end_bb-start_bb)  ; // Unit : millisecond
        printf("%.3f, %d \n", proctime, tot_benefit) ;
        
}
void
grdyMethod(Item *item, int count, int W){
    int i = 0, capacity = W ;
    clock_t start_grdy, end_grdy ; // Variables for time counting
    float proctime = 0.f ;
    
    start_grdy = clock() ;
    while((clock() - start_grdy)/ CLOCKS_PER_SEC  < 900.f){ // for setting the time limit as 15 mins
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
        end_grdy = clock() ;
        proctime = (float)(end_grdy-start_grdy) ; // Unit : millisecond
        printf("%.3f / %.3f \n", proctime, max_benefit );
        max_benefit = 0.f; tot_weight = 0; // initialising global variables as 0 for branch and bound
        break ;
    }
    // if time is over 15 mins, then force terminate
    if((clock()-start_grdy) / CLOCKS_PER_SEC >= 900.f)
      printf("15 mins Time limit exceeded in Greedy Method! \n") ;
}

void
dpMethod(Item *item, int count, int W){
    int i = 0, j =0 ;
    clock_t start_dp, end_dp ; // Variables for time counting
    float proctime = 0.f ;
    int    **dp = calloc( W+1 ,sizeof(int *)) ; // storing the values in dp table. idx:0~W+1
    for( i = 0 ; i < W+1 ; i++){
        dp[i] = calloc(count+1 ,sizeof(int)) ; // initialising dp table.
    }
    start_dp = clock() ;
    
    while((clock()-start_dp)/ CLOCKS_PER_SEC  < 2){ // for setting the time limit as 15 mins
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
        
        end_dp = clock() ;
        proctime = (float)(end_dp-start_dp) ; // Unit : millisecond
        printf("%.3f, %d \n", proctime, dp[W][count]) ;
        
        free(dp) ;
        break ;
      
    }
    if( (clock()-start_dp)/ CLOCKS_PER_SEC  >= 2){
        // if time is over 15 mins, then force terminate
        printf("Time limit exceed in DP Method! Abnormally terminated. \n") ;
       }
}



