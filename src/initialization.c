/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialization.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ale <ale@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/21 00:23:29 by ale           #+#    #+#                 */
/*   Updated: 2024/06/21 17:54:29 by ale           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void free_and_destroy(t_program *p_data, t_philo *philos, t_fork *forks)
{
	int i;
	
	i = 0;
	if (p_data)
	{
		pthread_mutex_destroy(&p_data->death_lock);
		pthread_mutex_destroy(&p_data->print_lock);
	}
	if (philos)
	{
		while(i < p_data->n_filos)
		{
			pthread_mutex_destroy(&philos[i].meal_lock);
		}
		free(philos);
	}
	
}

void	initialize_data(t_program *p_data, int argc, char **argv)
{
	p_data->n_filos = atoi(argv[1]);
	p_data->time_to_die = atoi(argv[2]);
	p_data->time_to_eat = atoi(argv[3]);
	p_data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		p_data->meals_needed = atoi(argv[5]);
	else
		p_data->meals_needed = -1;
	p_data->start_time = get_time_in_ms();
	pthread_mutex_init(&p_data->death_lock, NULL);
	pthread_mutex_init(&p_data->print_lock, NULL);
	p_data->someone_died = false;
}
void	init_forks_and_philos(t_philo *philos, t_fork *forks, t_program *p_data)
{
	int i;
	
	i = -1;
	philos = malloc(sizeof(t_philo) * p_data->n_filos);
	if (!philos)
		return (free_and_destroy(p_data, NULL, NULL));
	forks = malloc(sizeof(t_fork) * p_data->n_filos);
	if (!forks)
		return (free_and_destroy(p_data, philos, NULL));
	while(++i < p_data->n_filos)
	{
		philos[i].is_dead = false;
		philos[i].f_id = i + 1;
		philos[i].data = p_data;
		philos[i].fork_left = &forks[i];
		philos[i].last_meal = p_data->start_time;
		philos[i].meals_eaten = 0;
		philos[i].satisfied = false;
		if (i == 0)
			philos[i].fork_right = &forks[p_data->n_filos - 1];
		else
			philos[i].fork_right = &forks[i - 1];
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		pthread_mutex_init(&forks[i].lock, NULL);
	}
}

