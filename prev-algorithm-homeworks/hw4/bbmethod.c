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
    float bound ;     // Upper bound on the benefit we could achieve by expanding beyond this vertex
}Vertex;

//Vertex *PQ = NULL ;

// declare priority queue index as a global variable
int pqIndex = 0;

void
initQ(Vertex *pq){  // initiallising dynamically
    int i ;
    
    pq = (Vertex*)calloc( qLength+1 ,sizeof(Vertex) ) ;
    printf("initQ successful! \n") ;
    //pq->front = 0 ;
    //pq->rear = 0;
    /*for (i=0; i < qLength; i++) {
     pq->arr = (Vertex*)calloc(qLength, sizeof(Vertex)) ;
     }*/
}


int
QisEmpty(Vertex *pq){
    if (pqIndex < 0)
        return 1 ;
    else
        return 0 ;
}

int
QisFull(){
    if ((pqIndex + 1 > qLength))
        return 1 ;
    else
        return 0 ;
}

void
pq_swap(Vertex *a, Vertex *b){
    Vertex tmp = *a ;
    *a = *b ;
    *b = tmp ;
}

int
enqueue(Vertex *pq, Vertex node){ // insert function of PQ for B and B
    if( QisFull() ){
        printf("PQ is full! ") ;
        return 0;
    }
    
    pq[pqIndex] = node ;
    
    int current = pqIndex ;
    int parent = (pqIndex - 1 ) / 2 ;
    
    // Max-heapifying using swap function
    while ((current > 0) && (pq[current].bound > pq[parent].bound)) {
        pq_swap(&pq[current], &pq[parent]);
        current = parent;
        parent = (parent - 1) / 2;
    }
    pqIndex++;
    
    return 1;
}


Vertex
dequeue(Vertex *pq) {
    int current = 0;
    int leftChild = current * 2 + 1;
    int rightChild = current * 2 + 2;
    int maxNode = current;
    
    if (pqIndex <= 0) {
        Vertex temp;
        temp.acc_benefit = -1;
        return temp ;
    }
    
    // transfer the max value of PQ into top node
    Vertex top = pq[0];
    pqIndex--;
    
    // Like a stamp
    pq[0] = pq[pqIndex];
    
    
    
    while (leftChild < pqIndex) {  // to find the max benefit node
        if (pq[maxNode].bound < pq[leftChild].bound) {
            maxNode = leftChild;
        }
        if ((rightChild < pqIndex) && (pq[maxNode].bound < pq[rightChild].bound)) {
            maxNode = rightChild;
        }
        
        if (maxNode == current) {
            break;
        }
        // Max - heapifying
        else { // maxNode != current
            pq_swap(&pq[current], &pq[maxNode]);
            current = maxNode;
            leftChild = current * 2 + 1;
            rightChild = current * 2 + 2;
        }
    }
    
    return top;
}


/*
 void
 enqueue(Vertex* node){
 int i ;
 
 if (QisFull()) {
 printf("full! \n") ;
 return ;
 }
 // Insert the node index into last index
 int insertedValue = PQ[++pqIndex] = node.index ;
 
 PQ[0] =
 
 // find for an index which will be inserted
 for (i =0 ; i < pqIndex ; i++) {
 if (PQ[i].index > node.index) {
 continue ;
 }
 else
 break ;
 }
 insertIndex = i ;
 
 for (i = pqIndex-1 ; i>=insertIndex; i--) {
 PQ[i+1] = PQ[i] ;
 }
 PQ[insertIndex] = node ;
 pqIndex++ ;
 }
 void
 dequeue(){
 if (QisEmpty()) {
 printf("empty!  in dequeue \n") ;
 return ;
 }
 pqIndex-- ;
 }
*/
float
getbound(Vertex node, Item *item, int count, int W){
    if (node.acc_weight >= W) {
        return 0 ;
    }
    float current_bound = node.acc_benefit ;
    int idx = node.index + 1 ;
    int totweight = node.acc_weight ;
    
    while ((idx <= count) && (totweight + item[idx].weight <= W)) {
        totweight += item[idx].weight ;
        current_bound += item[idx].benefit ;
        idx++ ;
    }
    if (idx <= count ) {
        current_bound += (W - totweight) * item[idx].b_w ;
    }
    return current_bound ;
}



void grdyMethod(Item *item, int count, int W) ;
void dpMethod(Item *item, int count, int W) ;
void bbMethod(Item *item, int count, int W) ;
void merge(Item *item, int start, int mid, int end) ;
void mergeSort(Item *item, int start, int end) ;

