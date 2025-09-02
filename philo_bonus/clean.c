/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:12:35 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 00:05:26 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	set_val(int *key, int value, sem_t *sem)
{
	sem_wait(sem);
	*key = value;
	sem_post(sem);
}

int	read_value(int *value, sem_t *sem)
{
	int	val;

	sem_wait(sem);
	val = *value;
	sem_post(sem);
	return (val);
}

void	ft_sleeep(t_philos *philo)
{
	p_message(philo, 2);
	ft_usleep(philo->sema->time_to_sleep * 1000);
}

void	clean(t_sem *sema)
{
	int	i;

	i = 0;
	sem_close(sema->semaphores);
	sem_unlink("/forks");
	sem_close(sema->mess);
	sem_unlink("/mess");
	sem_close(sema->meal);
	sem_unlink("/meal");
	sem_close(sema->time);
	sem_unlink("/time");
	sem_close(sema->meal_nbr);
	sem_unlink("/meals_nbr");
	while (i < sema->philo_nbr)
	{
		free(sema->philosophers[i]);
		i++;
	}
	free(sema->philosophers);
	free(sema);
}

void	semaphore_initialisation(t_sem *sema, char **argv)
{
	sem_unlink("/forks");
	sem_unlink("/mess");
	sem_unlink("/meal");
	sem_unlink("/meals_nbr");
	sem_unlink("/time");
	sema->semaphores = sem_open("/forks", O_CREAT, 0644, ft_atoi(argv[1]));
	sema->mess = sem_open("/mess", O_CREAT, 0644, 1);
	sema->time = sem_open("/time", O_CREAT, 0644, 1);
	sema->meal_nbr = sem_open("/meals_nbr", O_CREAT, 0644, 1);
	sema->meal = sem_open("/meal", O_CREAT, 0644, 0);
	if (sema->mess == SEM_FAILED || sema->time == SEM_FAILED
		|| sema->meal_nbr == SEM_FAILED || sema->semaphores == SEM_FAILED
		|| sema->meal == SEM_FAILED)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
}
