#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 
#include "limits.h"
#include <math.h>
#include <time.h>

#define VECTOR_SIZE 20
#define PROCESSES 5

void myScatter(int *sbuff, int scnt, int*rbuff, int rcnt, int root, MPI_Comm comm){


	int myrank, size;
	MPI_Comm_rank(comm, &myrank); 
	MPI_Comm_size(comm, &size); 
	MPI_Status status;
	if(myrank==root){

		int i;
		for(i=0; i<PROCESSES; i++){
			if(i!=root){
				printf("Process %d sends %d value(s) to Process %d\n",root, scnt, i );
				MPI_Send(sbuff+ (i*scnt), scnt, MPI_INT, i, 0, comm );
			}
		}
		for(i=0;i<VECTOR_SIZE/PROCESSES;i++){
			rbuff[i]=sbuff[i];
		}
	}
	else{
		MPI_Recv(rbuff, rcnt, MPI_INT, root, 0, comm, &status);
		printf("Process %d receives %d from Process %d\n", myrank, *rbuff, root);
	}
	int i;
	for(i=0; i < VECTOR_SIZE/PROCESSES; i++){
		printf("My rank is %d and I have %d\n", myrank, rbuff[i]);
	}
}



int set_root(){
	srand(time(NULL));
	return (int) (rand()%PROCESSES); 
}

int main (){

	int vector [VECTOR_SIZE];
	int* rec_vector;
	int rank;
	int root= set_root();

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	rec_vector= (int*) malloc(sizeof(int)*VECTOR_SIZE/PROCESSES);
	if(rank==root)
	{
		int i;
		for(i=0; i<VECTOR_SIZE; i++){
			vector[i]=i;
		}
	}

	myScatter(vector, VECTOR_SIZE/PROCESSES, rec_vector, VECTOR_SIZE/PROCESSES, root, MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;

}

