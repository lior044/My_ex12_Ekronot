#include "threads.h"


#define NUM_OF_THREADS 10

int main()
{

	//Part 3 - section C
	std::cout << NUM_OF_THREADS << " threads are running " << std::endl;
	callWritePrimesMultipleThreads(0, 1000, "primes2.txt", NUM_OF_THREADS);

	std::cout << "\nOnly one thread are running " << std::endl;
	callWritePrimesMultipleThreads(0, 1000, "primes2.txt", 1);

	system("pause");
	return 0;
}