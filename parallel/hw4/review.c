#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#define MYTYPE int

MYTYPE * MPI_Recv_dynamic(MPI_Datatype datatype, int source, int tag, MPI_Comm comm, int * count_number){
    MPI_Status stat;
    MPI_Probe(source, tag, comm, &stat);
    MPI_Get_count(&stat, datatype, count_number);
    MYTYPE *buf;
    buf=(MYTYPE *)malloc(sizeof(MYTYPE)* count_number[0]);
    MPI_Recv(buf, count_number[0],datatype,source,tag,comm, MPI_STATUS_IGNORE);
    return buf;
}

int main(int argc, char ** argv){
    MPI_Init(NULL,NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int number_amount, i;

    if(world_rank == 0){
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
        srand(time(NULL));
        number_amount = (rand()/(float)RAND_MAX) * MAX_NUMBERS;

        for(i=0;i<number_amount;i++){
            numbers[i] = i + 1;
        }

        MPI_Send(numbers,number_amount,MPI_INT,1,888,MPI_COMM_WORLD);
        printf(".\n");
    }else{
        int * number_buff;
        int count_num;
        number_buff = MPI_Recv_dynamic(MPI_INT, 0, 888, MPI_COMM_WORLD, &count_num);
        printf(",\n");
        for(i=0;i<count_num;i++){
            printf("%d ",number_buff[i]);
        }
        printf("\n");
    }
    MPI_Finalize();

}