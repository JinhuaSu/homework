#include<mpi.h>
#include<stdio.h>

int main(){
    MPI_Init(NULL,NULL);
    int reduce_value_1,reduce_value_2;
    int new_rank, world_rank;
    int ranks_1[4] = {2,3,4,5};
    int ranks_2[4] = {4,5,6,7};

    MPI_Group world_group , new_group_1,new_group_2;
    MPI_Comm new_comm_1,new_comm_2;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Group_incl(world_group, 4, ranks_1, &new_group_1);

    MPI_Group_incl(world_group, 4, ranks_2, &new_group_2);

    MPI_Comm_create(MPI_COMM_WORLD, new_group_1, &new_comm_1);

    MPI_Comm_create(MPI_COMM_WORLD, new_group_2, &new_comm_2);
    if(new_comm_1!=MPI_COMM_NULL){
        MPI_Reduce(&world_rank,&reduce_value_1,1,MPI_INT,MPI_SUM,3,new_comm_1);
    }
    if(new_comm_2!=MPI_COMM_NULL){
        MPI_Reduce(&world_rank,&reduce_value_2,1,MPI_INT,MPI_MAX,1,new_comm_2);
    }
    if(world_rank==5){
        MPI_Comm_rank(new_comm_1, &new_rank);
        printf("Group 1: world rank = %d, new rank = %d, reduce value = %d\n",world_rank, new_rank, reduce_value_1);
        MPI_Comm_rank(new_comm_2, &new_rank);
        printf("Group 2: world rank = %d, new rank = %d, reduce value = %d\n",world_rank, new_rank, reduce_value_2);
    }
    MPI_Finalize();
}