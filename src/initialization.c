/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialization.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ale <ale@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/21 00:23:29 by ale           #+#    #+#                 */
/*   Updated: 2024/06/21 00:39:40 by ale           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

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