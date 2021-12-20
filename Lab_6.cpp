#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char** argv)
{
	int rank, size, ibuf;
	MPI_Status status;
	float rbuf;
	// Begin parallel
	MPI_Init(&argc, &argv);
	// Store number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Store process name
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	ibuf = rank;
	rbuf = 1.0 * rank;
	// Process 1 sends ibuf to the process 0 with tag 5 
	if (rank == 1) MPI_Send(&ibuf, 1, MPI_INT, 0, 80, MPI_COMM_WORLD);
	// Process 2 sends rbuf to the process 0 with tag 5 
	if (rank == 2) MPI_Send(&rbuf, 1, MPI_FLOAT, 0, 68, MPI_COMM_WORLD);
	// 
	if (rank == 0) {
		// Process 0 get status of message with tag 5 
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		// If the message from process 1
		if (status.MPI_TAG == 80) {
			// Read message from 1, then from 2
			MPI_Recv(&ibuf, 1, MPI_INT, 1, 80, MPI_COMM_WORLD, &status);
			MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 68, MPI_COMM_WORLD, &status);
			cout << "Process 0 recv " << ibuf << " from process 1, " << rbuf << "from process 2\n";
		}
		//If the message from process 2
		else if (status.MPI_TAG == 68) {
			// Read message from 2, then from 1
			MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 68, MPI_COMM_WORLD, &status);
			MPI_Recv(&ibuf, 1, MPI_INT, 1, 80, MPI_COMM_WORLD, &status);
			cout << "Process 0 recv " << rbuf << " from process 2, " << ibuf << "from process 1\n";
		}
	}
	// End parallize 
	MPI_Finalize();
}