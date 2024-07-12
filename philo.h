#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_philosopher
{
	int				id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_info	*info;
}					t_philosopher;

typedef struct s_info
{
	int				num_philosophers;
	int				time_eat;
	int				time_sleep;
	int				time_die;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_mutex;
	int				*time_since_last_meal;
	int				stop_simulation;
}					t_info;