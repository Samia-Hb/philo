/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:27:04 by shebaz            #+#    #+#             */
/*   Updated: 2024/07/17 11:18:28 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_info t_info;

typedef struct s_philosopher
{
	int             id;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_info          *info;
	int				state[3];
} t_philosopher;

typedef struct s_info
{
	int             num_philosophers;
	int             time_eat;
	int             time_sleep;
	int             time_die;
	t_philosopher   *philosophers;
	pthread_mutex_t *forks;
	pthread_mutex_t death_mutex;
	int             *time_since_last_meal;
	int             stop_simulation;
} t_info;

void eat(t_philosopher *philosopher, int time)
{
	printf("Philosopher %d is eating\n", philosopher->id + 1);
	usleep(time);
}

void ft_sleep(t_philosopher *philosopher, t_info *info)
{
	printf("Philosopher %d is sleeping\n", philosopher->id + 1);
	info->time_since_last_meal[philosopher->id] = 0;
	usleep(info->time_sleep);
}

void *philosopher_routine(void *arg)
{
	t_philosopher *philosopher = (t_philosopher *)arg;
	t_info *info = philosopher->info;

	while (1)
	{
		printf("Philosopher %d is thinking\n", philosopher->id + 1);
		info->time_since_last_meal[philosopher->id]++;
		pthread_mutex_lock(&info->death_mutex);
		if (info->time_since_last_meal[philosopher->id] > info->time_die)
		{
			printf("Philosopher %d is dead\n", philosopher->id + 1);
			pthread_mutex_unlock(&info->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&info->death_mutex);
		if (philosopher->id == info->num_philosophers - 1)
		{
			pthread_mutex_lock(philosopher->right_fork);
			printf("Philosopher %d has taken the right fork\n", philosopher->id + 1);
			pthread_mutex_lock(philosopher->left_fork);
			printf("Philosopher %d has taken the left fork\n", philosopher->id + 1);
		}
		else
		{
			pthread_mutex_lock(philosopher->left_fork);
			printf("Philosopher %d has taken the left fork\n", philosopher->id + 1);
			pthread_mutex_lock(philosopher->right_fork);
			printf("Philosopher %d has taken the right fork\n", philosopher->id + 1);
		}
		eat(philosopher, info->time_eat);
		info->time_since_last_meal[philosopher->id] = 0;
		pthread_mutex_unlock(philosopher->left_fork);
		printf("Philosopher %d has put down the left fork\n", philosopher->id + 1);
		pthread_mutex_unlock(philosopher->right_fork);
		printf("Philosopher %d has put down the right fork\n", philosopher->id + 1);
		ft_sleep(philosopher, info);
	}
	return (NULL);
}

void initale_variable(char **argv, t_info *info)
{
	int i;

	info->num_philosophers = atoi(argv[1]);
	info->time_eat = atoi(argv[2]) * 1000;
	info->time_sleep = atoi(argv[3]) * 1000;
	info->time_die = atoi(argv[4]) * 1000;
	info->stop_simulation = 0;
	info->philosophers = (t_philosopher *)malloc(info->num_philosophers * sizeof(t_philosopher));
	info->forks = (pthread_mutex_t *)malloc(info->num_philosophers * sizeof(pthread_mutex_t));
	info->time_since_last_meal = (int *)malloc(info->num_philosophers * sizeof(int));
	i = 0;
	while (i < info->num_philosophers)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		info->philosophers[i].id = i;
		info->philosophers[i].left_fork = &info->forks[i];
		info->philosophers[i].right_fork = &info->forks[(i + 1) % info->num_philosophers];
		info->philosophers[i].info = info;
		info->time_since_last_meal[i] = 0;
		i++;
	}
	pthread_mutex_init(&info->death_mutex, NULL);
}

void clean_up(t_info *info, pthread_t *threads)
{
	int i;

	i = 0;
	while (i < info->num_philosophers)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&info->death_mutex);
	free(info->philosophers);
	free(info->forks);
	free(info->time_since_last_meal);
	free(info);
	free(threads);
}

int main(int argc, char **argv)
{
	t_info		*info;
	pthread_t	*threads;
	int			i;

	if (argc != 5)
	{
		printf("Required: %s num_philosophers time_eat time_sleep time_die\n", argv[0]);
		return (1);
	}
	if (!strcmp(argv[1],"1"))//one philo case
	{
		printf("There is only one philosopher\n");
		return (1);
	}
	info = (t_info *)malloc(sizeof(t_info));
	initale_variable(argv, info);
	threads = (pthread_t *)malloc(info->num_philosophers * sizeof(pthread_t));
	i = 0;
	while (i < info->num_philosophers)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine, &info->philosophers[i]))
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < info->num_philosophers)
	{
		if (pthread_join(threads[i], NULL))
		{
			perror("Failed to join thread");
			return (1);
		}
		i++;
	}
	clean_up(info, threads);
	return (0);
}
