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
	int				meals_needed;
	long int		start_time;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	bool			someone_died;
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
	bool			is_dead;
	pthread_mutex_t meal_lock;
	bool			satisfied;
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
	int x;
	int	total_satisfaction;

	new_philos = (t_philo *)(philos);

	i = 0;
	while(1)
	{
		i = 0;
		x = 0;
		total_satisfaction = 0;
		while(i < new_philos->data->n_filos)
		{
			pthread_mutex_lock(&new_philos[i].meal_lock);
			// int fuck_heredoc = get_time_in_ms() - new_philos[i].last_meal;
			// printf("COMPARING THIS WITH TIME OF DEATH %d for philo number %zu\n", fuck_heredoc, new_philos[i].f_id);
			if (get_time_in_ms() - new_philos[i].last_meal >= new_philos[i].data->time_to_die)
			{
				pthread_mutex_lock(&new_philos->data->print_lock);
				//printf("for philosopher n: %zu the last meal was at : %ld and the current time is: %ld\n", new_philos[i].f_id, new_philos[i].last_meal - new_philos->data->start_time, get_time_in_ms() - new_philos->data->start_time);
				printf("%ld philo n: %zu has DIED\n", get_time_in_ms() - new_philos[i].data->start_time, new_philos[i].f_id);
				pthread_mutex_unlock(&new_philos->data->print_lock);

				pthread_mutex_lock(&new_philos[i].data->death_lock);
				new_philos[i].is_dead = true;
				new_philos[i].data->someone_died = true;
				pthread_mutex_unlock(&new_philos[i].data->death_lock);
				// pthread_mutex_lock(&new_philos[i].data->print_lock);
				// printf("%ld philo n %zu: has DIED\n", get_time_in_ms() - new_philos[i].data->start_time, new_philos[i].f_id);
				// pthread_mutex_unlock(&new_philos[i].data->print_lock);
				pthread_mutex_unlock(&new_philos[i].meal_lock);
				return NULL;
			}
			pthread_mutex_unlock(&new_philos[i].meal_lock);
			i++;
		}
		// pthread_mutex_lock(&new_philos[0].meal_lock);
		// for (x; x < new_philos[0].data->n_filos; x++)
		// {
		// 	//printf("hello\n");
		// 	if (!new_philos[x].satisfied)
		// 		break;
		// }
		// //printf("total satisfaction after looping all the PHILOSSSS %d\n", x);
		// if (++x == new_philos->data->n_filos)
		// {
		// 	printf("FAKING HELL!!\n");
		// 	pthread_mutex_lock(&new_philos[0].data->death_lock);
		// 	new_philos[0].data->someone_died = true;
		// 	pthread_mutex_unlock(&new_philos[0].data->death_lock);
		// }
		pthread_mutex_unlock(&new_philos[0].meal_lock);
		usleep(500);
	}
	return NULL;
}

int	death_check(t_philo *new_philos)
{
	pthread_mutex_lock(&new_philos->data->death_lock);
	if (new_philos->is_dead)
	{
		pthread_mutex_unlock(&new_philos->data->death_lock);
		return (1);
	}
	if (new_philos->data->someone_died)
	{
		pthread_mutex_unlock(&new_philos->data->death_lock);
		//pthread_mutex_unlock(&new_philos->fork_left->lock);
		//pthread_mutex_unlock(&new_philos->fork_right->lock);
		return (1);
	}
	pthread_mutex_unlock(&new_philos->data->death_lock);
	return (0);
}

