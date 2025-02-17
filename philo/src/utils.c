/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:25:56 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/11 15:25:15 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

long int	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	safe_usleep(long int sleep_time_in_ms)
{
	long int	start_time;
	long int	time_passed;

	start_time = get_time_in_ms();
	time_passed = 0;
	while (time_passed < sleep_time_in_ms)
	{
		usleep(500);
		time_passed = get_time_in_ms() - start_time;
	}
}

void	ft_sleep(long int sleep_time_in_ms, t_philo *new_philos)
{
	long int	start_time;
	long int	time_passed;

	start_time = get_time_in_ms();
	time_passed = 0;
	while (time_passed < sleep_time_in_ms)
	{
		usleep(200);
		time_passed = get_time_in_ms() - start_time;
		if (death_check(new_philos))
			break ;
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
	pthread_mutex_unlock(&self_philo->death_lock);
	return (0);
}
