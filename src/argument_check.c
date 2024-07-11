/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   argument_check.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: asemerar <asemerar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 11:26:18 by asemerar      #+#    #+#                 */
/*   Updated: 2024/07/11 15:24:51 by asemerar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

int	correct_arguments(int argc, char **argv)
{
	int	i;
	int	x;

	i = 0;
	if (argc < 5 || argc > 6)
		return (display_error(ERR_N_ARGS), 0);
	while (argv[++i])
	{
		x = 0;
		if (!argv[i][x])
			return (display_error(ERR_EMPTY_ARGS), 0);
		else if (argv[i][x] == '0')
			return (display_error(ERR_0_IN_ARG), 0);
		while (argv[i][x])
		{
			if (!(argv[i][x] >= '0' && argv[i][x++] <= '9'))
				return (display_error(ERR_ONLY_UNUMBERS), 0);
		}
	}
	return (1);
}

long	simple_atoi(char *argv)
{
	long	number;
	int		i;

	number = 0;
	i = 0;
	while (argv[i])
	{
		if (number > (LONG_MAX - (argv[i] - '0')) / 10)
			return (0);
		number = number * 10 + (argv[i] - '0');
		i++;
	}
	return (number);
}
