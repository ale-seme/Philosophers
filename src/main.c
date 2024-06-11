#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

typedef struct s_program
{
	int				n_filos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	bool			is_dead;
	int				meals_needed;
	long int		start_time;
	pthread_mutex_t	death_lock;
}	t_program;

typedef struct s_fork
{
	pthread_mutex_t lock;
	int				fork_id;
}	t_fork;

typedef struct s_philo
{
	
	size_t			f_id;
	struct timeval	tv;
	long int		last_meal;
	pthread_t		thread;
	t_fork			*fork_right;
	t_fork			*fork_left;
	t_program*		data;
	pthread_mutex_t	lock;
	pthread_mutex_t meal_lock;
	//t_program 		*program;	

}	t_philo;

long int	get_time_in_ms()
{
	struct	timeval tv;
	
	gettimeofday(&tv, NULL);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void *monitoring_routine(void *philos)
{
	t_philo *new_philos;
	int i;
	new_philos = (t_philo *)(philos);

	i = 0;
	while(1)
	{
		i = 0;
		while(i < new_philos->data->n_filos)
		{
			pthread_mutex_lock(&new_philos[i].meal_lock);
			if (get_time_in_ms() - new_philos[i].last_meal > new_philos[i].data->time_to_die)
			{
				pthread_mutex_lock(&new_philos[i].data->death_lock);
				new_philos[i].data->is_dead = true;
				pthread_mutex_unlock(&new_philos[i].data->death_lock);
				printf("%ld philo n %zu: has DIED\n", get_time_in_ms() - new_philos[i].data->start_time, new_philos[i].f_id);
				pthread_mutex_unlock(&new_philos[i].meal_lock);
				return NULL;
			}
			pthread_mutex_unlock(&new_philos[i].meal_lock);
		}
		i++;
		usleep(1000);
	}
	return NULL;
}

void *routine(void *philos)
{

	t_philo *new_philos;
	
	new_philos = (t_philo *)(philos);

	while(1)
	{
		pthread_mutex_lock(&new_philos->fork_left->lock);
		pthread_mutex_lock(&new_philos->fork_right->lock);
		//printf("new_philo\nf_id: %lu\ntv: %p\nthread: %p\nfork_right: %p\nfork_left: %p\ndata: %p\n", new_philos->f_id, &new_philos->tv, &new_philos->thread, new_philos->fork_right, new_philos->fork_left, new_philos->data);
		
		printf("%ld philo n :%ld has taken the left fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		printf("%ld philo n :%ld has taken the right fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		printf("%ld philo n :%ld is eating\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_lock(&new_philos->lock);
		new_philos->last_meal = get_time_in_ms();
		pthread_mutex_unlock(&new_philos->lock);
		usleep(new_philos->data->time_to_eat *1000);
		pthread_mutex_unlock(&new_philos->fork_left->lock);
		pthread_mutex_unlock(&new_philos->fork_right->lock);

		pthread_mutex_lock(&new_philos->lock);
		printf("%ld philo n :%ld is sleeping\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->lock);

		usleep(new_philos->data->time_to_sleep *1000);
		printf("%ld philo n :%ld is thinking\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_lock(&new_philos->lock);
		pthread_mutex_unlock(&new_philos->lock);
	}
	return NULL;

}

int main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("not right amout of argument given\n");
		return (1);
	}
	t_program 			p_data;
	t_philo				*philosophers;
	t_fork				*forks;
	pthread_t			monitor;
	int					i;

	//p_data.g_time = 
	
	p_data.n_filos = atoi(argv[1]);
	p_data.time_to_die = atoi(argv[2]);
	p_data.time_to_eat = atoi(argv[3]);
	p_data.time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		p_data.meals_needed = atoi(argv[5]);
	else
		p_data.meals_needed = 0;
	
	philosophers = malloc(sizeof(t_philo) *p_data.n_filos);
	forks = malloc(sizeof(t_fork) *p_data.n_filos);

	pthread_mutex_init(&p_data.death_lock, NULL);
	p_data.start_time = get_time_in_ms();
	p_data.is_dead = false;


	i = 0;
	while(i < p_data.n_filos)
	{
		forks[i].fork_id = i + 1;
		pthread_mutex_init(&forks[i].lock, NULL);
		i++;
	}
	i = 0;
	while(i < p_data.n_filos)
	{
		philosophers[i].f_id = i + 1;
		philosophers[i].data = &p_data;
		pthread_mutex_init(&philosophers[i].lock, NULL);
		pthread_mutex_init(&philosophers[i].meal_lock, NULL);
		philosophers[i].fork_left = &forks[i];
		philosophers[i].last_meal = p_data.start_time;
		if (i == 0)
			philosophers[i].fork_right = &forks[p_data.n_filos - 1];
		else
			philosophers[i].fork_right = &forks[i - 1];
		//pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers);
		printf("philo n: %zu\n", philosophers[i].f_id);
		i++;
	}
	i = 0;
	pthread_create(&monitor, NULL, &monitoring_routine, philosophers);
	while (i < p_data.n_filos)
	{
		pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers[i]);
		i++;
	}
	i = 0;
	while(i < p_data.n_filos)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	//int x = 0;
// 	while(x < p_data.n_filos)
// 	{
// 		printf("philo n: %zu, and his fork left_id: %p, and his fork right_id:%p\n", philosophers[x].f_id, philosophers[x].fork_left, philosophers[x].fork_right);
// 		printf("philo n: %zu, and his fork left_lock: %p, and his fork right_lock: \
// %p\n", philosophers[x].f_id, philosophers[x].fork_left, philosophers[x].fork_right);

// 		x++;
 	//}
	// free(philosophers);
	// free(forks);
}