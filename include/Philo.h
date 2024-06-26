#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

typedef struct s_program
{
	int				n_filos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_needed;
	long int		start_time;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	bool			someone_died;
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
	pthread_mutex_t meal_lock;
	bool			satisfied;
	int				meals_eaten;
}	t_philo;

void		initialize_data(t_program *p_data, int argc, char **argv);
void		init_forks_and_philos(t_philo *philos, t_fork *forks, t_program * p_data);
void		*routine(void *philos);
void		create_philos_threads(t_philo *philosophers);
void    	join_philos_threads(t_philo *philosophers);
long int	get_time_in_ms();
void		ft_sleep(long int sleep_time_in_ms);
int			death_check(t_philo *new_philos);
void		free_and_destroy(t_program *p_data, t_philo *philos, t_fork *forks);

/*philosophers actions*/
int	grabbing_forks_even(t_philo *new_philos);
int	grabbing_forks_odd(t_philo *new_philos);
int	action_eating(t_philo *new_philos);
int	action_sleeping(t_philo *new_philos);
int	action_thinking(t_philo *new_philos);




