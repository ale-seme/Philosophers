/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_and_errors.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:25:23 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/10 19:55:41 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
		while (error[i])
			i++;
		write (2, error, i);
	}
}

void	free_data_and_err(t_program *p_data, const char *error, int stop)
{
	pthread_mutex_t	*data_mutexes[2];
	long			i;

	i = 0;
	data_mutexes[0] = &p_data->print_lock;
	data_mutexes[1] = &p_data->start_lock;
	if (p_data)
	{
		while (i < stop)
		{
			pthread_mutex_destroy(data_mutexes[i]);
			i++;
		}
		write (2, error, 29);
	}
}

void	free_and_error(t_program *p_data, t_philo *philos, t_fork *forks, \
	char *err)
{
	if (p_data)
	{
		pthread_mutex_destroy(&p_data->print_lock);
		pthread_mutex_destroy(&p_data->start_lock);
	}
	if (philos)
	{
		free(philos);
	}
	if (forks)
	{
		free(forks);
	}
	display_error(err);
}

void	destroy_free_and_err(t_philo *philos, t_fork *forks, long n, long stop)
{
	long	i;
	char	*err;

	i = -1;
	err = NULL;
	if (n == 1)
		pthread_mutex_destroy(&philos[stop].meal_lock);
	else if (n == 2)
	{
		pthread_mutex_destroy(&philos[stop].meal_lock);
		pthread_mutex_destroy(&philos[stop].death_lock);
	}
	while (++i < stop)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);
		pthread_mutex_destroy(&philos[i].death_lock);
		pthread_mutex_destroy(&forks[i].lock);
	}
	if (n >= 0 && n <= 2)
		err = ERR_MUTEX_INIT;
	else if (n == 3)
		err = ERR_THREAD_F;
	free_and_error(philos->data, philos, forks, err);
}

void	detach_and_error(t_philo *philosophers, long stop, char *err)
{
	long	i;

	i = -1;
	while (++i < stop)
	{
		pthread_detach(philosophers[i].thread);
	}
	display_error(err);
}
