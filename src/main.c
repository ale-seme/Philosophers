#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"


long int	get_time_in_ms()
{
	struct	timeval tv;
	
	gettimeofday(&tv, NULL);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_sleep(long int sleep_time_in_ms)
{
	long int	start_time;
	long int	time_passed;

	start_time = get_time_in_ms();
	time_passed = 0;
	while(time_passed < sleep_time_in_ms)
	{
		usleep(50 *1000);
		time_passed = get_time_in_ms() - start_time;
	}
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
			if (new_philos[i].satisfied)
				total_satisfaction++;
			pthread_mutex_unlock(&new_philos[i].meal_lock);
			i++;
		}
		if (total_satisfaction == new_philos->data->n_filos)
		{
			pthread_mutex_lock(&new_philos[0].data->death_lock);
			new_philos[0].data->someone_died = true;
			pthread_mutex_unlock(&new_philos[0].data->death_lock);
			return (NULL);
		}
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
		return (1);
	}
	pthread_mutex_unlock(&new_philos->data->death_lock);
	return (0);
}

int main(int argc, char **argv)
{
	t_program 			p_data;
	t_philo				*philosophers;
	t_fork				*forks;
	pthread_t			monitor;
	int					i;

	if (argc < 5 || argc > 6)
	{
		printf("not right amout of arguments given\n");
		return (1);
	}
	initialize_data(&p_data, argc, argv);
	init_forks_and_philos(philosophers, forks, &p_data);
	pthread_create(&monitor, NULL, &monitoring_routine, philosophers);
	create_philos_threads(philosophers);
	pthread_join(monitor, NULL);
	join_philos_threads(philosophers);
	free(philosophers);
	free(forks);
	return (0);
}
