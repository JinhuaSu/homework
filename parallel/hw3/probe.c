#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv){

    MPI_Init(NULL,NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size != 2){
        fprintf(stderr,"Must use two processes for this examples\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int number_amount, i;
    if(world_rank == 0){
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
        srand(time(NULL));
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

        for(i=0; i<number_amount; i++){
            numbers[i] = 1 + 2*i;
        }

        MPI_Send(numbers, number_amount, MPI_INT, 1, 888, MPI_COMM_WORLD);

        printf("0 sent %d numbers to 1\n", number_amount);
    }else if(world_rank == 1){
        MPI_Status stat;
        MPI_Probe(0,888,MPI_COMM_WORLD, &stat);
        MPI_Get_count(&stat, MPI_INT, &number_amount);
        int* number_buff = (int*) malloc(sizeof(int)*number_amount);

        MPI_Recv(number_buff, number_amount, MPI_INT, 0, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 dynamically received %d numbers from process 0.\n", number_amount);
        free(number_buff);
    }


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
