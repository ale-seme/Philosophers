/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:26:27 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/09 12:06:39 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

/*Error messages*/

#define ERR_N_ARGS "Incorrect number of arguments\n"
#define ERR_EMPTY_ARGS "The simulation doesn't accept empty args\n"
#define ERR_0_IN_ARG "The simulation doesnt accept an args starting with 0\n"
#define ERR_ONLY_UNUMBERS "The simulation accepts only unsigned numbers\n"
#define ERR_LONG_OVERFLOW "The simulation accepts numbers >= 1 && <= LONG_MAX\n"
#define ERR_MUTEX_D "Error in mutex init in Data\n"
#define ERR_MALLOC_FI "Error in malloc philosophers\n"
#define ERR_MALLOC_FO "Error in malloc forks\n"
#define ERR_MUTEX_INIT "Error during the init_forks_and_philos\n"
#define ERR_THREAD_F "Error during the creation of a philo thread\n"

/*Structs of the program*/

typedef struct s_program
{
	long			n_filos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meals_needed;
	long			start_time;
	bool			synchronized;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	start_lock;
	struct s_philo		*all_philos;
	struct s_fork		*all_forks;
}	t_program;

typedef struct s_fork
{
	pthread_mutex_t lock;
}	t_fork;

typedef struct s_philo
{
	
	size_t			f_id;
	struct timeval	tv;
	long			last_meal;
	pthread_t		thread;
	t_fork			*fork_right;
	t_fork			*fork_left;
	t_program*		data;
	bool			satisfied;
	bool			is_dead;
	bool			someone_died;
	long			meals_eaten;
	pthread_mutex_t meal_lock;
	pthread_mutex_t	death_lock;
}	t_philo;

/*argument checking*/
int		correct_arguments(int argc, char **argv);
long	simple_atoi(char *argv);

/*Data initialization*/

int		initialize_data(t_program *p_data, int argc, char **argv);
int		init_forks_and_philos(t_philo *philos, t_fork *forks, t_program * p_data);

/*Create and join threads*/

int		create_philos_threads(t_philo *philosophers);
void	join_philos_threads(t_philo *philosophers);

/*Monitoring and time management*/
void	*routine(void *philos);
long	get_time_in_ms();
void	ft_sleep(long int sleep_time_in_ms, t_philo *new_philos);
int		death_check(t_philo *self_philo);

/*philosophers actions*/
int		grabbing_forks_even(t_philo *new_philos);
int		grabbing_forks_odd(t_philo *new_philos);
int		action_eating(t_philo *new_philos);
int		action_sleeping(t_philo *new_philos);
int		action_thinking(t_philo *new_philos);

/*cleaning functions*/

void	free_data_and_err(t_program *p_data, const char *error, int index);
void	free_and_error(t_program *p_data, t_philo *philos, t_fork *forks, char *err);
void	display_error(char *error);
void	destroy_free_and_err(t_philo *philos, t_fork *forks, long n, long stop);
void	detach_and_error(t_philo *philosophers, long stop, char *err);