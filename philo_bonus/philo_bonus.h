/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:17:42 by shebaz            #+#    #+#             */
/*   Updated: 2024/12/01 00:23:42 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include "../philo/philo.h"
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

typedef struct t_philos	t_philos;

typedef struct t_sem
{
	int					philo_nbr;
	t_philos			**philosophers;
	sem_t				*semaphores;
	sem_t				*mess;
	sem_t				*time;
	sem_t				*meal_nbr;
	sem_t				*meal;
	int					forks_nbr;
	int					start_time;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					meals_nbr;
	int					dead_flag;
}						t_sem;

typedef struct t_philos
{
	pthread_t			thread;
	t_sem				*sema;
	pid_t				pid;
	int					id;
	int					meals_nbr;
	int					dead_sign;
	int					start_time;
	int					last_time_eat;
}						t_philos;

long					ft_atoi(char *str);
int						initiale_struct(char **argv, t_sem *sema);
int						initiale_philos(t_sem *sema);
void					p_message(t_philos *philo, int msg_code);
void					kill_all_process(t_philos **philos);
void					clean(t_sem *sema);
int						parse(int argc, char **argv);
int						current_time(t_sem *main, int i);
void					ft_sleeep(t_philos *philo);
void					*philosopher_creation(t_sem *sem);
void					*check_death(void *arg);
void					set_val(int *key, int value, sem_t *sem);
int						read_value(int *value, sem_t *sem);
int						is_full(t_philos *philo);
void					semaphore_initialisation(t_sem *sema, char **argv);
int						has_philosopher_died(t_philos *philo);
#endif
