#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

void my_gather(void* send_data, int send_num, MPI_Datatype send_type, \
void* recv_data, int recv_num, MPI_Datatype recv_type, int root, MPI_Comm communicator){
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);
    if(world_rank == root){
        int size;
        MPI_Type_size(send_type,&size);
        void * tmp;
        char * tmp2,*tmp3;
        tmp = (void *)malloc(size * send_num);
        for(int i=0;i<world_size;i++){
            if(i != root){
                MPI_Recv(tmp,send_num,send_type,i,i,communicator,MPI_STATUS_IGNORE);
            }else{
                tmp = send_data;
            }
            for(int j=0;j<size*send_num;j++){
                tmp2 = recv_data+i*size*send_num+j;
                tmp3 = tmp+j;
                *tmp2 = *tmp3;
            }
        }
        free(tmp);
    }else{
        MPI_Send(send_data,send_num,send_type,root,world_rank,communicator);
    }
}

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

    my_gather(send_data,send_num,MPI_INT,recv_data,send_num,MPI_INT,2,MPI_COMM_WORLD);

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