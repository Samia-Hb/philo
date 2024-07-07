#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// last_meal = gettime - i;
typedef struct {
    int id;
    int state;
    int alive;
    int last_meal;
} philosopher_t;

typedef struct info
{
    int *chopsticks;
    int nbr_philo;
    int time_die;
    int time_eat;
    int time_sleep;
    pthread_mutex_t mutex;
    philosopher_t *philosophers;
} t_info;

// void *philosopher_routine(void *arg)
// {
//     t_info *info = (t_info *)arg;
//     int philosopher_id = info->philosophers->id;

//     while (1)
//     {
//         printf("Philosopher %d is sleeping.\n", philosopher_id);
//         usleep(info->time_sleep);
//         pthread_mutex_lock(&info->mutex);
//         while (!(info->chopsticks[philosopher_id] && info->chopsticks[(philosopher_id + 1)]))
//         {
//             printf("Philosopher %d couldn't acquire chopsticks and is waiting.\n", philosopher_id);
//             pthread_mutex_unlock(&info->mutex);
//             pthread_mutex_lock(&info->mutex);
//         }
//         info->chopsticks[philosopher_id] = 0;
//         info->chopsticks[philosopher_id + 1] = 0;
//         printf("Philosopher %d acquired chopsticks and is eating.\n", philosopher_id);
// 		usleep(info->time_eat);
//         pthread_mutex_unlock(&info->mutex);
//         printf("Philosopher %d is sleeping.\n", philosopher_id);
//         usleep(info->time_sleep);
//         if (!info->philosophers[philosopher_id].alive)
//             break;
//     }
//     return NULL;
// }

void *philosopher_routine(void *arg)
{
    t_info *info = (t_info *)arg;
    int philosopher_id = info->philosophers->id;

    while (1)
    {
		//sleeping
        printf("Philosopher %d is sleeping.\n", philosopher_id);
        usleep(info->time_sleep);
		//eating
		if(info->chopsticks[philosopher_id] && info->chopsticks[philosopher_id + 1])
        pthread_mutex_lock(&info->mutex);
        printf("Philosopher %d is eating.\n", philosopher_id);
		usleep(info->time_eat);
		pthread_mutex_unlock(&info->mutex);
        while (!(info->chopsticks[philosopher_id] && info->chopsticks[(philosopher_id + 1)]) && info->time_die)
        {
			//thinking
	        printf("Philosopher %d is thinking.\n", philosopher_id);
			(info->time_die)--;
			if (!info->time_die)
			{
	        	info->philosophers[philosopher_id].alive = 0;
				break;
			}
		}
		if (info->philosophers[philosopher_id].alive == 0)
		{
			printf("Philosopher %d is dead.\n", philosopher_id);
			break;
		}
    }
    return NULL;
}

int main(int ac, char **av)
{
    int i;
	int *chopsticks;
	int philo_nbr;
	philosopher_t *philosophers;
    pthread_t philosopher_thread[philo_nbr];
	t_info *general_info;
    if (ac != 5)
    {
        printf("Usage: %s <number_of_philosophers>\n", av[0]);
        return 1;
    }
	i = 0;
    philo_nbr = atoi(av[1]);
    if (philo_nbr <= 0)
    {
        printf("Number of philosophers must be a positive integer.\n");
        return (1);
    }
    philosophers = (philosopher_t *)malloc(philo_nbr * sizeof(philosopher_t));
    chopsticks = (int *)malloc(philo_nbr * sizeof(int));
    general_info = (t_info *)malloc(sizeof(t_info));
    general_info->time_die = atoi(av[2]);
    general_info->time_eat = atoi(av[3]);
    general_info->time_sleep = atoi(av[4]);
    pthread_mutex_init(&general_info->mutex, NULL);
    general_info->nbr_philo = philo_nbr;
    general_info->philosophers = philosophers;
    general_info->chopsticks = chopsticks;
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
        if (pthread_create(&philosopher_thread[i], NULL, philosopher_routine, (void *)general_info) != 0)
        {
            printf("Error creating thread %d\n", i);
            free(philosophers);
            free(chopsticks);
            free(general_info);
            return (1);
        }
        i++;
    }
	i = 0;
    while (i < philo_nbr)
    {
        if (pthread_join(philosopher_thread[i], NULL) != 0)
		{
            printf("Error joining thread %d\n", i);
			return (0);
		}
		i++;
    }
    pthread_mutex_destroy(&general_info->mutex);
    free(philosophers);
    free(chopsticks);
    free(general_info);
    return 0;
}
