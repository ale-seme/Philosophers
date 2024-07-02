#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include "Philo.h"

int correct_arguments(int argc, char **argv)
{

    int i;
    int x;

    i = 0;
    if (argc < 5 || argc > 6)
    {
        printf("incorrect number of arguments\n");
        return (0);
    }
    while(argv[++i])
    {
        x = 0;
        if (!argv[i][x])
        {
            printf("The simulation doesn't accept empty arguments\n");
            return (0);
        }
        else if (argv[i][x] == '0')
        {
            printf("the simulation doesnt accept an argument starting with 0\n");
            return (0);
        }
        while(argv[i][x])
        {
            
            if (!(argv[i][x] >= '0' && argv[i][x++] <= '9'))
            {
                printf("The simulation accepts only unsigned numbers\n");
                return (0);
            }
        }
    }
    return (1);
}

int simple_atoi(char *argv)
{
    long long int number;
    int i = 0;

    while(argv[i++])
    {
        while(argv[i])
        {
            number = number *10 + (argv[i] + '0');
            if (number > LONG_MAX)
            {
                printf("The simulation accepts till LONG_MAX\n");
                return (0);
            }
        }
    }
    return (number);
}
