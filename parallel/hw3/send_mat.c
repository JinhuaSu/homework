#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

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

int main(){
    MPI_Init(NULL,NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

 
    int nrow = 2, ncol=3;
    double ** matr = make2Darray(nrow, ncol);

    if(world_rank == 0){
        for(int i=0; i < nrow; i++){
            for(int j=0; j<ncol; j++){
                matr[i][j] = i *j;
            }
            
        }
        MPI_Send(&(matr[0][0]), nrow*ncol, MPI_DOUBLE, 1, 888, MPI_COMM_WORLD);
    }

    if(world_rank == 1){
        MPI_Recv(&(matr[0][0]), nrow*ncol, MPI_DOUBLE, 0, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0; i<nrow; i++){
            for(int j = 0; j<ncol; j++){
                printf(" %f ",matr[i][j]);
            }
            printf("\n");
        }
    }
    delete2Darray(matr,nrow,ncol);

}
