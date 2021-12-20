#include <iostream>
#include <mpi.h>
using namespace std;
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
	int received=0;
	int maxMessageLength = 1000*1000;
	// Allocate memory for the message
	char* message = (char*)(malloc(maxMessageLength * sizeof(char)));
	double highestBandwidth = 0;
	for (int messageLength = 1; messageLength <= maxMessageLength; messageLength *= 10) {
		// Average 
		int numIters = 10;
		if (rank == 0) {
			// Start time 
			double time_start = MPI_Wtime();
			for (int j = 0; j < numIters; j++) {
				// Sent message from message of lenght to 1
				MPI_Send(message, messageLength, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
				// Receive confirmation from 1
				MPI_Recv(&received, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
			}
			//End time
			double time_finish = MPI_Wtime();
			int bytesInMb = 1024 * 1024;
			printf("N: %d, L: %d, T: %f\n", numIters, messageLength, time_finish - time_start);
			// Calculate bandwidth by formula 
			double bandwidth = 2 * numIters * messageLength / ((time_finish - time_start) * bytesInMb);
			printf("Message length %d bandwidth %f MB/s\n", messageLength, bandwidth);
			highestBandwidth = bandwidth > highestBandwidth ? bandwidth : highestBandwidth;
		}
		else if (rank == 1) {
			for (int j = 0; j < numIters; j++) {
				//Receive message from 0
				MPI_Recv(message, messageLength, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
				// Sent confirmation to 0
				MPI_Send(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			}
		}

	}
	if (rank == 0) {
		printf("Highest bandwidth: %f\n", highestBandwidth);
	}
	int numIters = 1000*1000;
	if (rank == 0) {
		// Start time 
		double time_start = MPI_Wtime();
		for (int j = 0; j < numIters; j++) {
			// Sent message from message of lenght to 1
			MPI_Send(message, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			// Receive confirmation from 1
			MPI_Recv(&received, 0, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		}
		//End time
		double time_finish = MPI_Wtime();
		printf("N: %d, T: %f\n", numIters, time_finish - time_start);
		// Calculate latency by formula 
		printf("Latency: %.9f microseconds\n",  1000*1000*(time_finish - time_start)/(2*numIters));
		fflush(stdout);
	}
	else if (rank == 1) {
		for (int j = 0; j < numIters; j++) {
			//Receive message from 0
			MPI_Recv(message, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			// Sent confirmation to 0
			MPI_Send(&received, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}

	// End parallize 
	MPI_Finalize();
}