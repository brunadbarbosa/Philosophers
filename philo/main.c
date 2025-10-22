/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:44:37 by brmaria-          #+#    #+#             */
/*   Updated: 2025/10/22 17:37:03 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

int	validate_args(int argc, char **argv)
{
	if (argc != 5 || argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (0);		
	}
	if (!check_character(argv))
	{
		printf("Error: Arguments must be positive numbers\n");
		return (0);
	}
	return (1);
}

long long	ft_atoll(char *s)
{
	int			i = 0;
	int			sign = 1;
	long long	result = 0;

	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' 
		|| s[i] == '\v' || s[i] == '\f' || s[i] == '\r')
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i])
	{
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (result * sign);
}
int main(int argc, char **argv)
{
	//t_rules	*rules;
	
	if (!validate_args(argc, argv))
		return (1);
	printf("Passou no valid0");
	return (0);
}
