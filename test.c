#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_philosopher
{
    int id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_info *info;
} t_philosopher;

typedef struct s_info
{
    int num_philosophers;
    int time_eat;
    int time_sleep;
    int time_die;
    t_philosopher *philosophers;
    pthread_mutex_t *forks;
    pthread_mutex_t death_mutex;
    int *time_since_last_meal;
    int stop_simulation;
} t_info;

void eat(t_philosopher *philosopher, int time)
{
    printf("Philosopher %d is eating.\n", philosopher->id);
    usleep(time);
}

void ft_sleep(t_philosopher *philosopher, int time)
{
    printf("Philosopher %d is sleeping.\n", philosopher->id);
    usleep(time);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_info *info = philosopher->info;

    while (!info->stop_simulation)
    {
        // Thinking
        printf("Philosopher %d is thinking.\n", philosopher->id);
        
        // Picking up forks
        if (philosopher->id % 2 == 0)
        {
            pthread_mutex_lock(philosopher->right_fork);
            printf("Philosopher %d has taken the right fork.\n", philosopher->id);
            pthread_mutex_lock(philosopher->left_fork);
            printf("Philosopher %d has taken the left fork.\n", philosopher->id);
        }
        else
        {
            pthread_mutex_lock(philosopher->left_fork);
            printf("Philosopher %d has taken the left fork.\n", philosopher->id);
            pthread_mutex_lock(philosopher->right_fork);
            printf("Philosopher %d has taken the right fork.\n", philosopher->id);
        }

        // Eating
        eat(philosopher, info->time_eat);
        pthread_mutex_lock(&info->death_mutex);
        info->time_since_last_meal[philosopher->id] = 0;
        pthread_mutex_unlock(&info->death_mutex);

        // Putting down forks
        pthread_mutex_unlock(philosopher->left_fork);
        printf("Philosopher %d has put down the left fork.\n", philosopher->id);
        pthread_mutex_unlock(philosopher->right_fork);
        printf("Philosopher %d has put down the right fork.\n", philosopher->id);

        // Sleeping
        ft_sleep(philosopher, info->time_sleep);
    }
    return NULL;
}

void *monitor_routine(void *arg)
{
    t_info *info = (t_info *)arg;

    while (!info->stop_simulation)
    {
        for (int i = 0; i < info->num_philosophers; i++)
        {
            pthread_mutex_lock(&info->death_mutex);
            info->time_since_last_meal[i] += 1000;
            if (info->time_since_last_meal[i] >= info->time_die)
            {
                printf("Philosopher %d has died.\n", i);
                info->stop_simulation = 1;
                pthread_mutex_unlock(&info->death_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&info->death_mutex);
        }
        usleep(1000);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("Required: %s num_philosophers time_eat time_sleep time_die\n", argv[0]);
        return 1;
    }
    t_info info;
    int i = 0;
    info.num_philosophers = atoi(argv[1]);
    info.time_eat = atoi(argv[2]) * 1000;
    info.time_sleep = atoi(argv[3]) * 1000;
    info.time_die = atoi(argv[4]) * 1000;
    info.stop_simulation = 0;
    info.philosophers = (t_philosopher *)malloc(info.num_philosophers * sizeof(t_philosopher));
    info.forks = (pthread_mutex_t *)malloc(info.num_philosophers * sizeof(pthread_mutex_t));
    info.time_since_last_meal = (int *)malloc(info.num_philosophers * sizeof(int));
    while (i < info.num_philosophers)
    {
        pthread_mutex_init(&info.forks[i], NULL);
        info.philosophers[i].id = i;
        info.philosophers[i].left_fork = &info.forks[i];
        info.philosophers[i].right_fork = &info.forks[(i + 1) % info.num_philosophers];
        info.philosophers[i].info = &info;
        info.time_since_last_meal[i] = 0;
        i++;
    }
    pthread_mutex_init(&info.death_mutex, NULL);
    pthread_t threads[info.num_philosophers];
    pthread_t monitor_thread;
    i = 0;
    while (i < info.num_philosophers)
    {
        if (pthread_create(&threads[i], NULL, philosopher_routine, &info.philosophers[i]))
        {
            perror("Failed to create thread");
            return 1;
        }
        i++;
    }
    if (pthread_create(&monitor_thread, NULL, monitor_routine, &info))
    {
        perror("Failed to create monitor thread");
        return 1;
    }
    for (int i = 0; i < info.num_philosophers; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            perror("Failed to join thread");
            return 1;
        }
    }
    pthread_join(monitor_thread, NULL);
    for (int i = 0; i < info.num_philosophers; i++)
    {
        pthread_mutex_destroy(&info.forks[i]);
    }
    pthread_mutex_destroy(&info.death_mutex);
    free(info.philosophers);
    free(info.forks);
    free(info.time_since_last_meal);
    return 0;
}
