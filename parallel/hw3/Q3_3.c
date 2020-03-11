#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define data_len 6

void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator){
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);
    if(world_rank == root){
        for(int rank = 0; rank < world_size; rank++){
            if(rank != root){
                MPI_Send(data,count,datatype,rank,rank,communicator);
            }
        }
    }else{
        MPI_Recv(data,count,datatype,root,world_rank,communicator,MPI_STATUS_IGNORE);
    }
}
int main(int argc, char** argv){
    MPI_Init(NULL,NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int data[data_len];
    if(world_rank == 0){
        int data[data_len] = {1,2,3,6,8,10};
    }
     
    my_bcast(&data, data_len, MPI_INT,0, MPI_COMM_WORLD);

    if(world_rank == 0){
        printf("Process 0 broadcasted data.\n");
    }else{
        printf("Process %d successfully received %d-data array ", world_rank, data_len);
        for(int i =0; i<data_len; i++){
            printf("%d ", data[i]);
        }
        printf("from root process\n");
    }

    MPI_Finalize();
}
