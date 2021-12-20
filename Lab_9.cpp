#include <iostream>
#include <mpi.h>
using namespace std;

// Function of the casscade magic 
void cascadePowerOfTwo(int numberOfProcesses, int rank, int vectorSize, int * resultVector, int * inputVector, MPI_Status * status) {
	for (int i = 2; i <= numberOfProcesses; i *= 2) {
		if (rank % i == 0) {
			MPI_Recv(inputVector, vectorSize, MPI_INT, rank + i / 2, 0, MPI_COMM_WORLD, status);
			for (int i = 0; i < vectorSize; i++) {
				resultVector[i] += inputVector[i];
			}
		} if (rank % i == i / 2) {
			MPI_Send(resultVector, vectorSize, MPI_INT, rank - i / 2, 0, MPI_COMM_WORLD);
		}
	}
}

// Used in cascade part
int countSetBits(int n)
{
	int count = 0;
	while (n) {
		count += n & 1;
		n >>= 1;
	}
	return count;
}

int main(int argc, char** argv)
{
	
	int rank, numProcesses;
	MPI_Status status;
	// Begin parallel
	MPI_Init(&argc, &argv);
	// Store number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	// Store process name
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double time_start, time_end;
	const int vectorSize = 1000 * 1000;
	// Allocate memory for the message
	int* inputVector = (int*)malloc(vectorSize * sizeof(int));
	
	// for each process we have a vector that contains 1
	for (int i = 0; i < vectorSize; i++) {
		inputVector[i] = 1;
	}
	
	//MPI Reduce
	// result vector is our final
	int* resultVector = (int*)malloc(vectorSize * sizeof(int));
	time_start = MPI_Wtime();
	// MPI_Reduce sums inputVector from all processes and store the result in resultVector of process 0
	MPI_Reduce(inputVector, resultVector, vectorSize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	time_end = MPI_Wtime();
	// Print our calculations and time
	if (rank == 0) {
		printf("MPI_Reduce took %f\n", time_end - time_start);
		printf("First five elements: %d %d %d %d %d\n", resultVector[0], resultVector[1], resultVector[2], resultVector[3], resultVector[4]);
		fflush(stdout);
	}
	// MPI_Barrier to syncronize processes 
	MPI_Barrier(MPI_COMM_WORLD);
	// Cascade part
	// Make resultVector 1 for case of 1 process
	for (int i = 0; i < vectorSize; i++) {
		resultVector[i] = 1;
	}
	// Start time
	time_start = MPI_Wtime();
	// Count number of 1s in binary num processes (this is how many timees we ran cascade)
	int numCascades = countSetBits(numProcesses);
	
	
	int offset = 0;
	// Here we calculate largest power of 2 (the value) and run cascade on it that works for any positive number
	while (numProcesses >= 1) {
		int biggestPowerOfTwo = 1;
		while (biggestPowerOfTwo * 2 <= numProcesses) {
			biggestPowerOfTwo *= 2;
		}
		if (rank >= offset && rank -offset < biggestPowerOfTwo) {
			cascadePowerOfTwo(numProcesses, rank, vectorSize, resultVector, inputVector, &status);
		}
		if (rank == offset && rank != 0) {
			MPI_Send(resultVector, vectorSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		// Save the firts process in the new cascade 
		offset += biggestPowerOfTwo;
		// Repeat cycle 
		numProcesses -= biggestPowerOfTwo;
	}
	time_end = MPI_Wtime();
	if (rank == 0) {
		// Reseive results from other cascades
		for (int i = 1; i < numCascades; i++) {
			MPI_Recv(inputVector, vectorSize, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			for (int i = 0; i < vectorSize; i++) {
				// Sum them in our resultVector
				resultVector[i] += inputVector[i];
			}
		}
		printf("Cascade took %f\n", time_end - time_start);
		printf("First five elements: %d %d %d %d %d\n", resultVector[0], resultVector[1], resultVector[2], resultVector[3], resultVector[4]);
	}
	
	
	// End parallize 
	MPI_Finalize();
	return 0;
}
