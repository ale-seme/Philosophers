#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"


void	display_and_set_death(t_philo *new_philos, int i)
{
	pthread_mutex_lock(&new_philos->data->print_lock);
	printf("%ld philo n: %zu has DIED\n", get_time_in_ms() - new_philos[i].data->start_time, new_philos[i].f_id);
	pthread_mutex_unlock(&new_philos->data->print_lock);
	i = -1;
	while(++i < new_philos->data->n_filos)
	{
		pthread_mutex_lock(&new_philos[i].death_lock);
		new_philos[i].is_dead = true;
		pthread_mutex_unlock(&new_philos[i].death_lock);
	}
	//new_philos[i].someone_died = true;
	//pthread_mutex_unlock(&new_philos[i].meal_lock);
}

void	set_satisfaction_reached(t_philo *new_philos)
{
	int	i;

	i = -1;
	while(++i < new_philos->data->n_filos)
	{
		pthread_mutex_lock(&new_philos[i].death_lock);
		new_philos[i].is_dead = true; //here to focus
		pthread_mutex_unlock(&new_philos->death_lock);
	}
}

void *monitoring_routine(void *philos)
{
	t_philo *new_philos;
	int i;
	int	total_satisfaction;

	new_philos = (t_philo *)(philos);
	// while(!new_philos->data->synchronized)
	// 	continue;
	pthread_mutex_lock(&new_philos->data->start_monitoring);
	pthread_mutex_unlock(&new_philos->data->start_monitoring);
	while(1)
	{
		i = 0;
		total_satisfaction = 0;
		while(i < new_philos->data->n_filos)
		{
			pthread_mutex_lock(&new_philos[i].meal_lock);
			//printf("philo last meal %ld\n", new_philos[i].last_meal);
			if (get_time_in_ms() - new_philos[i].last_meal >= new_philos[i].data->time_to_die)
			{
				pthread_mutex_unlock(&new_philos[i].meal_lock);
				printf("INFO %ld\n", new_philos[i].last_meal);
				return (display_and_set_death(new_philos, i), NULL);
			}
			if (new_philos[i].satisfied)
				total_satisfaction++;
			pthread_mutex_unlock(&new_philos[i].meal_lock);
			i++;
		}
		if (total_satisfaction == new_philos->data->n_filos)
			return (set_satisfaction_reached(new_philos), NULL);
		usleep(200);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	t_program 			p_data;
	t_philo				*philosophers;
	t_fork				*forks;
	pthread_t			monitor;
	int					i;

	if (argc < 5 || argc > 6)
		return (printf("not right amout of arguments given\n"), 1);
	if (!initialize_data(&p_data, argc, argv))
		return (1);
	philosophers = malloc(sizeof(t_philo) * p_data.n_filos);
	if (!philosophers)
		return (free_and_destroy(&p_data, NULL, NULL), 1);
	forks = malloc(sizeof(t_fork) * p_data.n_filos);
	if (!forks)
		return (free_and_destroy(&p_data, philosophers, NULL), 1);
	// p_data.all_philos = philosophers;//considering if good
	// p_data.all_forks = forks; //considering if important
	init_forks_and_philos(philosophers, forks, &p_data);
	create_philos_threads(philosophers);
	//pthread_mutex_lock(&p_data.start_monitoring);
	pthread_create(&monitor, NULL, &monitoring_routine, philosophers);
	//pthread_mutex_unlock(&p_data.start_monitoring);
	pthread_join(monitor, NULL);
	join_philos_threads(philosophers);
	return (free_and_destroy(&p_data, philosophers, forks), 0);
}
