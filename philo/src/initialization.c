/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialization.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ale <ale@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/21 00:23:29 by ale           #+#    #+#                 */
/*   Updated: 2024/07/11 15:23:15 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

int	atoi_is_overflow(t_program *p_data)
{
	if (p_data->n_filos == 0 \
		|| p_data->time_to_die == 0 \
		|| p_data->time_to_eat == 0 || p_data->time_to_sleep == 0 \
		|| p_data->meals_needed == 0)
	{
		display_error(ERR_LONG_OVERFLOW);
		return (1);
	}
	return (0);
}

int	initialize_data(t_program *p_data, int argc, char **argv)
{
	p_data->n_filos = simple_atoi(argv[1]);
	p_data->time_to_die = simple_atoi(argv[2]);
	p_data->time_to_eat = simple_atoi(argv[3]);
	p_data->time_to_sleep = simple_atoi(argv[4]);
	if (argc == 6)
		p_data->meals_needed = simple_atoi(argv[5]);
	else
		p_data->meals_needed = -1;
	if (atoi_is_overflow(p_data))
		return (0);
	if (pthread_mutex_init(&p_data->print_lock, NULL) == -1)
		return (free_data_and_err(p_data, ERR_MUTEX_D, 0), 0);
	if (pthread_mutex_init(&p_data->start_lock, NULL) == -1)
		return (free_data_and_err(p_data, ERR_MUTEX_D, 1), 0);
	p_data->synchronized = false;
	return (1);
}

void	populate_everything(t_philo *philos, t_fork *forks, t_program *p_data, \
		long i)
{
	philos[i].f_id = i + 1;
	philos[i].data = p_data;
	philos[i].fork_left = &forks[i];
	philos[i].last_meal = get_time_in_ms();
	philos[i].meals_eaten = 0;
	philos[i].is_dead = false;
	philos[i].someone_died = false;
	philos[i].satisfied = false;
	if (p_data->n_filos == 1)
		philos[i].fork_right = &forks[i];
	else if (i == 0)
		philos[i].fork_right = &forks[p_data->n_filos - 1];
	else
		philos[i].fork_right = &forks[i - 1];
}

int	init_forks_and_philos(t_philo *philos, t_fork *forks, t_program *p_data)
{
	long	i;

	i = -1;
	while (++i < p_data->n_filos)
	{
		populate_everything(philos, forks, p_data, i);
		if (pthread_mutex_init(&philos[i].meal_lock, NULL) == -1)
			return (destroy_free_and_err(philos, forks, 0, i), 0);
		if (pthread_mutex_init(&philos[i].death_lock, NULL) == -1)
			return (destroy_free_and_err(philos, forks, 1, i), 0);
		if (pthread_mutex_init(&forks[i].lock, NULL) == -1)
			return (destroy_free_and_err(philos, forks, 2, i), 0);
	}
	return (1);
}
