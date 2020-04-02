#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 
#include "limits.h"
#include <math.h>
#include <time.h>

#define VECTOR_SIZE 20
#define PROCESSES 5




void  myBCast(int* buff, int count, int root, MPI_Comm comm ){
	int receive;
	int comm_size;
	int myrank;
	MPI_Comm_rank(comm, &myrank);
	MPI_Comm_size(comm, &comm_size);
	MPI_Status status;
	int i;
	int tag=0;
	if (myrank==root){
		for(i=0; i < comm_size; i++){
			if(i!=root){
				printf("Process %d sends %d to process %d\n", root,*buff, i);
				MPI_Send(buff, count, MPI_INT, i, tag, comm);
			}
		}
		receive= *buff;
		printf("Process %d 'receives' %d from himself\n",myrank, receive);
	}
	else{
		MPI_Recv(buff, count, MPI_INT, root, tag, comm, &status);
		printf("Process %d receives %d from process %d\n",myrank , *buff,root );
	}
}



int set_root(){
	srand(time(NULL));
	return (int) (rand()%PROCESSES); 
}

int main (){

	int msg;
	int rank;
	int root= set_root();

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank==root){msg=1000;}

	myBCast(&msg, 1, root, MPI_COMM_WORLD );

	MPI_Finalize();
	return 0;

}

