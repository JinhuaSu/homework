#include<mpi.h>
#include<stdio.h>

int main(){
    MPI_Init(NULL,NULL);
    int world_size, world_rank, sendbuf, recvbuf;
    int new_rank;
    int ranks[4] = {2,3,4,5};

    MPI_Group world_group , new_group;
    MPI_Comm new_comm;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    sendbuf = world_rank;

    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Group_incl(world_group, 4, ranks, &new_group);

    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

    if(new_comm != MPI_COMM_NULL){
        MPI_Comm_rank(new_comm, &new_rank);
        MPI_Reduce(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, 0, new_comm);
        printf("world rank = %d, new rank = %d\n",world_rank, new_rank);
    }
    if(new_rank == 0){
        printf("*** world rank = %d, new rank = %d, sum = %d\n", world_rank, new_rank,recvbuf);
    }
    MPI_Finalize();
}