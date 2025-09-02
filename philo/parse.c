/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:12:46 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/28 06:47:37 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_fulll(t_philo *philo)
{
	if (philo->main->meals && philo->meals_nbr >= philo->main->meals)
	{
		return (1);
	}
	return (0);
}

void	set_value(pthread_mutex_t *mutex, int *var, int value)
{
	pthread_mutex_lock(mutex);
	*var = value;
	pthread_mutex_unlock(mutex);
}

int	parse(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("philo_nbr time_die time_eat time_sleep meals_nbr[optional]\n");
		return (0);
	}
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0 || ft_atoi(argv[i]) > INT_MAX
			|| ft_atoi(argv[i]) < INT_MIN)
		{
			printf("Error \n");
			return (0);
		}
		i++;
	}
	return (1);
}
