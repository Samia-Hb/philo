/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:37:21 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/18 04:50:46 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_threads(t_main *main, int philo_nbr)
{
	int	i;

	i = 0;
	main->start_time = get_time_in_ms();
	while (i < philo_nbr)
	{
		main->philos[i].main = main;
		main->philos[i].last_meal = main->start_time;
		if (pthread_create(&main->philos[i].thread, NULL, routine,
				&main->philos[i]))
		{
			printf("Thread Creation Failed\n");
			return ;
		}
		i++;
	}
}

void	print_message(t_philo *philo, int msg_code)
{
	int	time;

	pthread_mutex_lock(&philo->main->message_lock);
	time = calculate_time(philo->main);
	if (msg_code == 4 && reader(&philo->main->dead_flag,
			&philo->main->dead_sign) && !is_fulll(philo))
		printf("%d  %d has taken a fork\n", time, philo->id);
	else if (msg_code == 5 && reader(&philo->main->dead_flag,
			&philo->main->dead_sign))
		printf("%d  %d is eating\n", time, philo->id);
	else if (msg_code == 6 && reader(&philo->main->dead_flag,
			&philo->main->dead_sign) && !is_fulll(philo))
		printf("%d  %d is thinking\n", time, philo->id);
	else if (msg_code == 7 && (reader(&philo->main->tmp,
				&philo->main->flag) != philo->main->philo_nbr))
		printf("%d  %d died\n", time, philo->id);
	else if (msg_code == 8 && reader(&philo->main->dead_flag,
			&philo->main->dead_sign) && !is_fulll(philo))
		printf("%d  %d is sleeping\n", time, philo->id);
	pthread_mutex_unlock(&philo->main->message_lock);
}

void	init_philos(t_main *main)
{
	int	i;

	i = 0;
	while (i < main->philo_nbr)
	{
		main->philos[i].id = i + 1;
		main->philos[i].meals_nbr = 0;
		main->philos[i].right_fork = &main->forks[i];
		main->philos[i].left_fork = &main->forks[(i + 1) % main->philo_nbr];
		pthread_mutex_init(&main->forks[i], NULL);
		i++;
	}
}

void	initialize_struct(char **argv, t_main *main)
{
	main->philos = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	main->forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(argv[1]));
	main->philo_nbr = ft_atoi(argv[1]);
	main->time_die = ft_atoi(argv[2]);
	main->flag = 0;
	main->time_eat = ft_atoi(argv[3]);
	main->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
		main->meals = ft_atoi(argv[5]);
	else
		main->meals = 0;
	main->dead_sign = 1;
	if (!main->philos || !main->forks)
	{
		printf("Allocation Failed.\n");
		return ;
	}
	init_philos(main);
	pthread_mutex_init(&main->message_lock, NULL);
	pthread_mutex_init(&main->dead_flag, NULL);
	pthread_mutex_init(&main->meal_flag, NULL);
	pthread_mutex_init(&main->tmp, NULL);
}

int	main(int argc, char **argv)
{
	t_main	*main;

	if (!parse(argc, argv))
		return (0);
	main = malloc(sizeof(t_main));
	if (!main)
	{
		printf("Allocation Failed.\n");
		return (0);
	}
	initialize_struct(argv, main);
	create_threads(main, main->philo_nbr);
	monitor_creation(main);
	join_threads(main->philos, main->philo_nbr);
	ft_clean(main);
	return (0);
}
