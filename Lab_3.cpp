#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char* argv[]) {
	//Begin parallel
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	// n stores number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	// rank stores process name
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		cout << "Hello from process " << rank <<"\n";
		for (i = 1; i < n; i++) {
			// Receive 1 integer from any sourse saved message 
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			// print our message
			cout << "Hello from process " << message << endl;
		}
	}
	// if rank isnt 0 => Send process 0 one int containing the address of rank
	else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	//MPI finalise
	MPI_Finalize();
	return 0;
}