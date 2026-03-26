/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:17:34 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/16 13:33:10 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, 4);
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, 4);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, 4);
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, 4);
	}
}

void	unlock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	meals_lock(t_philo *philo)
{
	pthread_mutex_lock(&philo->main->dead_flag);
	philo->last_meal = get_time_in_ms();
	philo->meals_nbr++;
	if (is_fulll(philo))
	{
		pthread_mutex_lock(&philo->main->tmp);
		philo->main->flag++;
		pthread_mutex_unlock(&philo->main->tmp);
	}
	pthread_mutex_unlock(&philo->main->dead_flag);
}

void	ft_eat(t_philo *philo)
{
	lock_forks(philo);
	meals_lock(philo);
	print_message(philo, 5);
	ft_usleep(philo->main->time_eat * 1000);
	unlock_forks(philo);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->main->philo_nbr == 1)
	{
		print_message(philo, 4);
		ft_usleep(philo->main->time_eat + 10);
	}
	else
	{
		while (reader(&philo->main->dead_flag, &philo->main->dead_sign))
		{
			if (is_fulll(philo))
				break ;
			ft_eat(philo);
			ft_sleep(philo);
			print_message(philo, 6);
			if (philo->main->philo_nbr % 2 == 0)
				continue ;
			ft_usleep(1000);
		}
	}
	return (NULL);
}
