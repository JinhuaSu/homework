#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 100
int main(){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) + world_rank);
    struct{
        double val;
        int rank;
    } data[N];

    struct{
        double val;
        int rank;
    }reduced_data[N];

    for(int i =0;i<N;i++){
        data[i].val = (double)(rand())/RAND_MAX;
        data[i].rank = world_rank;
    }
    
    MPI_Reduce(data,reduced_data,N,MPI_DOUBLE_INT,MPI_MINLOC,0,MPI_COMM_WORLD);
    if(world_rank==0){
        printf("The minimum values and ranks of %d processes are", world_size);
        for(int i=0; i<N; i++){
            printf("(%f , %d)",reduced_data[i].val,reduced_data[i].rank);
        }
        printf("\n");
    }
    MPI_Finalize();

}