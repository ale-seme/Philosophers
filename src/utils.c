#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"

void	display_error(char *error)
{
	int	i;

	i = 0;
	if (error)
	{
		while(error[i])
			i++;
		write(2, error, i);
	}
}

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
		usleep(200);
		time_passed = get_time_in_ms() - start_time;
		if (death_check(new_philos))
			break;
	}
}
int	death_check(t_philo *self_philo)
{
	pthread_mutex_lock(&self_philo->death_lock);
	if (self_philo->is_dead)
	{
		pthread_mutex_unlock(&self_philo->death_lock);
		return (1);
	}
	// if (self_philo->someone_died)//here to focus
	// {
	// 	pthread_mutex_unlock(&self_philo->death_lock);
	// 	return (1);
	// }
	pthread_mutex_unlock(&self_philo->death_lock);
	return (0);
}


