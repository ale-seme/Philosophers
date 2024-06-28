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

void	ft_sleep(long int sleep_time_in_ms, t_philo *new_philos)
{
	long int	start_time;
	long int	time_passed;

	start_time = get_time_in_ms();
	time_passed = 0;
	while(time_passed < sleep_time_in_ms)
	{
		usleep(2000);
		time_passed = get_time_in_ms() - start_time;
		if (death_check(new_philos))
			break;
	}
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


