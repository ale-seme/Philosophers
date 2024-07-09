/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   argument_check.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:26:18 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/09 11:49:49 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
        return(printf(ERR_N_ARGS), 0);
    while(argv[++i])
    {
        x = 0;
        if (!argv[i][x])
            return(printf(ERR_EMPTY_ARGS), 0);
        else if (argv[i][x] == '0')
            return(printf(ERR_0_IN_ARG), 0);
        while(argv[i][x])
        {
            if (!(argv[i][x] >= '0' && argv[i][x++] <= '9'))
                return(printf(ERR_ONLY_UNUMBERS), 0);
        }
    }
    return (1);
}

long    simple_atoi(char *argv)
{
    long number;

    number = 0;
    int i = 0;

    while(argv[i])
    {
        
        if (number > (LONG_MAX - (argv[i] - '0')) / 10)
        {
            printf(ERR_LONG_OVERFLOW);
            return (0);
        }
        number = number *10 + (argv[i] - '0');
        i++;
    }
    return (number);
}
