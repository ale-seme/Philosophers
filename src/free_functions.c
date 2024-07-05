#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"




int	free_data_and_err(t_program *p_data, const char *error, int stop)
{
	pthread_mutex_t *data_mutexes[2];
	long	i;

	i = 0;
	data_mutexes[0] = &p_data->print_lock;
	data_mutexes[1] = &p_data->start_lock;
	if (p_data)
	{
		while(i < stop)
		{
			pthread_mutex_destroy(data_mutexes[i]);
			i++;
		}
		write(2, ERR_MUTEX_D, 29);
		return (0);
	}
	return (1);
}

void free_and_error(t_program *p_data, t_philo *philos, t_fork *forks, char *err)
{
	long	i;
	
	if (p_data)
	{
		pthread_mutex_destroy(&p_data->print_lock);
		pthread_mutex_destroy(&p_data->start_lock);
	}
	i = -1;
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
	display_error(err);
}
