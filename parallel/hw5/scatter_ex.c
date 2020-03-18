#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    MPI_Init(NULL,NULL);

    int world_size, world_rank, *sendbuf, *recvbuf;
    int recvbuf_proc3;
    int new_rank;
    int ranks[3] = {0,1,2};

    MPI_Group world_group, new_group;
    MPI_Comm new_comm;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_rank == 1){
        sendbuf = (int*)malloc(10 *sizeof(int));
        for(int i =0; i < 10; i++){
            sendbuf[i] = i+1;
        }
    }

    if(world_rank < 3){
        recvbuf = (int *)malloc(3*sizeof(int));
    }

    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Group_incl(world_group, 3, ranks, &new_group);

    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

    if(new_comm != MPI_COMM_NULL){
        MPI_Scatter(sendbuf, 3, MPI_INT, recvbuf, 3, MPI_INT, 1, new_comm);
        printf("Process %d received numbers %d %d %d \n", world_rank, recvbuf[0], recvbuf[1], recvbuf[2]);
    }

    if(world_rank == 1){
        MPI_Send(&(sendbuf[9]), 1, MPI_INT, 3, 888, MPI_COMM_WORLD);
    }

    if(world_rank == 3){
        MPI_Recv(&recvbuf_proc3, 1, MPI_INT, 1, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d\n", world_rank, recvbuf_proc3);
    }
}