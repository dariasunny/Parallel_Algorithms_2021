#include <iostream>
#include "mpi.h"
#define NTIMES 100
using namespace std;
int main(int argc, char** argv)
{
	double time_start, time_finish;
	int rank, i;
	int len;
	char* name = new char;
	//Begin parallel 
	MPI_Init(&argc, &argv);
	//Get process name
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Get processor name
	MPI_Get_processor_name(name, &len);
	//start time for this process
	time_start = MPI_Wtime();
	//wait for 100 cycles
	for (i = 0; i < NTIMES; i++)
		time_finish = MPI_Wtime();
	//print average time for each process
	cout << "processor " << name << ", process " << rank << "time = " << (time_finish - time_start) / NTIMES << endl;
	//End parallel
	MPI_Finalize();
}