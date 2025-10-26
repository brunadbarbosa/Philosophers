/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:28:32 by brmaria-          #+#    #+#             */
/*   Updated: 2025/10/26 12:34:34 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_rules
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				meals_limit;
	int				someone_died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_lock;
}	t_rules;

typedef struct s_philosopher
{
	int				id;
	long long		last_meal;
	int				meals_eaten;
	t_rules		*rules;
} t_philosopher;

int	validate_args(int argc, char **argv);
long long	ft_atoll(char *s);

#endif