#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

/*memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock*/

int x = 2;

void *routine1()
{
	printf("value of x in t1: %d\n", x);
	return (NULL);
}

void *routine2()
{
	sleep(2);
	x++;
	printf("vale of x in t2: %d\n", x);
	return (NULL);
}

int main(int argc, char **argv)
{
	pthread_t t1, t2;
	(void)argv;
	(void)argc;


	if (pthread_create(&t1, NULL, &routine1, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, &routine2, NULL) != 0)
		return (1);
	if (pthread_join(t1, NULL) != 0)
		return (2);
	if (pthread_join(t2, NULL) != 0)
		return (2);
	return (0);
}