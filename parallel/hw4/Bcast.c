#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

int main(int argc, char** argv){
    if(argc != 3){
        fprintf(stderr, "Usage: compare_bcast number_elements number_trials\n");

        exit(1);
    }

    int number_elements = atoi(argv[1]);
    int number_trials = atoi(argv[2]);
}