#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h" 
#include "limits.h"
#include <math.h>
#include <time.h>

#define PROCESSES 5
void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if (world_rank == root) {
    // If we are the root process, send our data to everyone
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {
    // If we are a receiver process, receive the data from the root
    MPI_Recv(data, count, datatype, root, 0, communicator,
             MPI_STATUS_IGNORE);
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

	my_bcast(&msg, 1,MPI_INT, root, MPI_COMM_WORLD );

	MPI_Finalize();
	return 0;

}

