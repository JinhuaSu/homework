#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int *send_data, *recv_data;
    int total_number = 10000;
    int recv_num = total_number / world_size;
    
    if(world_rank == 1){
        send_data = (int *)malloc(sizeof(int)*total_number);
        for(int i=0;i<total_number;i++){
            send_data[i] = i+1;
        }
    }

    recv_data = (int *)malloc(sizeof(int)*recv_num);


    MPI_Scatter(send_data,recv_num,MPI_INT,recv_data,recv_num,MPI_INT,1,MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank == 1){
        free(send_data);
    }

    long* sum_array;
    if(world_rank == 0){
        sum_array = (long*)malloc(sizeof(long)*world_size);
    }

    long sum_data=0;
    for(int i=0;i<recv_num;i++){
        sum_data += (long)recv_data[i];
    }
    free(recv_data);

    MPI_Gather(&sum_data,1,MPI_LONG,sum_array,1,MPI_LONG,0,MPI_COMM_WORLD);

    if(world_rank == 0){
        long final_sum = 0;
        for(int i = 0; i<world_size; i++){
            printf("Processor 0 received sub-sum result from processor %d: %ld\n" , i,sum_array[i]);
            final_sum += sum_array[i]; 
        }
        free(sum_array);
        printf("Total sum is %ld\n", final_sum);
    }

    MPI_Finalize();

}