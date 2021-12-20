#include <iostream>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank, size, prev, next;
	// array with 2 ints
	int buf[2];
	MPI_Init(&argc, &argv);
	// tell the process when operation completes
	MPI_Request reqs[4];
	// We have 4 stats
	MPI_Status stats[4];
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// Initialize prev as well as next
	prev = rank - 1;
	next = rank + 1;
	// if rank 0 -> prev is the last elem
	if (rank == 0) prev = size - 1;
	// of last elem -> next is 0 
	if (rank == size - 1) next = 0;
	// We begin listening for a message here but still go ahead (non-blocking receive)
	

	MPI_Sendrecv(&rank, 1, MPI_INT, next, 0, &buf[0], 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &stats[0]);
	MPI_Sendrecv(&rank, 1, MPI_INT, prev, 0, &buf[1], 1, MPI_INT, next, 0, MPI_COMM_WORLD, &stats[1]);

	

	//Your code here.
	//Here you need to display the number of the current process, and what it receives from the previous and next processes.

	printf("I am %d. I received %d from prev and %d from next.\n", rank, buf[0], buf[1]);
	MPI_Finalize();
}