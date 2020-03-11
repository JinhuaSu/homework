#include<stdio.h>
#include<time.h>
int main(){

    clock_t start,end;
    start = clock();
    
    long res = 0;
    long count;
    for(count = 0;count <= 1e9;count++){
        res += count;
    }
    
    end = clock();
    double duration = (double)(end - start)/CLOCKS_PER_SEC;
    printf("The result is %ld, and this programm cost %f seconds.",res,duration);
}
