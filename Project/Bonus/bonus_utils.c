/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:52:17 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/05 10:46:22 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	current_time(t_sem *main, int i)
{
	return (get_time_in_ms() - main->philosophers[i]->start_time);
}

void	kill_all_process(t_philos **philos)
{
	int	i;
	int	philo_nbr;

	i = 0;
	philo_nbr = philos[i]->sema->philo_nbr;
	while (i < philo_nbr)
	{
		kill(philos[i]->pid, SIGKILL);
		i++;
	}
}

void	p_message(t_philos *philo, int msg_code)
{
	int	time;

	sem_wait(philo->sema->mess);
	time = current_time(philo->sema, philo->id - 1);
	if (msg_code == 1 && !is_full(philo))
		printf("%d  %d is eating\n", time, philo->id);
	else if (msg_code == 2 && !is_full(philo))
		printf("%d  %d is sleeping\n", time, philo->id);
	else if (msg_code == 3 && !is_full(philo))
		printf("%d  %d is thinking\n", time, philo->id);
	else if (msg_code == 4)
	{
		printf("%d  %d is dead\n", time, philo->id);
		return ;
	}
	else if (msg_code == 6 && !is_full(philo))
		printf("%d  %d has taken a fork \n", time, philo->id);
	sem_post(philo->sema->mess);
}

int	initiale_philos(t_sem *sema)
{
	int	i;

	i = 0;
	sema->philosophers = malloc(sema->philo_nbr * sizeof(t_philos *));
	if (!sema->philosophers)
	{
		printf("Allocation Failed\n");
		return (0);
	}
	while (i < sema->philo_nbr)
	{
		sema->philosophers[i] = malloc(sizeof(t_philos));
		if (!sema->philosophers[i])
		{
			printf("Allocation Failed\n");
			return (0);
		}
		sema->philosophers[i]->id = i + 1;
		sema->philosophers[i]->start_time = get_time_in_ms();
		sema->philosophers[i]->meals_nbr = 0;
		sema->philosophers[i]->pid = -1;
		sema->philosophers[i]->sema = sema;
		i++;
	}
	return (1);
}

int	initiale_struct(char **argv, t_sem *sema)
{
	semaphore_initialisation(sema, argv);
	sema->philo_nbr = ft_atoi(argv[1]);
	sema->time_to_die = ft_atoi(argv[2]);
	sema->time_to_eat = ft_atoi(argv[3]);
	sema->time_to_sleep = ft_atoi(argv[4]);
	sema->dead_flag = 0;
	sema->meals_nbr = 0;
	if (argv[5])
		sema->meals_nbr = ft_atoi(argv[5]);
	if (!initiale_philos(sema))
		return (0);
	return (1);
}
