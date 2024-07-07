#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int state;
    int alive;
} philosopher_t;


typedef struct PhilosopherArgs {
  philosopher_t* philosophers;
  int num_philosophers;
} PhilosopherArgs;


typedef struct info
{
	int	*chopsticks;
	int	nbr_philo;
	pthread_mutex_t	mutex;
	philosopher_t	*philosophers;
}t_info;

void *philosopher_routine(t_info *info)
{
	int philosopher_id = info->(philosophores.id);
	while (1)
  	{
    	printf("Philosopher %d is thinking.\n", philosopher_id);
    	usleep(rand() % 3 + 1);
		pthread_mutex_lock(&info->mutex);
		while (!(info->chopsticks[philosopher_id] && info->chopsticks[(philosopher_id + 1) % info->nbr_philo]))
		{
			printf("Philosopher %d couldn't acquire chopsticks and is waiting.\n", philosopher_id);
			pthread_mutex_unlock(&info->mutex);
			sleep(rand() % 2 + 1);
			pthread_mutex_lock(&info->mutex);
			pthread_mutex_unlock(&info->mutex);
			sleep(rand() % 2 + 1);
			pthread_mutex_lock(&info->mutex);
		}
		info->chopsticks[philosopher_id] = 0;
		info->chopsticks[(philosopher_id + 1) % info->nbr_philo] = 0;
		printf("Philosopher %d acquired chopsticks and is eating.\n", philosopher_id);
		sleep(rand() % 2 + 1);
		pthread_mutex_unlock(&info->mutex);
		printf("Philosopher %d is sleeping.\n", philosopher_id);
		sleep(rand() % 3 + 1);
		if (!info->philosophers[philosopher_id].alive)
			break;
    }
    return(NULL);
}


int main(int ac, char **av)
{
	pthread_mutex_t mutex;
	t_info *general_info;
	int i = 0, j = 0;
    int philo_nbr = atoi(av[1]);
    if (philo_nbr <= 0)
	{
        printf("Number of philosophers must be a positive integer.\n");
        return (1);
    }
    philosopher_t *philosophers = malloc(philo_nbr * sizeof(philosopher_t));
    int *chopsticks = malloc(philo_nbr * sizeof(int));
    pthread_t philosopher_thread[philo_nbr];
    pthread_mutex_init(&mutex, NULL);

	i = 0;
    while (i < philo_nbr)
	{
        philosophers[i].id = i;
        philosophers[i].state = 0;
        philosophers[i].alive = 1;
        chopsticks[i] = 1;
		i++;
    }
	i = 0;
	while (i < philo_nbr)
	{
		general_info->chopsticks[i] = chopsticks[i];
		i++; 
	}
	general_info->nbr_philo = philo_nbr;
	general_info->mutex = mutex;
	i = 0;
	while (i < philo_nbr)
	{
		general_info->philosophers[i] = philosophers[i];
		i++;	
	}
    for (i = 0; i < philo_nbr; i++) {
        if (pthread_create(&philosopher_thread[i], NULL, philosopher_routine, (void *)&general_info) != 0)
		{ 
            printf("Error creating thread %d\n", i);
            free(philosophers);
            free(chopsticks);
            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }

    for (i = 0; i < philo_nbr; i++) {
        if (pthread_join(philosopher_thread[i], NULL) != 0) {
            printf("Error joining thread %d\n", i);
        }
    }

    pthread_mutex_destroy(&mutex);
    free(philosophers);
    free(chopsticks);
    return 0;
}

