#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

#define N 10000

int main(int argc, char** argv){
    MPI_Init(NULL,NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

    srand(time(NULL) * world_rank);

    int within_count = 0;
    double x,y;
    for(int i = 0; i < N; i++){
        x = rand() / (double)RAND_MAX;
        y = rand() / (double)RAND_MAX;
        if((x - 0.5)*(x - 0.5)+ (y - 0.5)*(y - 0.5) <= 0.25){
            within_count ++;
        }
    }
    if(world_rank == 0){
        int tmp;
        double pi;
        for(int i = 1; i < world_size; i++){
            MPI_Recv(&tmp,1,MPI_INT,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("Process 0 receive data from process %d.\n",i);
            within_count += tmp;
        }
        pi = (double)(within_count * 4) / (world_size * N);
        printf("Finally %d of %d random points fell within the circle. A Pi value calculated by Montcalo method is %f.\n", within_count, world_size *N, pi);
    }else{
        MPI_Send(&within_count,1,MPI_INT,0,world_rank,MPI_COMM_WORLD);
        printf("Process %d sends data to process 0.\n",world_rank);
    }
    MPI_Finalize();
}
