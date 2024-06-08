#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct s_program
{
	int				n_filos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_t		*th_array;
	pthread_mutex_t	*forks;
}	t_program;

typedef struct s_fork
{
	pthread_mutex_t lock;
	int				fork_id;
}	t_fork;

typedef struct s_philo
{
	
	size_t			f_id;
	size_t			start_time;
	pthread_t		thread;
	t_fork			fork_right;
	t_fork			fork_left;
	t_program*		data;
	//t_program 		*program;	

}	t_philo;


int main(int argc, char **argv)
{
	t_program 			p_data = {};
	t_philo				philosophers[3];
	pthread_t			th_array[3];
	// t_fork				forks[3];

	p_data.n_filos = atoi(argv[1]);
	p_data.time_to_die = atoi(argv[2]);
	p_data.time_to_eat = atoi(argv[3]);
	p_data.time_to_sleep = atoi(argv[4]);
	p_data.th_array = th_array;
	// p_data.forks = forks;

	printf("this is number of philos %d\n", p_data.n_filos);
	printf("this is number of philos %d\n", p_data.time_to_eat);
	printf("this is number of philos %d\n", p_data.time_to_die);
	printf("this is number of philos %d\n", p_data.time_to_sleep);


	int i = 1;
	while(i < 4)
	{
		philosophers[i].f_id = i;
		philosophers[i].fork_left.fork_id = i;
		philosophers[i].thread = p_data.th_array[i];
		// philosophers[i]
		printf("this is philo n: %zu\n and this is left fork id \
			%d\n", philosophers[i].f_id, philosophers[i].fork_left.fork_id);
		i++;
	}

}