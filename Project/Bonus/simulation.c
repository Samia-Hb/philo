/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:22:30 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/16 13:12:09 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eat(t_philos *philo)
{
	sem_wait(philo->sema->semaphores);
	sem_wait(philo->sema->semaphores);
	p_message(philo, 6);
	p_message(philo, 6);
	sem_wait(philo->sema->time);
	philo->last_time_eat = get_time_in_ms();
	sem_post(philo->sema->time);
	p_message(philo, 1);
	sem_wait(philo->sema->meal_nbr);
	philo->meals_nbr++;
	if (philo->sema->meals_nbr && philo->meals_nbr >= philo->sema->meals_nbr)
		sem_post(philo->sema->meal);
	sem_post(philo->sema->meal_nbr);
	ft_usleep(philo->sema->time_to_eat * 1000);
	sem_post(philo->sema->semaphores);
	sem_post(philo->sema->semaphores);
}

void	simulation(t_philos *philo)
{
	if (philo->sema->philo_nbr == 1)
	{
		p_message(philo, 6);
		ft_usleep(philo->sema->time_to_die);
	}
	else
	{
		while (!has_philosopher_died(philo))
		{
			if (philo->sema->meals_nbr && is_full(philo))
				break ;
			eat(philo);
			ft_sleeep(philo);
			p_message(philo, 3);
			if (philo->sema->philo_nbr % 2 == 0)
				continue ;
			ft_usleep(1000);
		}
	}
}

void	*philosopher_creation(t_sem *sem)
{
	int	i;

	i = 0;
	while (i < sem->philo_nbr)
	{
		sem->philosophers[i]->pid = fork();
		if (sem->philosophers[i]->pid == 0)
		{
			sem->philosophers[i]->last_time_eat = get_time_in_ms();
			pthread_create(&sem->philosophers[i]->thread, NULL, check_death,
				sem->philosophers[i]);
			simulation(sem->philosophers[i]);
			pthread_join(sem->philosophers[i]->thread, NULL);
		}
		i++;
	}
	i = 0;
	while (i < sem->philo_nbr)
	{
		sem_wait(sem->meal);
		i++;
	}
	kill_all_process(sem->philosophers);
	return (NULL);
}
