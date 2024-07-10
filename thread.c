#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *count_to_100(void *arg)
{
	(void)arg;
	int i = 0;
	while(i < 100)
	{ 
		printf("The count has finished\n");
		i++;
	}
	usleep(10000);
	return (NULL);
}

void *print_hello(void *arg)
{
	int i = 0;
	(void)arg;
	while(i < 100)
	{
		printf("Hello, world!\n");
		i++;
	}
	usleep(10000);
	return (NULL);
}
typedef struct philo
{
	int must_eat;
	int last_time_meal;
	
}t_phil ;
void ft_odd_phil(t_phil *phil)
{
 while (phil->must_eat)
 {
  pthread_mutex_lock(&phil->var->forks[phil->fr_fork]);
  printf("%ld %d %s\n", get_time_in_ms() - phil->var->start_time, \
    phil->pos, "has taken a fork");
  pthread_mutex_lock(&phil->var->forks[phil->sc_fork]);
  printf("%ld %d %s\n", get_time_in_ms() - phil->var->start_time, \
    phil->pos, "has taken a fork");
  printf("%ld %d %s\n", get_time_in_ms() - phil->var->start_time, \
    phil->pos, "is eating");
  if (ft_sleep(phil, phil->var->time_to_eat))
   break ;
  phil->last_time_ate = get_time_in_ms();
  pthread_mutex_unlock(&phil->var->forks[phil->sc_fork]);
  pthread_mutex_unlock(&phil->var->forks[phil->fr_fork]);
  printf("%ld %d %s\n", get_time_in_ms() - phil->var->start_time, \
    phil->pos, "is sleeping");
  if (ft_sleep(phil, phil->var->time_to_sleep))
   break ;
  printf("%ld %d %s\n", get_time_in_ms() - phil->var->start_time, \
    phil->pos, "is thinking");
  phil->must_eat--;
 }
}

int main()
{
	pthread_t thread_1;
	pthread_t thread_2;

	pthread_create(&thread_1, NULL, count_to_100, NULL);
	pthread_create(&thread_2, NULL, print_hello, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
}
///////////////