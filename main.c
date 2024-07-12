/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:27:04 by shebaz            #+#    #+#             */
/*   Updated: 2024/07/12 09:53:43 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philosopher *philosopher, int time)
{
	printf("Philosopher %d is eating\n", philosopher->id);
	usleep(time);
}

void	ft_sleep(t_philosopher *philosopher, int time)
{
	printf("Philosopher %d is sleeping\n", philosopher->id + 1);
	usleep(time);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_info			*info;

	philosopher = (t_philosopher *)arg;
	info = philosopher->info;
	while (!info->stop_simulation)
	{
		printf("Philosopher %d is thinking\n", philosopher->id + 1);
		if (philosopher->id % 2 == 0)
		{
			pthread_mutex_lock(philosopher->right_fork);
			printf("Philosopher %d has taken the right fork\n", philosopher->id
				+ 1);
			pthread_mutex_lock(philosopher->left_fork);
			printf("Philosopher %d has taken the left fork\n", philosopher->id
				+ 1);
		}
		else
		{
			pthread_mutex_lock(philosopher->left_fork);
			printf("Philosopher %d has taken the left fork\n", philosopher->id
				+ 1);
			pthread_mutex_lock(philosopher->right_fork);
			printf("Philosopher %d has taken the right fork\n", philosopher->id
				+ 1);
		}
		eat(philosopher, info->time_eat);
		pthread_mutex_lock(&info->death_mutex);
		info->time_since_last_meal[philosopher->id] = 0;
		pthread_mutex_unlock(&info->death_mutex);
		pthread_mutex_unlock(philosopher->left_fork);
		printf("Philosopher %d has put down the left fork\n", philosopher->id
			+ 1);
		pthread_mutex_unlock(philosopher->right_fork);
		printf("Philosopher %d has put down the right fork\n", philosopher->id
			+ 1);
		ft_sleep(philosopher, info->time_sleep);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_info	*info;
	int		i;

	info = (t_info *)arg;
	while (!info->stop_simulation)
	{
		i = 0;
		while (i < info->num_philosophers)
		{
			pthread_mutex_lock(&info->death_mutex);
			info->time_since_last_meal[i] += 1000;
			if (info->time_since_last_meal[i] >= info->time_die)
			{
				printf("Philosopher %d has died\n", i + 1);
				info->stop_simulation = 1;
				pthread_mutex_unlock(&info->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&info->death_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	initale_variable(char **argv, t_info *info)
{
	int	i;

	i = 0;
	info->num_philosophers = atoi(argv[1]);
	info->time_eat = atoi(argv[2]) * 1000;
	info->time_sleep = atoi(argv[3]) * 1000;
	info->time_die = atoi(argv[4]) * 1000;
	info->stop_simulation = 0;
	info->philosophers = (t_philosopher *)malloc(info->num_philosophers
			* sizeof(t_philosopher));
	info->forks = (pthread_mutex_t *)malloc(info->num_philosophers
			* sizeof(pthread_mutex_t));
	info->time_since_last_meal = (int *)malloc(info->num_philosophers
			* sizeof(int));
	while (i < info->num_philosophers)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		info->philosophers[i].id = i;
		info->philosophers[i].left_fork = &info->forks[i];
		info->philosophers[i].right_fork = &info->forks[(i + 1)
			% info->num_philosophers];
		info->philosophers[i].info = info;
		info->time_since_last_meal[i] = 0;
		i++;
	}
	pthread_mutex_init(&info->death_mutex, NULL);
}

void	clean_up(t_info *info)
{
	int	i;

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
}

int	main(int argc, char **argv)
{
	t_info		*info;
	pthread_t	*threads;
	pthread_t	monitor_thread;
	int			i;

	if (argc != 5)
	{
		printf("Required: %s num_philosophers time_eat time_sleep time_die\n", argv[0]);
		return (1);
    }
	info = (t_info *)malloc(sizeof(t_info));
	initale_variable(argv, info);
	threads = (pthread_t *)malloc(info->num_philosophers * sizeof(pthread_t));
	i = 0;
	while (i < info->num_philosophers)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine,
				&info->philosophers[i]))
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, info))
	{
		perror("Failed to create monitor thread");
		return (1);
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
	pthread_join(monitor_thread, NULL);
	clean_up(info);
	free(info);
	free(threads);
	return (0);
}