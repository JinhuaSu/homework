#include<mpi.h>
#include<stdio.h>

int main(){
    MPI_Init(NULL,NULL);

    int world_size, world_rank;

    
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int color =  0 ? 1: world_rank == 0 || world_rank == 2 || world_rank == 4 || world_rank == 5;

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD,color, world_rank, &new_comm);


    int new_rank,new_size;
    MPI_Comm_rank(new_comm, &new_rank);

    printf("Group %d: world rank = %d, new rank = %d\n", color ,world_rank, new_rank);

    MPI_Finalize();
}