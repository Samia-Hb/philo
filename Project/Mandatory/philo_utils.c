/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:19:07 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/18 04:52:03 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	reader(pthread_mutex_t *mutex, int *var)
{
	int	returned;

	pthread_mutex_lock(mutex);
	returned = *var;
	pthread_mutex_unlock(mutex);
	return (returned);
}

int	calculate_time(t_main *main)
{
	return (get_time_in_ms() - main->start_time);
}

int	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_usleep(int usec)
{
	int	start_time;
	int	result;

	start_time = get_time_in_ms();
	result = usec / 1000;
	while ((get_time_in_ms() - start_time) < result)
	{
		usleep(100);
	}
	return (0);
}

void	ft_clean(t_main *main)
{
	int	i;

	i = 0;
	while (i < main->philo_nbr)
	{
		pthread_mutex_destroy(&main->forks[i]);
		i++;
	}
	free(main->forks);
	free(main->philos);
	pthread_mutex_destroy(&main->dead_flag);
	pthread_mutex_destroy(&main->meal_flag);
	pthread_mutex_destroy(&main->message_lock);
	pthread_mutex_destroy(&main->tmp);
	free(main);
}