// Make sure that at the main function,
// the output should make .txt file through FILE operation.

int
main(){
    int i, j, k ;
    int W = 0 ; // (number of items) * 40 ;
    int numOfitems = 0 ; // user input through scanf
    int** dpresult = { 0x0, } ;
    //clock_t start_main, end_main ; // Variables for time pqIndex
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
    //int tot_benefit = 0;
    
    mergeSort(item, 1, W) ;
    // make a Priority queue of node
    Vertex *PQ = (Vertex*)calloc( count+1 ,sizeof(Vertex) );
    //initQ(PQ) ;
    printf("initQ successful! \n") ;
    // 1st step : make a root node.
    /*Vertex u, v ;
    v.index = 0; v.acc_weight = 0 ; v.acc_benefit = 0 ;
    v.bound = getbound(v, item, count, W) ;
    enqueue(PQ, v) ;
    while (!QisEmpty(PQ)) {
        v = dequeue(PQ) ;
        if(v.bound > max_benefit){
            u.index = v.index +1 ;
            u.acc_weight = v.acc_weight + item[u.index].weight ;
            u.acc_benefit = v.acc_benefit + item[u.index].benefit ;
        
            if (u.acc_weight <= W && u.acc_benefit > max_benefit ) {
                max_benefit = u.acc_benefit ;
            }
            u.bound = getbound(u, item, count, W) ;
            
            if(u.bound > max_benefit )
                enqueue(PQ, u) ;
            u.acc_weight = v.acc_weight ;
            u.acc_benefit = v.acc_benefit ;
            u.bound = getbound(u, item, count, W) ;
            if( u.bound > max_benefit )
                enqueue(PQ, u) ;
        }
    }*/
    Vertex root;
    root.acc_weight = 0;
    root.acc_benefit = 0;
    root.index = 0;
    for(i = 1; i < count+1; i++){
        // to make a Vertex(0 , 0) of B and B knapsack
        if(tot_weight + item[i].weight > W){
            root.bound += (W - tot_weight) * item[i].b_w;
            break;
        }
        else {
            root.bound += item[i].benefit;
            tot_weight += item[i].weight;
        }
    }
    enqueue(PQ, root);

    // compute all benefits, bounds of nodes from branch and bound tree
    while( !QisEmpty(PQ) ){
        Vertex temp = dequeue(PQ);
        Vertex child[2];
        int index = temp.index + 1;
        
        if(temp.acc_benefit == -1)
            break;
        
        if(temp.bound < max_benefit)
            continue;
        
        if(temp.index == count)
            continue;
        
        // child[0] = left node(Promising)
        child[0].acc_weight = temp.acc_weight + item[index].weight ;
        child[0].acc_benefit = temp.acc_benefit + item[index].benefit ;
        // child[1] = right node(non-promising)
        child[1].acc_weight = temp.acc_weight ;
        child[1].acc_benefit = temp.acc_benefit ;

        //tasking for 2 times because it's not for sure if the node is the best or not.
        
        for(i = 0; i < 2; i++){
        
            child[i].index = index ;
            child[i].bound = child[i].acc_benefit ;
            // Even if children' weights might be larger than W,
            // keep going cuz it's still effective.
            if( W < child[i].acc_weight )
                continue ;

            // then take the weight and seek bound
            tot_weight = child[i].acc_weight ;
            for( j = index+1 ; j < count+1 ; j++ ){
                if(tot_weight + item[j].weight > W ){
                    child[i].bound += (W-tot_weight) * item[j].b_w ;
                    break ;
                }
                else {
                    child[i].bound += item[j].benefit;
                    tot_weight += item[j].weight;
                }
            }
            //max_benefit보다 작으면 continue
            if(child[i].bound < max_benefit) continue;
            
            //benefit을 구하고 max_benefit 보다 크면 max_benefit 업데이트
            if(child[i].acc_benefit > max_benefit)
                max_benefit = child[i].acc_benefit;
            //push 해줘
            enqueue(PQ, child[i]);

        }
    }

    /*
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
        
        
    }*/
    free(PQ) ;
    end_bb = clock() ;
    proctime = (float)(end_bb-start_bb)  ; // Unit : millisecond
    printf("%.3f, %d \n", proctime, max_benefit) ;
    
    max_benefit = 0.f ; tot_weight = 0; pqIndex = 0;
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

void
grdyMethod(Item *item, int count, int W){
    int i = 0, capacity = W ;
    clock_t start_grdy, end_grdy ; // Variables for time pqIndex
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
    clock_t start_dp, end_dp ; // Variables for time pqIndex
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



