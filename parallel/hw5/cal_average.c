#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

int main(int args,char ** arkws){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    float num[100], *num_avg;

    for(int i=0; i<100;i++){
        num[i] = world_rank*100+i+1;
    }

    if(world_rank == 0){
        num_avg = (float *)malloc(100*sizeof(float));
    }

    MPI_Reduce(num,num_avg,100,MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(world_rank == 0){
        for(int i=0; i <100; i++){
            num_avg[i] /= world_size;
            printf(" %f ", num_avg[i]);
        }
        free(num_avg);
    }
    MPI_Finalize();
}