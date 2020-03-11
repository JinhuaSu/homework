#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){
    MPI_Init(NULL,NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int token, prev, next;
    prev = world_rank - 1;
    next = world_rank + 1;

    if(world_rank == 0){
        prev = world_size - 1;
    }

    if(world_rank == world_size - 1){
        next = 0;
    }
    if(world_rank == 0){
        token = -1;
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Process %d sent token %d to process %d\n", world_rank, token, next);
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d to process %d\n", world_rank, token, prev);
    }else{
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d to process %d\n", world_rank, token, prev);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Process %d sent token %d to process %d\n", world_rank, token, next);
    }

    MPI_Finalize();
}
