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
		printf("%ld philo n :%zu has taken the right fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
		pthread_mutex_lock(&new_philos->fork_left->lock);
		if (death_check(new_philos))
		{
			pthread_mutex_unlock(&new_philos->fork_left->lock);
			pthread_mutex_unlock(&new_philos->fork_right->lock);
			return (0);
		}
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld philo n :%zu has taken the left fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
	}
	return (1);
}

int	grabbing_forks_odd(t_philo *new_philos)
{
	if (new_philos->f_id % 2 != 0)
	{
		pthread_mutex_lock(&new_philos->fork_left->lock);
		if (death_check(new_philos))
		{
			pthread_mutex_unlock(&new_philos->fork_left->lock);
			return (0);
		}
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld philo n :%zu has taken the left fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);

		pthread_mutex_lock(&new_philos->fork_right->lock);
		if (death_check(new_philos))
		{
			pthread_mutex_unlock(&new_philos->fork_right->lock);
			pthread_mutex_unlock(&new_philos->fork_left->lock);
			return (0);
		}
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld philo n :%zu has taken the right fork\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
	}
	return (1);
}

int	action_eating(t_philo *new_philos)
{
	if (death_check(new_philos))
		return (0);
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld philo n :%zu is eating\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);
	pthread_mutex_lock(&new_philos->meal_lock);
	new_philos->last_meal = get_time_in_ms();
	new_philos->meals_eaten++;
	if (new_philos->meals_eaten >= new_philos->data->meals_needed && new_philos->data->meals_needed >= 0)
		new_philos->satisfied = true;
	pthread_mutex_unlock(&new_philos->meal_lock);
	ft_sleep(new_philos->data->time_to_eat);
	pthread_mutex_unlock(&new_philos->fork_left->lock);
	pthread_mutex_unlock(&new_philos->fork_right->lock);
	return (1);
}

int	action_sleeping(t_philo *new_philos)
{
	if (death_check(new_philos))
		return (0);
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld philo n :%zu is sleeping\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);
	ft_sleep(new_philos->data->time_to_sleep);
	return (1);
}
int	action_thinking(t_philo *new_philos)
{
	if (death_check(new_philos))
		return (0);
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld philo n :%zu is thinking\n", get_time_in_ms() - new_philos->data->start_time, new_philos->f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);
	return (1);
}


void *routine(void *philos)
{
	t_philo *new_philos = (t_philo *)(philos);

	while (1)
	{
		if (death_check(new_philos))
			break;
		if (!grabbing_forks_even(new_philos));
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