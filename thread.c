#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *count_to_100(void *arg)
{
	(void)arg;
	int i = 0;
	pthread_t tid = pthread_self();
	printf("thread_id = %lu\n", tid);
	while (i < 100)
	{ 
		printf("The count has finished\n");
		i++;
	}
	usleep(10000);
	return (NULL);
}

typedef struct philo
{
	int must_eat;
	int last_time_meal;
	
}t_phil ;

int main()
{
	pthread_t thread_1;
	pthread_t thread_2;

	pthread_create(&thread_1, NULL, count_to_100, NULL);
	pthread_create(&thread_2, NULL, count_to_100, NULL);
	pthread_join(thread_2, NULL);
	// pthread_join(thread_2, NULL);
}