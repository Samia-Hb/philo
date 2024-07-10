# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_philosopher
{
    int id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    int time_eat;
    int time_die;
    int time_sleep;
    int time_last_meal;
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
} t_info;

void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_info *info = philosopher->info;
    int philosopher_id = philosopher->id;
    int j = 1;
    while (1)
    {
        while (j && info->time_die)
        {
            printf("Philosopher %d is thinking.\n", philosopher_id);
            info->time_die--;
            if (!info->time_die)
            {
                info->philosophers[philosopher_id].alive = 0;
                break;
            }
        }
        if (info->philosophers[philosopher_id].alive == 0)
        {
            printf("Philosopher %d is dead.\n", philosopher_id);
            break;
        }
        pthread_mutex_lock(philosopher->left_fork);
        pthread_mutex_lock(philosopher->right_fork);
        j = 0;
        printf("Philosopher %d is eating.\n", philosopher_id);
        usleep(info->time_eat);
        pthread_mutex_lock(&info->death_mutex);
        info->time_since_last_meal[philosopher_id] = 0;
        pthread_mutex_unlock(&info->death_mutex);
        pthread_mutex_unlock(philosopher->right_fork);
        pthread_mutex_unlock(philosopher->left_fork);
        printf("Philosopher %d is sleeping.\n", philosopher_id);
        usleep(info->time_sleep);
    }
    return (NULL);
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("Requiered: %s num_philosophers time_eat time_sleep time_die\n", argv[0]);
        return 1;
    }
    int i;
    t_info info;
    info.num_philosophers = atoi(argv[1]);
    info.time_eat = atoi(argv[2]) * 1000;
    info.time_sleep = atoi(argv[3]) * 1000;
    info.time_die = atoi(argv[4]) * 1000;
    pthread_t threads[info.num_philosophers];
    info.philosophers = malloc(info.num_philosophers * sizeof(t_philosopher));
    info.forks = malloc(info.num_philosophers * sizeof(pthread_mutex_t));
    info.time_since_last_meal = malloc(info.num_philosophers * sizeof(int));
    i = 0;
    while(i < info.num_philosophers)
    {
        pthread_mutex_init(&info.forks[i], NULL);
        info.philosophers[i].id = i;
        info.philosophers[i].left_fork = &info.forks[i];
        info.philosophers[i].right_fork = &info.forks[(i + 1) % info.num_philosophers];
        info.philosophers[i].info = &info;
        info.time_since_last_meal[i] = 0;
        info.philosophers[i].alive = 1;
        i++;
    }
    pthread_mutex_init(&info.death_mutex, NULL);
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
    while (1)
    {
        i = 0;
        while (i < info.num_philosophers)
        {
            pthread_mutex_lock(&info.death_mutex);
            info.time_since_last_meal[i]++;
            if (info.time_since_last_meal[i] > info.time_die)
            {
                printf("Philosopher %d is dead.\n", i);
                exit(1);
            }
            pthread_mutex_unlock(&info.death_mutex);
            i++;
        }
        usleep(1000);
    }
    i = 0;
    while (i < info.num_philosophers)
    {
        if (pthread_join(threads[i], NULL))
        {
            perror("Failed to join thread");
            return 1;
        }
        i++;
    }
    i = 0;
    while (i < info.num_philosophers)
    {
        pthread_mutex_destroy(&info.forks[i]);
        i++;
    }
    pthread_mutex_destroy(&info.death_mutex);
    free(info.philosophers);
    free(info.forks);
    free(info.time_since_last_meal);

    return 0;
}



