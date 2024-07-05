#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "Philo.h"

//strace -f -e trace=futex -tt -T -o strace_output ./philo

void	display_and_set_death(t_philo *new_philos, long i)
{
	//pthread_mutex_lock(&new_philos->data->print_lock);
	//pthread_mutex_unlock(&new_philos->data->print_lock);
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
	usleep(400);
	if (!flag)
	{
		pthread_mutex_lock(&new_philos->data->print_lock);
		printf("%ld %zu died\n", get_time_in_ms() - new_philos[n].data->start_time, new_philos[n].f_id);
		pthread_mutex_unlock(&new_philos->data->print_lock);
	}
	//new_philos[i].someone_died = true;
	//pthread_mutex_unlock(&new_philos[i].meal_lock);
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

void *monitoring_routine(void *philos)
{
	t_philo *new_philos;
	long	i;
	long	total_satisfaction;

	new_philos = (t_philo *)(philos);
	//ft_sleep(1, new_philos);
	// while(!new_philos->data->synchronized)
	// 	continue;
	// pthread_mutex_lock(&new_philos->data->start_monitoring);
	// pthread_mutex_unlock(&new_philos->data->start_monitoring);
	pthread_mutex_lock(&new_philos->data->start_lock);
	pthread_mutex_unlock(&new_philos->data->start_lock);
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
				//printf("INFO %ld\n", new_philos[i].last_meal);
				return (display_and_set_death(new_philos, i), NULL);
			}
			if (new_philos[i].satisfied)
				total_satisfaction++;
			pthread_mutex_unlock(&new_philos[i].meal_lock);
			i++;
		}
		if (total_satisfaction == new_philos->data->n_filos)
			return (set_satisfaction_reached(new_philos), NULL);
		ft_sleep(200, new_philos);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	t_program 			p_data;
	t_philo				*philosophers;
	t_fork				*forks;
	pthread_t			monitor;
	long				i;

	if (!correct_arguments(argc, argv))
		return (1);
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
