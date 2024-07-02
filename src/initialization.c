/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialization.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ale <ale@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/21 00:23:29 by ale           #+#    #+#                 */
/*   Updated: 2024/07/02 22:58:54 by ale           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

int	initialize_data(t_program *p_data, int argc, char **argv)
{
	p_data->n_filos = atoi(argv[1]);
	p_data->time_to_die = atoi(argv[2]);
	p_data->time_to_eat = atoi(argv[3]);
	p_data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		p_data->meals_needed = atoi(argv[5]);
	else
		p_data->meals_needed = -1;
	//p_data->start_time = get_time_in_ms();
	// if (pthread_mutex_init(&p_data->death_lock, NULL) == -1)
	// 	return (free_data_and_err(p_data, ERR_MUTEX_D, 0), 0);
	if (pthread_mutex_init(&p_data->print_lock, NULL) == -1)
		return (free_data_and_err(p_data, ERR_MUTEX_D, 0), 0);
	if (pthread_mutex_init(&p_data->start_lock, NULL) == -1)
		return (free_data_and_err(p_data, ERR_MUTEX_D, 1), 0);
	if (pthread_mutex_init(&p_data->start_monitoring, NULL) == -1)
		return (free_data_and_err(p_data, ERR_MUTEX_D, 2), 0);
	p_data->synchronized = false;
	//p_data->someone_died = false;
	return (1);
}
void	init_forks_and_philos(t_philo *philos, t_fork *forks, t_program *p_data)
{
	int i;
	
	i = -1;
	while(++i < p_data->n_filos)
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
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		pthread_mutex_init(&philos[i].death_lock, NULL);
		pthread_mutex_init(&forks[i].lock, NULL);
	}
}

