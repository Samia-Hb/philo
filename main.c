/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:27:04 by shebaz            #+#    #+#             */
/*   Updated: 2024/07/03 23:03:19 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <semaphore.h>
typedef struct philo
{
    pthread_t	thread;
    int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
}t_philo;

int var = 0;
pthread_mutex_t mutex;

void *F1()
{
    int i = 0;
    while (i < 1000000)
    {
        pthread_mutex_lock(&mutex);
        var++;
        i++;
        pthread_mutex_unlock(&mutex);
    }
    return (NULL);
}
int main(int ac, char **av)
{
    if(ac != 4)
        return (0);
    int i = 0;
    int j = 0;
    int philo_nbr = atoi(av[1]);
    int fork_nbr = philo_nbr;
    pthread_t p[philo_nbr];
    
    t_philo p[philo_nbr];
    pthread_mutex_init(&mutex, NULL);
    while(i < philo_nbr)
    {
        if (pthread_create(&p[i], NULL, &F1, NULL) != 0)
            return (1);
        i++;
    }
    while (j < philo_nbr)
    {
        pthread_join(p[j], NULL);
        j++;
    }
    printf("%d\n",var);
    pthread_mutex_destroy(&mutex);
}