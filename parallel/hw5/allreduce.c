#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>

float mean(float *num, int length){
    float s = 0;
    for(int i = 0; i<length; i++){
        s+=num[i];
    }
    s /= length;
    return s;
}

int main(){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    float num[100], num_avg_per_proc, num_avg;

    for(int i=0; i<100; i++){
        num[i] = world_rank * 100 + i+1;
    }

    num_avg_per_proc = mean(num, 100);

    MPI_Allreduce(&num_avg_per_proc, &num_avg, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    num_avg /= 3;

    float sum_sq_per_proc = 0, sum_sq;
    for(int i = 0; i<100;i++){
        sum_sq_per_proc += (num[i] - num_avg) * (num[i] - num_avg);
    }
    MPI_Reduce(&sum_sq_per_proc,&sum_sq, 1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);

    if(world_rank == 0){
        float sd;
        sd = sqrt(sum_sq / (300 -1));
        printf("The standarded deviation is %f\n",sd);
    }
    MPI_Finalize();
}