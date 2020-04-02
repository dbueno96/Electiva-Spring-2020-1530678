#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[]){
	int size, rank, dest, source, count, tag=1;
	int msg=0;
	int i;
	MPI_Status Stat;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	if (rank!=0) {
		source= rank-1;
		MPI_Recv(&msg, 1, MPI_INT, source, tag,MPI_COMM_WORLD, &Stat);
		printf("Task %d receives %d from task %d\n", rank, msg, source);
	}

	dest= rank+1; 
	if (rank==size-1) {
		dest=0;
	}
	MPI_Send(&msg, 1, MPI_INT,dest, tag, MPI_COMM_WORLD); 
	
	if (rank==0) {
		source= size-1;
		MPI_Recv(&msg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
		printf("Task %d receives %d from task %d\n", rank, msg, source);

} 


	MPI_Finalize();
}