void *routine(void *philos)
{
    t_philo *new_philos = (t_philo *)(philos);
	int meals_eaten_routine;

    meals_eaten_routine = 0;
    while (1)
    {

		if (death_check(new_philos))
            break;
        if (new_philos->f_id % 2 == 0)
        {
            pthread_mutex_lock(&new_philos->fork_right->lock);
            if (death_check(new_philos))
            {
                pthread_mutex_unlock(&new_philos->fork_right->lock);
                break;
            }
            pthread_mutex_lock(&new_philos->data->print_lock);
            printf("%ld philo n :%zu has taken the right fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
            pthread_mutex_unlock(&new_philos->data->print_lock);

            pthread_mutex_lock(&new_philos->fork_left->lock);
            if (death_check(new_philos))
            {
                pthread_mutex_unlock(&new_philos->fork_left->lock);
                pthread_mutex_unlock(&new_philos->fork_right->lock);
                break;
            }
            pthread_mutex_lock(&new_philos->data->print_lock);
            printf("%ld philo n :%zu has taken the left fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
            pthread_mutex_unlock(&new_philos->data->print_lock);
        }
        else
        {
            pthread_mutex_lock(&new_philos->fork_left->lock);
            if (death_check(new_philos))
            {
                pthread_mutex_unlock(&new_philos->fork_left->lock);
                break;
            }
            pthread_mutex_lock(&new_philos->data->print_lock);
            printf("%ld philo n :%zu has taken the left fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
            pthread_mutex_unlock(&new_philos->data->print_lock);

            pthread_mutex_lock(&new_philos->fork_right->lock);
            if (death_check(new_philos))
            {
                pthread_mutex_unlock(&new_philos->fork_right->lock);
                pthread_mutex_unlock(&new_philos->fork_left->lock);
                break;
            }
            pthread_mutex_lock(&new_philos->data->print_lock);
            printf("%ld philo n :%zu has taken the right fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
            pthread_mutex_unlock(&new_philos->data->print_lock);
        }

        if (death_check(new_philos))
        {
            pthread_mutex_unlock(&new_philos->fork_left->lock);
            pthread_mutex_unlock(&new_philos->fork_right->lock);
            break;
        }

        printf("%ld philo n :%zu is eating\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
        pthread_mutex_unlock(&new_philos->data->print_lock);

        pthread_mutex_lock(&new_philos->meal_lock);
        new_philos->last_meal = get_time_in_ms();
		meals_eaten_routine++;
		if (meals_eaten_routine >= new_philos->data->meals_needed && new_philos->data->meals_needed >= 0)
			new_philos->satisfied = true;
        pthread_mutex_unlock(&new_philos->meal_lock);
        usleep(new_philos->data->time_to_eat * 1000);

        pthread_mutex_unlock(&new_philos->fork_left->lock);
        pthread_mutex_unlock(&new_philos->fork_right->lock);

        if (death_check(new_philos))
            break;
        pthread_mutex_lock(&new_philos->data->print_lock);
        printf("%ld philo n :%zu is sleeping\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
        pthread_mutex_unlock(&new_philos->data->print_lock);

        usleep(new_philos->data->time_to_sleep * 1000);

        if (death_check(new_philos))
            break;
        pthread_mutex_lock(&new_philos->data->print_lock);
        printf("%ld philo n :%zu is thinking\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
        pthread_mutex_unlock(&new_philos->data->print_lock);
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

	
	p_data.n_filos = atoi(argv[1]);
	p_data.time_to_die = atoi(argv[2]);
	p_data.time_to_eat = atoi(argv[3]);
	p_data.time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		p_data.meals_needed = atoi(argv[5]);
	else
		p_data.meals_needed = -33;
	p_data.start_time = get_time_in_ms();
	pthread_mutex_init(&p_data.death_lock, NULL);
	pthread_mutex_init(&p_data.print_lock, NULL);
	p_data.someone_died = false;

	philosophers = malloc(sizeof(t_philo) *p_data.n_filos);
	forks = malloc(sizeof(t_fork) *p_data.n_filos);

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
		philosophers[i].is_dead = false;
		philosophers[i].f_id = i + 1;
		philosophers[i].data = &p_data;
		philosophers[i].fork_left = &forks[i];
		philosophers[i].last_meal = p_data.start_time;
		philosophers[i].satisfied = false;
		if (i == 0)
			philosophers[i].fork_right = &forks[p_data.n_filos - 1];
		else
			philosophers[i].fork_right = &forks[i - 1];
		pthread_mutex_init(&philosophers[i].meal_lock, NULL);
		//printf("philo n: %zu\n", philosophers[i].f_id);
		i++;
	}
	i = 0;
	pthread_create(&monitor, NULL, &monitoring_routine, philosophers);
	while (i < p_data.n_filos)
	{
		//printf("starting time of thilosopher thread n:%zu is:%ld", philosophers[i].f_id, get_time_in_ms() - p_data.start_time);
		pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers[i]);
		i++;
	}
	i = 0;
	
	pthread_join(monitor, NULL);
	while(i < p_data.n_filos)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
	free(philosophers);
	free(forks);
	return (0);
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