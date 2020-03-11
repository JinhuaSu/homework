#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<assert.h>

int main(int argc, char** argv){
    if(argc != 3){
        fprintf(stderr, "Usage: compare_bcast number_elements number_trials\n");

        exit(1);
    }

    int number_elements = atoi(argv[1]);
    int number_trials = atoi(argv[2]);

    double total_my_bcast_time = 0.0;
    double total_mpi_bcast_time = 0.0;
    
    int i;
    int* data =(int*)malloc(sizeof(int)*number_elements);
    assert(data != NULL);

    if(world_rank == 0){
        for(i=0;i<number_elements;i++){
            data[i] = i*i;
        }
    }
}