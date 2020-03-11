#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int *send_data, *recv_data;
    int total_number = 100;
    int recv_num = total_number / world_size;

    if(total_number % world_size != 0){
        printf("Data must be equally partitioned! \n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    recv_data = (int *)malloc(sizeof(int)*recv_num);

    if(world_rank == 1){
        send_data = (int *)malloc(sizeof(int)*total_number);
        for(int i=0;i<total_number;i++){
            send_data[i] = i+1;
        }
    }

    MPI_Scatter(send_data,recv_num,MPI_INT,recv_data,recv_num,MPI_INT,1,MPI_COMM_WORLD);

    printf("Processor %d received numbers: from %d to %d\n", world_rank, recv_data[0],recv_data[recv_num-1]);

    free(recv_data);
    if(world_rank == 1){
        free(send_data);
    }

    MPI_Finalize();
}