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

void *routine(void *data)
{
	t_program *new_data;
	
	new_data = (t_program *)(data);
	if ()

}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("no arguments given\n");
		return (1);
	}
	t_program 			p_data;
	t_philo				philosophers[3];
	pthread_t			th_array[3];

	p_data.n_filos = atoi(argv[1]);
	p_data.time_to_die = atoi(argv[2]);
	p_data.time_to_eat = atoi(argv[3]);
	p_data.time_to_sleep = atoi(argv[4]);
	//p_data.th_array = th_array;
	// p_data.forks = forks;

	int i = 0;
	while(i < 3)
	{
		philosophers[i].f_id = i + 1;
		philosophers[i].fork_left.fork_id = i + 1;
		philosophers[i].fork_right.fork_id = philosophers[i].fork_left.fork_id -1;
		if (philosophers[i].fork_right.fork_id == 0)
			philosophers[i].fork_right.fork_id = p_data.n_filos;

		pthread_mutex_init(&philosophers[i].fork_left.lock, NULL);
		pthread_mutex_init(&philosophers[i].fork_right.lock, NULL);
		pthread_create(&philosophers[i].thread, NULL, &routine, &p_data);
		printf("philo n: %zu left fork id:%d and right_fork id: %d\n"\
			, philosophers[i].f_id, philosophers[i].fork_left.fork_id, philosophers[i].fork_right.fork_id);
		i++;
	}

}