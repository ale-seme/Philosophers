/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_threads.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:25:50 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/16 11:41:44 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

int	even_philos_synch(t_philo *self_philo)
{
	if (self_philo->f_id % 2 == 0)
	{
		ft_sleep(self_philo->data->time_to_eat / 2, self_philo);
		if (death_check(self_philo))
			return (0);
	}
	return (1);
}

void	*routine(void *philos)
{
	t_philo	*self_philo;

	self_philo = (t_philo *)(philos);
	pthread_mutex_lock(&self_philo->data->start_lock);
	pthread_mutex_unlock(&self_philo->data->start_lock);
	pthread_mutex_lock(&self_philo->meal_lock);
	self_philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&self_philo->meal_lock);
	if (!even_philos_synch(self_philo))
		return (NULL);
	while (1)
	{
		if (death_check(self_philo))
			break ;
		if (!grabbing_forks_even(self_philo) || !grabbing_forks_odd(self_philo))
			break ;
		if (death_check(self_philo))
			return (pthread_mutex_unlock(&self_philo->fork_left->lock), \
			pthread_mutex_unlock(&self_philo->fork_right->lock), NULL);
		if (!action_eating(self_philo) || !action_sleeping(self_philo) || \
			!action_thinking(self_philo))
			break ;
	}
	return (NULL);
}

int	create_philos_threads(t_philo *philosophers)
{
	long	i;

	i = -1;
	pthread_mutex_lock(&philosophers->data->start_lock);
	while (++i < philosophers->data->n_filos)
	{
		if (pthread_create(&philosophers[i].thread, NULL, &routine, \
			&philosophers[i]) != 0)
			return (detach_and_error(philosophers, i, ERR_THREAD_F), 0);
	}
	i = 0;
	philosophers->data->start_time = get_time_in_ms();
	pthread_mutex_unlock(&philosophers->data->start_lock);
	return (1);
}

void	join_philos_threads(t_philo *philosophers)
{
	long	i;

	i = -1;
	while (++i < philosophers->data->n_filos)
	{
		pthread_join(philosophers[i].thread, NULL);
	}
}
