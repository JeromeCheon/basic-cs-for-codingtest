/**
 HW4 of Algorithm 2020-1 by Jae Hong Cheon, 21400749, CSEE, HGU
 This source file implemented knapsack problem (0-1 : DP, B and B / fractional : Greedy)
 My program works for Greedy and DP solution only.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int tot_weight ;
float max_benefit ; // for greedy
int qLength ; // for branch and bound priority queue

// declare priority queue index as a global variable
int pqIndex = 0;

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

typedef struct _Result{
    float exec_time ;
    float final_benefit ;
}Result;

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



Result grdyMethod(Item *item, int count, int W) ;
Result dpMethod(Item *item, int count, int W) ;
Result bbMethod(Item *item, int count, int W) ;

int
main(){
    int i, k ;
    int W = 0 ; // (number of items) * 40 ;
    int numOfitems[] = {10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000} ; // number of items list
    Result DP[9], GREEDY[9] ;
    //Result BB[9] ;
    char *s1 = "Items" ;
    char *s2 = "Greedy" ;
    char *s3 = "D.P." ;
    FILE *fp ;
    
    clock_t start_main, end_main ; // Limiting total execution time
    fp = fopen("output.txt", "w") ; // makking output txt file
    if(fp == NULL){
        printf("file open error!") ;
        exit(-1) ;
    }
    //start_main = clock() ;
    printf("Hi. This is 0-1 knapsack program using 3 methods.\n") ;
    printf("%-s %40s %45s\n", s1, s2, s3) ;
    fprintf(fp,"%-s %40s %45s\n", s1, s2, s3 ) ;
    fclose(fp) ;
    k=0 ;
    while (k < 9) {
        fp = fopen("output.txt", "a") ;
        if (fp == NULL) {
            printf("file open error!") ;
            exit(-1) ;
        }
        qLength = numOfitems[k] ; // this is for branch and bound queue
        W = numOfitems[k] * 40 ;
        Item *items = (Item*)calloc( numOfitems[k]+1 ,sizeof(Item) ) ;
        for ( i = 1 ; i <= numOfitems[k] ; i++){
            items[i].benefit = rand() % 300 +1 ;
            items[i].weight = rand() % 100 +1 ;
            items[i].b_w = (float)items[i].benefit / items[i].weight ;
        }
        printf("%-6d \t\t ", numOfitems[k] ) ;
        fprintf(fp, "%-6d \t\t ", numOfitems[k] ) ;
        GREEDY[k] = grdyMethod(items, numOfitems[k], W) ;
        printf("%20.3f ms/$ %-15.3f\t ", GREEDY[k].exec_time, GREEDY[k].final_benefit ) ;
        fprintf(fp, "%20.3f ms/$ %-15.3f\t ", GREEDY[k].exec_time, GREEDY[k].final_benefit ) ;
        DP[k] = dpMethod(items, numOfitems[k], W) ;
        printf("%20.3f ms/$ %-15.3f \n", DP[k].exec_time, DP[k].final_benefit ) ;
        fprintf(fp, "%20.3f ms/$ %-15.3f \n", DP[k].exec_time, DP[k].final_benefit ) ;
        k++ ;
        
        if( (clock()-start_main)/ CLOCKS_PER_SEC >= 3600 ){// for setting the time limit as 1 hour in main function.
            printf("The execution time is over 1 hour. Terminate the program. \n") ;
            fclose(fp) ;
            free(items) ;
            
            return 0 ;
        }
        //bbMethod(items, numOfitems, W) ;
        fclose(fp) ;
        free(items) ;
    }
    return 0 ;
}

Result
grdyMethod(Item *item, int count, int W){
    int i = 0, capacity = W ;
    clock_t start_grdy, end_grdy ; // Variables for time counting
    float proctime = 0.f ;
    Result gdResult ;
    start_grdy = clock() ;
    // for setting the time limit as 15 mins
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
        if((clock()-start_grdy) / CLOCKS_PER_SEC >= 900 ){// for setting the time limit as 15 mins
            gdResult.exec_time = 0 ;
            gdResult.final_benefit = 0 ;
            max_benefit = 0.f; tot_weight = 0; // initialising global variables as 0 for branch and bound
            return gdResult ;
        }
        i++ ;
    }
    end_grdy = clock() ;
    proctime = (float)(end_grdy-start_grdy) ; // Unit : millisecond
    gdResult.exec_time = proctime ;
    gdResult.final_benefit = max_benefit ;
    max_benefit = 0.f; tot_weight = 0; // initialising global variables as 0 for branch and bound
    
    return gdResult ;
}

Result // A method using dp 'table'
dpMethod(Item *item, int count, int W){
    int i = 0, j =0 ;
    clock_t start_dp, end_dp ; // Variables for time counting
    float proctime = 0.f ;
    Result dpResult ;
    int **dp = calloc( W+1 ,sizeof(int *)) ; // storing the values in dp table. idx:0~W+1
    for( i = 0 ; i < W+1 ; i++){
        dp[i] = calloc(count+1 ,sizeof(int)) ; // initialising dp table.
    }
    start_dp = clock() ;
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
        if( (clock()-start_dp)/ CLOCKS_PER_SEC >= 900 ){ // for setting the time limit as 15 mins
            dpResult.exec_time = 0 ;
            dpResult.final_benefit = 0 ;
            free(dp) ;
            return dpResult ;
        }
        
    }
    
    end_dp = clock() ;
    
    proctime = (float)(end_dp-start_dp) ; // Unit : millisecond
    dpResult.exec_time = proctime ;
    dpResult.final_benefit = dp[W][count] ;
    
    free(dp) ;
    return dpResult ;
}

Result
bbMethod(Item *item, int count, int W){
    int i, j, k;
    clock_t start_bb, end_bb ; // Variables for time counting
    float proctime = 0.f ;
    Result bbResult ;
    start_bb = clock() ;
    
    // reuse mergesort again and do sort descending order of bi/wi
    mergeSort(item, 1, W) ;
    
    // make a Priority queue of node
    Vertex *PQ = (Vertex*)calloc( count+1 ,sizeof(Vertex) );
    
    // 1st step : make a root node. Then initialise with 0
    Vertex root;
    root.acc_weight = 0;
    root.acc_benefit = 0;
    root.index = 0;
    
    for(i = 1; i < count+1; i++){
        // to make a Vertex(0, 0) of B and B knapsack
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
    while( QisEmpty(PQ) != 1 ){ //until Q is empty
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
                    child[i].bound += item[j].benefit ;
                    tot_weight += item[j].weight ;
                }
            }
            // child node be less than max_benefit, okay continue
            if(child[i].bound < max_benefit)
                continue ;
            
            // if child node's benefit is larger than, then accumulate
            if(child[i].acc_benefit > max_benefit)
                max_benefit = child[i].acc_benefit ;
            
            // Insert into Queue again
            enqueue(PQ, child[i]) ;

        }
        if( (clock()-start_bb)/ CLOCKS_PER_SEC >= 900 ){ // for setting the time limit as 15 mins
            bbResult.exec_time = 0 ;
            bbResult.final_benefit = 0 ;
            free(PQ) ;
            return bbResult ;
        }
    }

    free(PQ) ;
    end_bb = clock() ;
    proctime = (float)(end_bb-start_bb)  ; // Unit : millisecond
 
    bbResult.exec_time = proctime ;
    bbResult.final_benefit = (int)max_benefit ;
    max_benefit = 0.f ; tot_weight = 0; pqIndex = 0;
    return bbResult ;
}
