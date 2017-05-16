//Implement a program that creates two threads.
//The threads will print their ID (pthread_self) 10 times and then stop.
//Insure that the printed IDs alternate always (ie A, B, A, B, ...)

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t m1, m2;

//The function will print the "A" part
void* a(void* x)
{
	//print 10 times
	for (int i = 0; i < 10; i++)
	{
		pthread_mutex_lock(&m2);
		printf("A: %u      (%d)\n",pthread_self(), i+1);
		pthread_mutex_unlock(&m1);
	}
	return NULL;
}


//The function will print the "B" part
void* b(void* x)
{
	//print 10 times
	for (int i = 0; i < 10; i++)
	{
		pthread_mutex_lock(&m1);
		printf("B: %u      (%d)\n",pthread_self(), i+1);
		pthread_mutex_unlock(&m2);
	}
	return NULL;
}

int main()
{
	//Declare the threads
	pthread_t t1, t2;

	//Initialise
	pthread_mutex_init(&m1, NULL);
	pthread_mutex_init(&m2, NULL);

	//Lock the part B
	pthread_mutex_lock(&m1);

	//Check if there was any errors in creating the thread
	if (pthread_create(&t1, NULL, a, NULL) < 0)
	{
		perror("Can't create the thread !");
		exit(1);
	}

	//Check if there was any errors in creating the thread
	if (pthread_create(&t2, NULL, b, NULL) < 0)
	{
		perror("Can't create the thread !");
		exit(2);
	}

	//Add the threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}
