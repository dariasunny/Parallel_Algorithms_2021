#include <stdio.h>
#include <string.h>
#include "mpi.h"
int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Comm intercomm;
	// Receive port_name from command line
	strcpy_s(port_name, argv[1]);
	printf("Attempt to connect\n");
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	printf("Server connection\n");
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	MPI_Send(&rank, 1, MPI_INT, 0, 0, intercomm);
	printf("Client sent message %d\n", rank);
	int r;
	MPI_Status status;
	MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
	printf("Client got value: %d\n", r);
	MPI_Finalize();
	return 0;
}