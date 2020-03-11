#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char** argv){
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

    double t1,t2,duration;
    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank == 0){
        t1 = MPI_Wtime();
    }
    long myvar,count,res;
    res = 0;
    count = world_rank;
    for(;count <= 1e9; count += 2){
        res += count;
    }
    if(world_rank == 0){
        MPI_Recv(&myvar,1,MPI_LONG,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        res += myvar;
    }else{
        MPI_Send(&res, 1, MPI_LONG,0,0,MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank == 0){
        t2 = MPI_Wtime();
        duration = t2 - t1;
        printf("The result is %ld, and this programm cost %f seconds.",res,duration);
    }
}
