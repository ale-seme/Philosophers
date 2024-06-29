#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define ERR_MUTEX_D "Error in mutex init in Data\n"
#define ERR_MUTEX_FI "Error in mutex init in philos\n"
#define ERR_MUTEX_FO "Error in mutex init in forks\n"



typedef struct s_program
{
	int				n_filos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_needed;
	long int		start_time;
	//pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	start_lock;
	pthread_mutex_t	start_monitoring;
	bool			synchronized;
	//bool			someone_died;
	struct s_philo 		*all_philos;
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
	long int		last_meal;
	pthread_t		thread;
	t_fork			*fork_right;
	t_fork			*fork_left;
	t_program*		data;
	bool			is_dead;
	bool			someone_died;
	pthread_mutex_t meal_lock;
	pthread_mutex_t	death_lock;
	bool			satisfied;
	int				meals_eaten;
}	t_philo;

int		initialize_data(t_program *p_data, int argc, char **argv);
void		init_forks_and_philos(t_philo *philos, t_fork *forks, t_program * p_data);
void		*routine(void *philos);
void		create_philos_threads(t_philo *philosophers);
void    	join_philos_threads(t_philo *philosophers);
long int	get_time_in_ms();
void		ft_sleep(long int sleep_time_in_ms, t_philo *new_philos);
int			death_check(t_philo *self_philo);
void		free_and_destroy(t_program *p_data, t_philo *philos, t_fork *forks);

/*philosophers actions*/
int	grabbing_forks_even(t_philo *new_philos);
int	grabbing_forks_odd(t_philo *new_philos);
int	action_eating(t_philo *new_philos);
int	action_sleeping(t_philo *new_philos);
int	action_thinking(t_philo *new_philos);

/*cleaning functions*/

int	free_data_and_err(t_program *p_data, const char *error, int index);
