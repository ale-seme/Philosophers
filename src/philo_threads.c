#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"


void *routine(void *philos)
{
	t_philo *new_philos = (t_philo *)(philos);

	while(!new_philos->data->synchronized)
		continue;
	while (1)
	{
		if (death_check(new_philos))
			break;
		if (!grabbing_forks_even(new_philos))
			break;
		if (!grabbing_forks_odd(new_philos))
			break;
		if (death_check(new_philos))
		{
			pthread_mutex_unlock(&new_philos->fork_left->lock);
			pthread_mutex_unlock(&new_philos->fork_right->lock);
			break;
		}
		if (!action_eating(new_philos))
			break ;
		if (!action_sleeping(new_philos))
			break ;
		if (!action_thinking(new_philos))
			break ;
	}
	return (NULL);
}

void	create_philos_threads(t_philo *philosophers)
{
	int i;

	i = -1;
	while(++i < philosophers->data->n_filos)
		pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers[i]);
	pthread_mutex_lock(&philosophers->data->start_lock);
	philosophers->data->synchronized = true;
	//philosophers->data->start_time = get_time_in_ms();
	pthread_mutex_unlock(&philosophers->data->start_lock);
}

void    join_philos_threads(t_philo *philosophers)
{
	int i;

	i = -1;
	while(++i < philosophers->data->n_filos)
	{
		pthread_join(philosophers[i].thread, NULL);
	}
}