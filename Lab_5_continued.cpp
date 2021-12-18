#include <stdio.h>
#include <iostream>
#include "mpi.h"
#include <vector>
using namespace std;
int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double time_start = MPI_Wtime();
	if (rank == 0)
	{
		std::vector<int> array(n, -1);
		cout << "Hello from process " << rank << "\n";
		for (i = 1; i < n; i++) {
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE,
				MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Hello from process " << message << endl;
			array[i] = message;
		}

		for (i = 0; i < n - 1; i++) {
			if (array[i] % 2 == 0 && array[i + 1] % 2 == 0) {
				printf("Received two consecutive even messages: %d, %d\n", array[i], array[i + 1]);
			}
		}
	}
	else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	double time_finish = MPI_Wtime();
	cout << "process " << rank << " finished in time = " << (time_finish - time_start) << endl;
	MPI_Finalize();
	return 0;
}