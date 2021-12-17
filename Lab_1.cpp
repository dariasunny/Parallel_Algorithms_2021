#include <iostream>
#include "omp.h"
#include <string>
#include<vector>

int main(int argc, char* argv[]) { //get parameters
	int sizeOfArray = std::stoi(argv[1]); //str->int
	std::vector<int> array(sizeOfArray); //initialize array
	srand(time(NULL)); //initialize random number gererator

	for (int i = 0; i < sizeOfArray; i++) { //make random number array
		array[i] = rand();
	}

	for (int numThreads = 1; numThreads < 11; numThreads++) { //init number of threads
		std::vector<int> maximums(numThreads); //array maximums that will contain max elems found by threads

		int max;
		int n;

		double start = omp_get_wtime(); //start time measurement 
#pragma omp parallel private(max, n) num_threads(numThreads) //parallelization with its own max and n 
		{
			max = array[0];
			n = omp_get_thread_num(); //name of thread
#pragma omp for
			for (int i = 0; i < sizeOfArray; i++) { //found here max
				int newNum = array[i];
				max = newNum > max ? newNum : max;
			}
			maximums[n] = max; //save max for each n
		}
		double end = omp_get_wtime(); //end time measurement 
		printf("Num threads: %d, Time of computation: %f\n", numThreads, end - start);
		//printf("Num threads: %d\n", numThreads);
		max = array[0];
		for (int i = 0; i < numThreads; i++) { //found max among n
			int newNum = maximums[i];
			max = newNum > max ? newNum : max;
		}
		/*printf("Array contains:\n"); //print our array 
		for (int i = 0; i < sizeOfArray; i++) {
			printf("%d ", array[i]);
		}*/
		//printf("\n"); // print max
		//printf("Biggest number is %d\n", max);
	}
}
