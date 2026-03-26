/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:52:13 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 00:48:57 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	has_philosopher_died(t_philos *philo)
{
	sem_wait(philo->sema->time);
	if (get_time_in_ms() - philo->last_time_eat > philo->sema->time_to_die)
	{
		sem_post(philo->sema->time);
		return (1);
	}
	sem_post(philo->sema->time);
	return (0);
}

int	is_full(t_philos *philo)
{
	int	res;

	res = 0;
	sem_wait(philo->sema->meal_nbr);
	if (philo->sema->meals_nbr && philo->meals_nbr >= philo->sema->meals_nbr)
		res = 1;
	sem_post(philo->sema->meal_nbr);
	return (res);
}

void	*check_death(void *arg)
{
	t_philos	*philo;
	int			i;

	philo = arg;
	while (1)
	{
		if (has_philosopher_died(philo))
		{
			p_message(philo, 4);
			i = 0;
			while (i < philo->sema->philo_nbr)
			{
				sem_post(philo->sema->meal);
				i++;
			}
			return (NULL);
		}
		ft_usleep(500);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_sem	*sem;

	if (!parse(argc, argv))
		return (0);
	sem = malloc(sizeof(t_sem));
	if (!sem)
	{
		printf("Allocation Failed.\n");
		return (0);
	}
	if (!initiale_struct(argv, sem))
		return (0);
	philosopher_creation(sem);
	clean(sem);
	return (1);
}
