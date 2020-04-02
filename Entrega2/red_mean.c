#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define VSIZE 20
#define PROCESSES 5


void init_vector(int*rank, int *root, float *vector){
	if(*rank==*root){
		int i;
		for(i=0;i<VSIZE; i++){
			vector[i]= (float) i;
			printf("v[%d]: %.2f\n",i,vector[i]);
		}
		printf("\n");
	}
	vector=&vector[0];
}
void print_v(float* v, int size){
	int i;
	for(i=0;i<size;i++){
		printf("%.2f\n", v[i]);
	}
}

float mpi_mean(int size, float*rec_vector){
	float sum=0;
	int i;
	for(i=0; i< size; i++){
		sum= sum+ rec_vector[i];
	}

	return (float)(sum / size);
}

void myOpFun(float *in, float *inout, int *len, MPI_Datatype *dptr) {
	printf("%f\n",*in);
	printf("%f\n",*inout);
	printf("%d\n",*len);
	printf("%p\n",*dptr);

}

int main(){
	int rank,size;
	float result;
	float vector[VSIZE];
	float means_vect[PROCESSES];
	float* rec_vector;
	srand(time(NULL));
	int root=rand()%PROCESSES;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	init_vector(&rank, &root, vector);

	rec_vector= (float*) malloc(sizeof(float)* VSIZE/PROCESSES);

	MPI_Scatter(vector, VSIZE/PROCESSES, MPI_FLOAT, rec_vector, VSIZE/PROCESSES, MPI_FLOAT, root, MPI_COMM_WORLD);

	float mean=mpi_mean(VSIZE/PROCESSES, rec_vector);


	printf("rank %d mean= %.2f\n",rank, mean);
	MPI_Op myOp;
	MPI_Op_create((MPI_User_function *)myOpFun, 1, &myOp);
	MPI_Reduce(&mean, &result, 1, MPI_FLOAT, MPI_SUM,root, MPI_COMM_WORLD);

	if(root==rank){
		result= result/PROCESSES;
		printf("Mean= %.2f\n",result);
	}


	MPI_Finalize();
}
