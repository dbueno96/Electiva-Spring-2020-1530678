#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 
#include "limits.h"
#include <math.h>
#include <time.h>

#define VECTOR_SIZE 20
#define PROCESSES 5

int myprint(int* rank, int * size){
	char hostname[HOST_NAME_MAX+1];
	gethostname(hostname, HOST_NAME_MAX+1);
	printf("Hello from %s\n", hostname);
}

void test(int * buffer, int size){
	int i;

	for(i=0; i < VECTOR_SIZE; i++){
		printf("buffer[%d]: %d\n",i, *buffer);
		buffer++;
	}
}


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
		for(i=1; i < comm_size; i++){
			printf("i=%d\nComm_size=%d\n",i,comm_size);
			printf("Process %d sends %d to process %d\n", root,*buff, i);
			MPI_Send(buff, count, MPI_INT, i, tag, comm);
		}
		receive= *buff;
		printf("Process 0 'receives' %d from himself\n", receive);
	}
	else{
		MPI_Recv(buff, count, MPI_INT, root, tag, comm, &status);
		printf("Process %d receives %d from process %d\n",myrank , *buff,root );
	}
}


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

	int msg;
	int vector [VECTOR_SIZE];
	int gather_vector [VECTOR_SIZE];
	int* rec_vector;
	int* gather_piece;
	int rank;
	int root= set_root();
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	rec_vector= (int*) malloc(sizeof(int)*VECTOR_SIZE/PROCESSES);
	gather_piece=(int*)malloc(sizeof(int)*VECTOR_SIZE/PROCESSES);
	if(rank==root)
	{
		int i;
		msg=1000;
		for(i=0; i<VECTOR_SIZE; i++){
			vector[i]=i;
		}
	}
	int i;
	for(i=0; i<VECTOR_SIZE/PROCESSES; i++){
		gather_piece[i]= i+ (rank* VECTOR_SIZE/PROCESSES);
	}

//	myprint(&rank,&size);
//	myBCast(&msg, 1, root, MPI_COMM_WORLD );
//	myScatter();
	myScatter(vector, VECTOR_SIZE/PROCESSES, rec_vector, VECTOR_SIZE/PROCESSES, root, MPI_COMM_WORLD);
//	myGather(gather_piece, VECTOR_SIZE/PROCESSES, gather_vector, VECTOR_SIZE/PROCESSES, root, MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;

}

