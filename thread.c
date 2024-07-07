#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *count_to_100(void *arg)
{
	(void)arg;
	int i = 0;
	while(i < 100)
	{ 
		printf("The count has finished\n");
		i++;
	}
	usleep(10000);
	return (NULL);
}

void *print_hello(void *arg)
{
	int i = 0;
	(void)arg;
	while(i < 100)
	{
		printf("Hello, world!\n");
		i++;
	}
	usleep(10000);
	return (NULL);
}

int main()
{
	pthread_t thread_1;
	pthread_t thread_2;

	pthread_create(&thread_1, NULL, count_to_100, NULL);
	pthread_create(&thread_2, NULL, print_hello, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
}