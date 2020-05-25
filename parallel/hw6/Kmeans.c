#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 10000000
#define K 3
#define PI 3.141592654

//sample a random number on (0,1)
double runif(){ 
	double tmp;
	do{
		tmp = (double) rand() / (double) RAND_MAX;
	}while(tmp>=1 || tmp <=0);
	return tmp;
} 

//sample Gaussian-distributed random numbers via Box-Muller algorithm
double rnorm(double mean, double sd){
	double tmp;
	double U, V;
	U = runif();
	V = runif();

	tmp = sqrt(-2.0*log(U)) * sin(2.0*PI*V);	
	tmp = mean + sd*tmp;

	return tmp;
}

//define a data matrix
double** make2Darray(int a1, int a2){
	double *tmp_vec = (double *)malloc(a1*a2*sizeof(double));
	double **tmp;
	tmp = (double **)malloc(a1*sizeof(double *));
	for(int i=0; i<a1; i++){
		tmp[i] = &(tmp_vec[i*a2]);
	}
	return tmp;
}

//free a data matrix
void delet2Darray(double **tmp){
	free(tmp[0]);
	free(tmp);
}

//compute distances between data and centers
// and return which cluster each data point belongs to

int* compute_Z(double **dat, int n_dat, int p, double **center){
	double dist, dist_tmp;
	int *Z;
	Z = (int*) malloc(n_dat * sizeof(int));
	
	for(int i=0; i < n_dat; i++){
		for(int k=0; k < K; k++){
			dist_tmp = 0;
			for(int j=0; j<p; j++){
				dist_tmp += pow(dat[i][j] - center[k][j], 2);
			}
			if(k==0){
				dist = dist_tmp;
				Z[i] = 0;
			}else if(dist_tmp < dist){
				dist = dist_tmp;
				Z[i] = k;
			}	 
		}
	}

	return Z;
}

//compute the centers 
double ** compute_d(double **dat, int p, int *Z){
	double s;
	int num;
	double **d = make2Darray(K, p);
	for(int k=0; k < K; k++){
		for(int j=0; j < p; j++){
			s=0;
			num=0;
			for(int i=0; i < N; i++){
				if(Z[i] == k){
					s += dat[i][j];
					num += 1;
				}
			}	
			d[k][j] = s / num;
		}
	}
	return d;
}


int main(){
	MPI_Init(NULL, NULL);
	int world_rank, world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//create a new communicator
	int *rank;
	rank = (int*) malloc(sizeof(int)*(world_size-1));
	for(int i=0; i < world_size-1; i++){
		rank[i] = i;
	}

	int *Z_partial;

	MPI_Group world_group, new_group;
	MPI_Comm new_comm;

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_incl(world_group, world_size-1, rank, &new_group);
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

	//generate data on process 0
	double **dat, **dat_partial;
	int p=2;
	if(world_rank == 0){
		srand(123);
		dat = make2Darray(N, p); //rows are samples
		for(int i=0; i < N; i++){
			if(i < N/3){
				dat[i][0] = rnorm(1, 0.2);
				dat[i][1] = rnorm(1, 0.2);
			}else if(i < 2*(N/3)){
				dat[i][0] = rnorm(1, 0.2);
				dat[i][1] = rnorm(-1, 0.2);
			}else{
				dat[i][0] = rnorm(-1, 0.2);
				dat[i][1] = rnorm(-1, 0.2);					
			}
		}
	}

	
	//scatter data
	if(world_rank < world_size - 1){
		dat_partial = make2Darray(N/world_size, p);
	}else{
		dat_partial = make2Darray(N - (world_size-1)*(N/world_size), p);
	}	

	if(new_comm != MPI_COMM_NULL){
		MPI_Scatter(dat[0], p*(N/world_size), MPI_DOUBLE, dat_partial[0], p*(N/world_size), MPI_DOUBLE, 0, new_comm);
	}

	if(world_rank == 0){
		MPI_Send(dat[(world_size-1)*(N/world_size)], p*(N - (world_size-1)*(N/world_size)), MPI_DOUBLE, 
				world_size-1, 888, MPI_COMM_WORLD);
	}
	

	if(world_rank == world_size -1){
		MPI_Recv(dat_partial[0], p*(N - (world_size-1)*(N/world_size)), MPI_DOUBLE, 
				0, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}



	//initialize centers on process 0
	double **d;
	d = make2Darray(K, p);
	
	if(world_rank == 0){
		d[0][0] = 0;
		d[0][1] = 0;

		d[1][0] = 1;
		d[1][1] = -3;

		d[2][0] = -1;
		d[2][1] = -3;
		printf("==================Initialization=================\n");
		printf("cluster 1's center is (%f, %f)\n", d[0][0], d[0][1]);
		printf("cluster 2's center is (%f, %f)\n", d[1][0], d[1][1]);
		printf("cluster 3's center is (%f, %f)\n", d[2][0], d[2][1]);
	}



	MPI_Bcast(d[0], K*p, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	//Kmeans
	int t, *Z;

	if(world_rank == 0){
 		Z = (int*)malloc(N*sizeof(int));
	}

	double t1, t2;
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank == 0){
		t1 = MPI_Wtime();
	}
	
	for(t=0; t < 5; t++){
		//step 1: obtain the cluster indicators
		if(world_rank < world_size - 1){
			Z_partial = compute_Z(dat_partial, N/world_size, p, d);
		}else{
			Z_partial = compute_Z(dat_partial, N - (world_size-1)*(N/world_size), p, d);
		}
		MPI_Barrier(MPI_COMM_WORLD);		

		//gathering
		if(new_comm != MPI_COMM_NULL){
			MPI_Gather(Z_partial, N/world_size, MPI_INT, Z, N/world_size, MPI_INT, 0, new_comm);
		}	
		
		if(world_rank == world_size-1){
			MPI_Send(Z_partial, N - (world_size-1)*(N/world_size), MPI_INT, 0, 888, MPI_COMM_WORLD);
		}

		if(world_rank == 0){
			MPI_Recv(&(Z[(world_size-1)*(N/world_size)]), N - (world_size-1)*(N/world_size),
				 MPI_INT, world_size-1, 888, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		//step2: compute centers on processor 0
		if(world_rank == 0){
			d = compute_d(dat, p, Z);
			printf("================== Iteration %d =================\n", t);
			printf("cluster 1's center is (%f, %f)\n", d[0][0], d[0][1]);
			printf("cluster 2's center is (%f, %f)\n", d[1][0], d[1][1]);
			printf("cluster 3's center is (%f, %f)\n", d[2][0], d[2][1]);
		}
		MPI_Bcast(d[0], K*p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		
	}

	if(world_rank == 0){
		t2 = MPI_Wtime();
		printf("\nTime cost: %f seconds \n", t2 - t1);
	}


	MPI_Finalize();
}
	