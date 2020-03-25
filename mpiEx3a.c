#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[]){
	int size, rank, dest, source, count, tag=1;
	int inmsg=11;
	int outmsg=1;
	MPI_Status Stat;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
	  dest = 1;
	  source = 1;
	  MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	  MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
          printf("Task %d received %d from task %d\n",rank, inmsg, dest);
	}else if (rank == 1) {
	  dest = 0;
	  source = 0;
	  MPI_Recv(&outmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
          printf("Task %d received %d from task %d\n",rank, outmsg, source);
	  MPI_Send(&inmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	 }

	MPI_Get_count(&Stat, MPI_INT, &count);
//	printf("Task %d: Received %d int(s) from task %d with tag %d \n",
//		   rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);

	MPI_Finalize();
}
