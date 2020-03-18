#include<mpi.h>
#include<stdio.h>

int main(){
    MPI_Init(NULL,NULL);

    int world_size, world_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int color = world_rank / 4;

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD,color, world_rank, &new_comm);


    int new_rank,new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);

    printf("*** world rank/size: %d/%d --- new rank/size: %d/%d\n",  world_rank,world_size, new_rank,new_size);

    MPI_Finalize();
}