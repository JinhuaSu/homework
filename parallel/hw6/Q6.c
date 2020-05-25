#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define N 100
#define PI 3.141592654

double runif(){ 
	double tmp;
	do{
		tmp = (double) rand() / (double) RAND_MAX;
	}while(tmp>=1 || tmp <=0);
	return tmp;
} 
double rnorm(double mean, double sd){
	double tmp;
	double U, V;
	U = runif();
	V = runif();
	tmp = sqrt(-2.0*log(U)) * sin(2.0*PI*V);	
	tmp = mean + sd*tmp;
	return tmp;
}
double** make2Darray(int a1, int a2, int generate_data){
	double *tmp_vec = (double *)malloc(a1*a2*sizeof(double));
	double **tmp;
	tmp = (double **)malloc(a1*sizeof(double *));
	for(int i=0; i<a1; i++){
        if(generate_data == 1){
            for(int j=0;j<a2;j++){
                tmp_vec[i*a2+j] = rnorm(0,1);
            }
        }
		tmp[i] = &(tmp_vec[i*a2]);
	}
	return tmp;
}
//free a data matrix
void delet2Darray(double **tmp){
	free(tmp[0]);
	free(tmp);
}

int main(){
    MPI_Init(NULL,NULL);
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	double t1, t2;
	MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank == 0){
		t1 = MPI_Wtime();
	}
    srand(123);
    double **A, **B, **partial_A,**partial_res,**res;
    partial_A  = make2Darray(N/world_size,N,0);
    if(world_rank == 0){
        A = make2Darray(N,N,1);
        B = make2Darray(N,N,1);
    }else{
        A = make2Darray(N,N,0);
        B = make2Darray(N,N,0);
    }
    MPI_Bcast(B[0],N*N,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Scatter(A[0],N*N/4,MPI_DOUBLE,partial_A[0],N*N/4,MPI_DOUBLE,0,MPI_COMM_WORLD); //MAYBE BUG2
    partial_res  = make2Darray(N/world_size,N,0);
    res  = make2Darray(N,N,0);
    for(int i=0;i<N/world_size;i++){
        for(int j=0;j<N;j++){
            for(int k=0;k<N;k++){
                if(k == 0){
                    partial_res[i][j] = partial_A[i][k] * B[j][k];
                }else{
                    partial_res[i][j] += partial_A[i][k] * B[j][k];
                }
            }
        }
    }
    MPI_Gather(partial_res[0],N*N/4,MPI_DOUBLE,res[0],N*N/4,MPI_DOUBLE,0,MPI_COMM_WORLD); // BUG3
	MPI_Barrier(MPI_COMM_WORLD);
    double res_3;
    if(world_rank == 0){
        res_3 = res[0][0] * res[1][1] * res[2][2] \
              + res[0][1] * res[1][2] * res[2][0] \
              + res[0][2] * res[1][0] * res[2][1] \
              - res[0][2] * res[1][1] * res[2][0] \
              - res[0][1] * res[1][0] * res[2][2] \
              - res[0][0] * res[1][2] * res[2][1];
		t2 = MPI_Wtime();
        printf("3rd order sequential master result is %f\nthe running time is %f seconds\n", res_3,t2-t1);
    }
    MPI_Finalize();
}