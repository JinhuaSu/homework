#include<mpi.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    MPI_Init(NULL,NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    float tmp_sum = 0,avg,res,* array;
    int data_size[3] = {5,4,3};
    if(world_rank == 0){
        float tmp[5] = {12,23,34,45,56};
        array = tmp;
    }else if(world_rank == 1){
        float tmp[4] = {21,0.5,7,8};
        array = tmp;
    }else{
        float tmp[3] = {7.1,8.2,9.3};
        array = tmp;
    }
    for(int i=0;i<data_size[world_rank];i++){
        tmp_sum += array[i];
    }
    MPI_Allreduce(&tmp_sum,&avg,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);
    avg /= 12;
    tmp_sum = 0;
    for(int i=0;i<data_size[world_rank];i++){
        tmp_sum += pow(array[i] - avg,2);
    }
    MPI_Reduce(&tmp_sum,&res,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
    if(world_rank==0){
        printf("Final result is %f\n" , res);
    }
    MPI_Finalize();
}