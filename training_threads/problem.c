#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/*memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock*/


typedef struct s_elements
{
	int *array;
	int a_index;
	pthread_mutex_t m1;
}	t_elements;

void *routine(void *elements)
{
	t_elements *new_elements = (t_elements *)(elements);
	pthread_mutex_lock(&new_elements->m1);
	printf("%d\n", new_elements->array[new_elements->a_index]);
	new_elements->a_index += 1;
	pthread_mutex_unlock(&new_elements->m1);
	return NULL;
}

int main(void)
{
	pthread_t t[10];
	t_elements elements;

	int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
	elements.array = primes;
	elements.a_index = 0;
	pthread_mutex_init(&elements.m1, NULL);
	for(int i = 0; i < 10; i++)
		pthread_create(&t[i], NULL, &routine, &elements);
	for(int i = 0; i < 10; i++)
		pthread_join(t[i], NULL);
	pthread_mutex_destroy(&elements.m1);
}