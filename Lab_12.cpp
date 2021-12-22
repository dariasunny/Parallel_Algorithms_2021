#include <iostream>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank, size, prev, next;
	// array with 2 ints
	float rbuf[2];
	float sbuf[2];
	MPI_Init(&argc, &argv);
	// tell the process when operation completes
	MPI_Request reqs[4];
	// We have 4 stats
	MPI_Status stats[4];
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	prev = rank - 1;
	next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	// Here we are ready to sent
	MPI_Recv_init(&rbuf[0], 1, MPI_FLOAT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Recv_init(&rbuf[1], 1, MPI_FLOAT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	MPI_Send_init(&sbuf[0], 1, MPI_FLOAT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Send_init(&sbuf[1], 1, MPI_FLOAT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	for (int i = 0; i < size; i++) {
		sbuf[0] = (float)rank;
		sbuf[1] = (float)rank;
		// Sent messaages
		MPI_Startall(4, reqs);
		// Wait for operations to complete
		MPI_Waitall(4, reqs, stats);
		//your code
	}
	MPI_Request_free(&reqs[0]);
	MPI_Request_free(&reqs[1]);
	MPI_Request_free(&reqs[2]);
	MPI_Request_free(&reqs[3]);

	printf("I am %d. I received %f from prev and %f from next.\n", rank, rbuf[0], rbuf[1]);
	MPI_Finalize();
}
