#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int *send_data, *recv_data;
    int send_num = 20;

    send_data = (int *)malloc(sizeof(int)*send_num);

    for(int i=0;i<send_num;i++){
        send_data[i] = 20 * world_rank + i;
    }

    if(world_rank == 2){
        recv_data = (int*)malloc(sizeof(int)*send_num*world_size);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(send_data,send_num,MPI_INT,recv_data,send_num,MPI_INT,2,MPI_COMM_WORLD);

    if(world_rank == 2){
        printf("Processor 2 received numbers: \n");
        for(int i = 0; i<send_num*world_size; i++){
            printf(" %d ", recv_data[i]);
        }
        printf("\n");
        free(recv_data);
    }

    free(send_data);
    MPI_Finalize();

}