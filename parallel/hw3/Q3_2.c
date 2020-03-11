#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
double ** make2Darray(int a1, int a2){
    double *tmp_vec = (double *) malloc(a1 * a2 * sizeof(double));
    double **tmp;
    tmp = (double **)malloc(a1*sizeof(double *));
    for(int i=0; i < a1; i++){
        tmp[i] = &(tmp_vec[i*a2]);
    }
    return tmp;
}
void delete2Darray(double **tmp, int a1, int a2){
    free(tmp[0]);
    free(tmp);
}
int main(int argc, char** argv){
    MPI_Init(NULL,NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int ncol=5;
    if(world_rank == 0){
        srand(time(NULL));
        int nrow = rand() / (double)RAND_MAX * 6 + 5; 
        double ** matr = make2Darray(nrow, ncol);
        for(int i=0; i < nrow; i++){
            for(int j=0; j<ncol; j++){
                matr[i][j] = i *j *0.5;
            }
        }
        MPI_Send(&(matr[0][0]), nrow*ncol, MPI_DOUBLE, 1, 888, MPI_COMM_WORLD);
        printf("0 sent %d x %d matrix to 1\n", nrow,ncol);
        delete2Darray(matr,nrow,ncol);
    }else if(world_rank == 1){
        MPI_Status stat;
        int number_amount;
        MPI_Probe(0,888,MPI_COMM_WORLD, &stat);
        MPI_Get_count(&stat, MPI_DOUBLE, &number_amount);
        int nrow = number_amount / ncol;
        double ** matr = make2Darray(nrow, ncol);
        MPI_Recv(&(matr[0][0]), nrow*ncol, MPI_DOUBLE, 0, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0; i<nrow; i++){
            for(int j = 0; j<ncol; j++){
                printf(" %f ",matr[i][j]);
            }
            printf("\n");
        }
        delete2Darray(matr,nrow,ncol);
    }
    MPI_Finalize();
}
