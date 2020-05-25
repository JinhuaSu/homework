//module load mpi/intelmpi-2015

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define B 100000
#define N 100
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

//sum function
double sum(double* array, int length){
	double s = 0;
	for(int i=0; i<length; i++){
		s += array[i];
	}
	return s;
}

//var function
double var(double* array, int length){
	double avg;
	avg = sum(array, length) / length;
	double s = 0;
	for(int i=0; i<length; i++){
		s += (array[i]-avg)*(array[i]-avg);
	}
	s = s/(length - 1);
	return s;
}


//uniformly sample from 0,1,...,n-2,n-1 
int sample(int n){
	double U = runif();
	int tmp;
	for(int i=0; i<n; i++){
		if((U > i/(double)n)&&(U < (i+1)/(double)n)){
			tmp = i;
			break;
		}
	}
	return tmp;
}


int main(){
	MPI_Init(NULL, NULL);
	int world_rank, world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	double *dat;
	dat = (double *)malloc(sizeof(double)*N);

	if(world_rank == 0){
		srand(123);
		for(int i=0; i<N; i++)
			dat[i] = rnorm(0,1);
	}

	MPI_Bcast(dat, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	int B0 = B / world_size;

	//Bootstrap
	int t;
	double s=0;
	double* re_dat = (double *)malloc(sizeof(double)*N);
	int index;

	double t1, t2;
	MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank == 0){
		t1 = MPI_Wtime();
	}

	for(t=0; t<B0; t++){
		for(int i=0; i<N; i++){
			index = sample(N);
			re_dat[i] = dat[index];
		}		
		s += var(re_dat, N);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank == 0){
		t2 = MPI_Wtime();
	}

	double avg;
	MPI_Reduce(&s, &avg, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(world_rank == 0){	
		avg = avg / B;
		printf("The bootstrap estimate of the mean of the sample variance is %f\n", avg);
		printf("Time cost %f seconds\n", t2 - t1);
	}

	MPI_Finalize();
}



