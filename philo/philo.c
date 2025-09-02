/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:17:31 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/15 08:05:44 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(t_philo *philo)
{
	print_message(philo, 8);
	ft_usleep(philo->main->time_sleep * 1000);
}

int	is_f(t_main *main)
{
	if (main->meals && reader(&main->dead_flag,
			&main->philos->meals_nbr) == main->meals)
		return (1);
	return (0);
}

void	*monitor(void *arg)
{
	t_main	*main;
	int		i;

	main = (t_main *)arg;
	while (1)
	{
		i = 0;
		while (i < main->philo_nbr)
		{
			if (reader(&main->tmp, &main->flag) >= main->philo_nbr)
				return (NULL);
			if (get_time_in_ms() - reader(&main->dead_flag,
					&main->philos[i].last_meal) > main->time_die)
			{
				set_value(&main->dead_flag, &main->dead_sign, 0);
				print_message(&main->philos[i], 7);
				return (NULL);
			}
			i++;
		}
		ft_usleep(600);
	}
	return (NULL);
}

void	*join_threads(t_philo *philos, int philo_nbr)
{
	int	i;

	i = 0;
	while (i < philo_nbr)
	{
		if (pthread_join(philos[i].thread, NULL))
		{
			printf("pthread_join");
			return (NULL);
		}
		i++;
	}
	if (pthread_join(philos->main->monitor, NULL))
	{
		printf("pthread_join monitor");
		return (NULL);
	}
	return (NULL);
}

void	monitor_creation(t_main *main)
{
	if (pthread_create(&main->monitor, NULL, (void *)monitor, main))
	{
		printf("Monitor Creation Failed\n");
		return ;
	}
}
