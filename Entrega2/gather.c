#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 
#include "limits.h"
#include <math.h>
#include <time.h>

#define VECTOR_SIZE 20
#define PROCESSES 5

void myGather(int *sbuff, int scnt, int*rbuff, int rcnt, int root, MPI_Comm comm){
	int myrank, size;
	MPI_Comm_rank(comm, &myrank);
	MPI_Comm_size(comm, &size);
	MPI_Status status;

	if(myrank==root){
		int i;
		for(i=0; i<PROCESSES; i++){
			if(i !=root){
				MPI_Recv(rbuff+ i*rcnt, rcnt, MPI_INT, i, 0, comm,&status);
				printf("Process %d receives %d value(s) from Process %d\n", myrank, rcnt, i);
			}
		}
		for(i=0;i<VECTOR_SIZE/PROCESSES;i++){
			rbuff[i+ myrank*rcnt]=sbuff[i];
		}

		for (i=0; i<VECTOR_SIZE;i++){
			printf("gather_vector[%d]: %d\n",i , *rbuff);
			rbuff++;
		}

	}else{

		printf("Process %d sends %d values(s) to process %d\n",myrank, scnt, root);
		MPI_Send(sbuff, scnt, MPI_INT, root, 0, comm);
	}
}

int set_root(){
	srand(time(NULL));
	return (int) (rand()%PROCESSES); 
}

int main (){

	int gather_vector [VECTOR_SIZE];
	int* gather_piece;
	int rank;
	int root= set_root();
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	gather_piece=(int*)malloc(sizeof(int)*VECTOR_SIZE/PROCESSES);

	int i;
	for(i=0; i<VECTOR_SIZE/PROCESSES; i++){
		gather_piece[i]= i+ (rank* VECTOR_SIZE/PROCESSES);
	}

	myGather(gather_piece, VECTOR_SIZE/PROCESSES, gather_vector, VECTOR_SIZE/PROCESSES, root, MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;

}

