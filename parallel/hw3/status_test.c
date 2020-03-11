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
    
    const int MAX_NUMBERS = 100;
    int numbers[MAX_NUMBERS];
    int number_amount, i;
    if(world_rank == 0){
        srand(time(NULL));
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

        for(i=0; i<number_amount; i++){
            numbers[i] = 1 + 2*i;
        }

        MPI_Send(numbers, number_amount, MPI_INT, 1, 888, MPI_COMM_WORLD);

        printf("0 sent %d numbers to 1\n", number_amount);
    }else if(world_rank == 1){
        MPI_Status status;
        MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &number_amount);
        printf("1 received %d numbers from the source process = %d, and the message tag is %d\n", number_amount, status.MPI_SOURCE, status.MPI_TAG);
    }


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
