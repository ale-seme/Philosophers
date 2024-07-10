/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:26:08 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/10 16:48:10 by asemerar      ########   odam.nl         */
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

//strace -f -e trace=futex -tt -T -o strace_output ./philo

void	display_as_last(long flag, t_philo *new_philos, long n)
{
	if (!flag)
	{
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld %zu died\n", get_time_in_ms() - \
			new_philos[n].data->start_time, new_philos[n].f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
	}
}

void	display_and_set_death(t_philo *new_philos, long i)
{
	long	n;
	int		flag;
	long	start_time;

	n = i;
	flag = 0;
	i = -1;
	start_time = get_time_in_ms();

	while(++i < new_philos->data->n_filos)
	{
		if (!flag && (get_time_in_ms() - start_time > 9))
		{
			flag = 1;
			pthread_mutex_lock(&new_philos->data->print_lock);
			printf("%ld %zu died\n", get_time_in_ms() - new_philos[n].data->start_time, new_philos[n].f_id);
			pthread_mutex_unlock(&new_philos->data->print_lock);
		}
		pthread_mutex_lock(&new_philos[i].death_lock);
		new_philos[i].is_dead = true;
		pthread_mutex_unlock(&new_philos[i].death_lock);
	}
	//ft_sleep(400, new_philos);
	usleep(400);
	display_as_last(flag, new_philos, n);
}

void	set_satisfaction_reached(t_philo *new_philos)
{
	long	i;

	i = -1;
	while(++i < new_philos->data->n_filos)
	{
		pthread_mutex_lock(&new_philos[i].death_lock);
		new_philos[i].is_dead = true; //here to focus
		pthread_mutex_unlock(&new_philos[i].death_lock);
	}
}

void monitoring_function(t_philo *new_philos)
{
	long	i;
	long	total_satisfaction;

	while(1)
	{
		i = -1;
		total_satisfaction = 0;
		while(++i < new_philos->data->n_filos)
		{
			pthread_mutex_lock(&new_philos[i].meal_lock);
			if (get_time_in_ms() - new_philos[i].last_meal >= \
				new_philos[i].data->time_to_die)
			{
				pthread_mutex_unlock(&new_philos[i].meal_lock);
				return (display_and_set_death(new_philos, i));
			}
			if (new_philos[i].satisfied)
				total_satisfaction++;
			pthread_mutex_unlock(&new_philos[i].meal_lock);
		}
		if (total_satisfaction == new_philos->data->n_filos)
			return (set_satisfaction_reached(new_philos));
		//ft_sleep(200, new_philos);
		usleep(200);
	}
	return;
}

int main(int argc, char **argv)
{
	t_program 			p_data;
	t_philo				*philosophers;
	t_fork				*forks;

	if (!correct_arguments(argc, argv))
		return (1);
	if (!initialize_data(&p_data, argc, argv))
		return (1);
	philosophers = malloc(sizeof(t_philo) * p_data.n_filos);
	if (!philosophers)
		return (free_and_error(&p_data, NULL, NULL, ERR_MALLOC_FI), 1);
	forks = malloc(sizeof(t_fork) * p_data.n_filos);
	if (!forks)
		return (free_and_error(&p_data, philosophers, NULL, ERR_MALLOC_FO), 1);
	if (!init_forks_and_philos(philosophers, forks, &p_data))
		return (1);
	if (!create_philos_threads(philosophers))
	{
		destroy_free_and_err(philosophers, forks, 4, p_data.n_filos);
		return (1);
	}
	monitoring_function(philosophers);
	join_philos_threads(philosophers);
	return (destroy_free_and_err(philosophers, forks, 5, p_data.n_filos), 0);
}
