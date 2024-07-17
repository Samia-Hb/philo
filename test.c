#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t *dead_lock;
	pthread_mutex_t *meal_lock;
}					t_philo;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t *forks;
	t_philo			*philos;
}					t_program;

void initialize_variables(t_program *program, char **argv)
{
	int i;
	int num_philo;

	i = 0;
	num_philo = atoi(argv[1]);
	program->philos = (t_philo *)malloc(num_philo * sizeof(t_philo));
	program->forks = (pthread_mutex_t *)malloc(num_philo * sizeof(pthread_mutex_t));
	program->dead_flag = 1; // Initialize the dead_flag
	while (i < num_philo)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		program->philos[i].id = i;
		program->philos[i].eating = 0;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal = 0;
		program->philos[i].time_to_die = (size_t)atoi(argv[2]);
		program->philos[i].time_to_eat = (size_t)atoi(argv[3]);
		program->philos[i].time_to_sleep = (size_t)atoi(argv[4]);
		program->philos[i].num_times_to_eat = atoi(argv[5]);
		program->philos[i].start_time = 0;
		program->philos[i].num_of_philos = num_philo;
		program->philos[i].r_fork = &program->forks[i];
		program->philos[i].l_fork = &program->forks[(i + 1) % num_philo];
		program->philos[i].write_lock = &program->write_lock;
		program->philos[i].dead_lock = &program->dead_lock;
		program->philos[i].meal_lock = &program->meal_lock;
		i++;
	}
}

void ft_eat(t_philo *philosopher)
{
	printf("Philosopher %d is eating\n", philosopher->id + 1);
	usleep(philosopher->time_to_eat);
}

void ft_sleep(t_philo *philosopher)
{
	printf("Philosopher %d is sleeping\n", philosopher->id + 1);
	usleep(philosopher->time_to_sleep);
}

void *philosopher_routine(void *argc)
{
	t_philo *philosopher = (t_philo *)argc;
	while(!philosopher->dead)
	{
		printf("Philosopher %d is thinking\n", philosopher->id + 1);
		if (philosopher->id == philosopher->num_of_philos - 1)
		{
			pthread_mutex_lock(philosopher->r_fork);
			printf("Philosopher %d has taken the right fork\n", philosopher->id + 1);
			pthread_mutex_lock(philosopher->l_fork);
			printf("Philosopher %d has taken the left fork\n", philosopher->id + 1);
		}
		else
		{
			pthread_mutex_lock(philosopher->l_fork);
			printf("Philosopher %d has taken the left fork\n", philosopher->id + 1);
			pthread_mutex_lock(philosopher->r_fork);
			printf("Philosopher %d has taken the right fork\n", philosopher->id + 1);
		}
		ft_eat(philosopher);
		philosopher->meals_eaten++;
		philosopher->last_meal = 0;
		pthread_mutex_unlock(philosopher->l_fork);
		printf("Philosopher %d has put down the left fork\n", philosopher->id + 1);
		pthread_mutex_unlock(philosopher->r_fork);
		printf("Philosopher %d has put down the right fork\n", philosopher->id + 1);
		ft_sleep(philosopher);
	}
	return (NULL);
}
size_t get_time_in_ms(void)
{
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void *monitor(void *arg)
{
    t_program *prog = (t_program *)arg;
    int i;

    while (!prog->dead_flag && prog->philos->meals_eaten)
    {
        i = 0;
        while (i < prog->philos->num_of_philos)
        {
            pthread_mutex_lock(prog->philos[i].meal_lock);
            if ((get_time_in_ms() - prog->philos[i].last_meal) > prog->philos[i].time_to_die)
            {
                pthread_mutex_lock(prog->philos[i].dead_lock);
                prog->dead_flag = 1;
                printf("philosopher %d is died\n", i);;
                pthread_mutex_unlock(prog->philos[i].dead_lock);
            }
            pthread_mutex_unlock(prog->philos[i].meal_lock);
            i++;
        }
        usleep(1000);
		prog->philos->meals_eaten--;
    }
    return (NULL);
}

int main(int argc, char **argv)
{
	if (argc != 6 && argc != 5)
	{
		printf("Usage: %s num_of_philos time_to_die time_to_eat time_to_sleep num_times_to_eat meals_number(not required)\n", argv[0]);
		return (1);
	}
	int i;
	t_program *program = (t_program *)malloc(sizeof(t_program));
	pthread_t monitor_thread;
	if (pthread_create(&monitor_thread, NULL, monitor, (void *)program) != 0)
	{
		perror("Failed to create monitor thread");
		free(program);
		return (1);
	}
	pthread_detach(monitor_thread);
	initialize_variables(program, argv);
	i = 0;
	while(i < atoi(argv[1]))
	{
		if (pthread_create(&program->philos[i].thread, NULL, philosopher_routine, (void *)&program->philos[i]) != 0)
		{
			write(2, "Error Creating Thread\n", 23);
			return(0);
		}
		i++;
	}
	i = 0;
	while (i < atoi(argv[1]))
	{
		pthread_join(program->philos[i].thread, NULL);
		i++;
	}
	free(program->philos);
	free(program->forks);
	free(program);
	return 0;
}

