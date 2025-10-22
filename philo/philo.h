/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:28:32 by brmaria-          #+#    #+#             */
/*   Updated: 2025/10/22 17:48:16 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

typedef struct s_rules
{
	int			num_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			meals_limit;
}	t_rules;


int	validate_args(int argc, char **argv);
long long	ft_atoll(char *s);

#endif