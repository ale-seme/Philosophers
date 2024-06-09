#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include <time.h>
#include <sys/time.h>

/*memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock*/


// void *routine1(void *arg)
// {
// 	printf("value of x in t1: %d\n", *(int *)arg);
// 	return (NULL);
// }
typedef struct lock_s
{
	pthread_mutex_t m1;
	int				x;
	struct timeval	tv;
}	lock_t;

void *routine2(void *elements)
{
	
	lock_t *elements_new = (lock_t *)(elements);

	pthread_mutex_lock(&elements_new->m1);
	for(int x = 1; x <= 1000000; x++)
	{
		(elements_new->x) += 1;
	}
	printf("vale of x in t2: %d\n", elements_new->x);
	pthread_mutex_unlock(&elements_new->m1);
	//return (pthread_mutex_unlock(&elements_new->m1), NULL);
	gettimeofday(&elements_new->tv, NULL);
	printf("thread is telling me the time %ld\n", (elements_new->tv.tv_sec*1000) + (elements_new->tv.tv_usec/1000));
	return (NULL);
}

int main(int argc, char **argv)
{
	pthread_t t[10];
	
	lock_t elements;
	
	
	int 	i;

	i = 0;
	elements.x = 0;
	(void)argv;
	(void)argc;

	pthread_mutex_init(&elements.m1, NULL);
	while (i < 10)
	{
		if (pthread_create(&t[i], NULL, &routine2, &elements) != 0)
			return (1);
		i++;
	}
	i = 0;
	while(i < 10)
	{
		if (pthread_join(t[i], NULL) != 0)
		return (3);
		i++;
	}
	pthread_mutex_destroy(&elements.m1);
	return (0);
}