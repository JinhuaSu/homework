#include <stdio.h>
#include <omp.h>

int main(){
    int data=0;

    #pragma omp parallel num_threads(3)
    {   
        for(int i = 0; i < 100; i++){
            data = data + 1;
        }
    }
    printf("data = %d.\n", data);
    return 0;
}