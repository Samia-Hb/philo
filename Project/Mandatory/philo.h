/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:17:42 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/18 04:52:45 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../philo_bonus/philo_bonus.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct philo	t_philo;

typedef struct main_struct
{
	pthread_mutex_t		message_lock;
	pthread_mutex_t		dead_flag;
	pthread_mutex_t		meal_flag;
	pthread_mutex_t		*forks;
	pthread_mutex_t		tmp;
	pthread_t			monitor;
	t_philo				*philos;
	int					start_time;
	int					time_eat;
	int					time_die;
	int					time_sleep;
	int					philo_nbr;
	int					meals;
	int					dead_sign;
	int					flag;
}						t_main;

typedef struct philo
{
	pthread_t			thread;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	t_main				*main;
	int					id;
	int					last_meal;
	int					meals_nbr;
}						t_philo;

void					*routine(void *arg);
int						is_fulll(t_philo *philo);
void					print_message(t_philo *philo, int msg_code);
int						get_time_in_ms(void);
int						ft_usleep(int usec);
int						parse(int argc, char **argv);
void					ft_clean(t_main *main);
int						calculate_time(t_main *main);
int						reader(pthread_mutex_t *mutex, int *var);
void					set_value(pthread_mutex_t *mutex, int *var, int value);
void					initialize_struct(char **argv, t_main *main);
void					create_threads(t_main *main, int philo_nbr);
void					*join_threads(t_philo *philos, int philo_nbr);
void					monitor_creation(t_main *main);
void					ft_sleep(t_philo *philo);
int						is_fulll(t_philo *philo);
#endif