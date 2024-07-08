#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"

int	grabbing_forks_even(t_philo *new_philos)
{
	if (new_philos->f_id % 2 == 0)
	{
		pthread_mutex_lock(&new_philos->fork_right->lock);
		if (death_check(new_philos))
		{
			pthread_mutex_unlock(&new_philos->fork_right->lock);
			return (0);
		}
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld %zu has taken a fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
		pthread_mutex_lock(&new_philos->fork_left->lock);
		if (death_check(new_philos))
		{
			pthread_mutex_unlock(&new_philos->fork_left->lock);
			pthread_mutex_unlock(&new_philos->fork_right->lock);
			return (0);
		}
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld %zu has taken a fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
	}
	return (1);
}

int	grabbing_forks_odd(t_philo *new_philos)
{
	if (new_philos->f_id % 2 != 0)
	{
		//ft_sleep(new_philos->data->time_to_sleep, new_philos);
		pthread_mutex_lock(&new_philos->fork_left->lock);
		if (death_check(new_philos))
			return(pthread_mutex_unlock(&new_philos->fork_left->lock), 0);
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld %zu has taken a fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
		if (new_philos->data->n_filos == 1)
		{
			ft_sleep(new_philos->data->time_to_die + 1, new_philos);
			if (death_check(new_philos))
				return (pthread_mutex_unlock(&new_philos->fork_left->lock), 0);
		}
		pthread_mutex_lock(&new_philos->fork_right->lock);
		if (death_check(new_philos))
			return (pthread_mutex_unlock(&new_philos->fork_right->lock), pthread_mutex_unlock(&new_philos->fork_left->lock), 0);
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld %zu has taken a fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
	}
	return (1);
}

int	action_eating(t_philo *new_philos)
{
	if (death_check(new_philos))
		return (0);
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld %zu is eating\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);

	pthread_mutex_lock(&new_philos->meal_lock);
	new_philos->last_meal = get_time_in_ms();
	new_philos->meals_eaten++;
	if (new_philos->meals_eaten >= new_philos->data->meals_needed && new_philos->data->meals_needed >= 0)
		new_philos->satisfied = true;
	pthread_mutex_unlock(&new_philos->meal_lock);
	ft_sleep(new_philos->data->time_to_eat, new_philos);
	pthread_mutex_unlock(&new_philos->fork_left->lock);
	pthread_mutex_unlock(&new_philos->fork_right->lock);
	return (1);
}

int	action_sleeping(t_philo *new_philos)
{
	if (death_check(new_philos))
		return (0);
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld %zu is sleeping\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);
	ft_sleep(new_philos->data->time_to_sleep, new_philos);
	return (1);
}
int	action_thinking(t_philo *new_philos)
{
	if (death_check(new_philos))
		return (0);
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld %zu is thinking\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);
	if ((new_philos->data->time_to_die - (2 * new_philos->data->time_to_eat) - new_philos->data->time_to_sleep) > 0) //store this value and this is the amount to sleep
		ft_sleep(new_philos->data->time_to_die - (2 * new_philos->data->time_to_eat) - new_philos->data->time_to_sleep, new_philos); //TRY TO TEST BACK 1MS BUT FROM CODAM PC
	return (1);
}