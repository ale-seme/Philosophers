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

int	ft_sleep(long int sleep_time_in_ms, t_philo*new_philos)
{
	long int	start_time;
	long int	time_passed;

	start_time = get_time_in_ms();
	time_passed = 0;
	while(time_passed < sleep_time_in_ms)
	{
		usleep(50 *1000);
		time_passed = get_time_in_ms() - start_time;
		if (death_check(new_philos))
		{
			// pthread_mutex_lock(&new_philos->meal_lock);
			// new_philos->last_meal = new_philos->data->time_to_die;
			// pthread_mutex_unlock(&new_philos->meal_lock);
			return(1);
		}
	}
	return (0);

}

void free_and_destroy(t_program *p_data, t_philo *philos, t_fork *forks)
{
	int i;
	
	i = -1;
	if (p_data)
	{
		pthread_mutex_destroy(&p_data->death_lock);
		pthread_mutex_destroy(&p_data->print_lock);
	}
	if (philos)
	{
		while(++i < p_data->n_filos)
			pthread_mutex_destroy(&philos[i].meal_lock);
		free(philos);
	}
	i = -1;
	if (forks)
	{
		while(++i < p_data->n_filos)
			pthread_mutex_destroy(&forks[i].lock);
		free(forks);
	}
}

