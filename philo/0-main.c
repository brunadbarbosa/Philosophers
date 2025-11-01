/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0-main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:44:37 by brmaria-          #+#    #+#             */
/*   Updated: 2025/11/01 17:53:41 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_in_ms(void)
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_philosopher *philo, const char *action, t_rules *rules)
{
	pthread_mutex_lock(&rules->death_lock);
	if (!rules->someone_died)
	{
		pthread_mutex_lock(&philo->rules->print_lock);
		printf("%lld %d %s\n", get_time_in_ms() - rules->start, philo->id, action);
		pthread_mutex_unlock(&philo->rules->print_lock);
	}
	pthread_mutex_unlock(&rules->death_lock);
}

void	grab_forks(t_philosopher *philo, t_rules *rules)
{
	int	left;
	int	right;
	
	left = philo->id - 1;
	right = (philo->id % rules->num_philos);
	
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&rules->forks[right]);
		print_action(philo, "has taken a fork", rules);
		pthread_mutex_lock(&rules->forks[left]);
		print_action(philo, "has taken a fork", rules);
	}
	else
	{
		pthread_mutex_lock(&rules->forks[left]);
		print_action(philo, "has taken a fork", rules);
		pthread_mutex_lock(&rules->forks[right]);
		print_action(philo, "has taken a fork", rules);
	}
	pthread_mutex_lock(&philo->eat_lock);
	philo->meals_eaten++;
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->eat_lock);
	print_action(philo, "is eating", rules);
}

int	check_death(t_philosopher *philo, t_rules *rules)
{
	pthread_mutex_lock(&philo->rules->death_lock);
	if(rules->someone_died)
	{
		pthread_mutex_unlock(&philo->rules->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->death_lock);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philosopher	*philos;
	t_rules			*rules;
	int				i;

	philos = (t_philosopher *)arg;
	rules = philos[0].rules;
	
	while (1)
	{
		i = 0;
		while (i < rules->num_philos)
		{
			pthread_mutex_lock(&rules->death_lock);
			if(rules->someone_died)
			{
				pthread_mutex_unlock(&rules->death_lock);
				return (NULL);
			}
			if (get_time_in_ms() - philos[i].last_meal > rules->time_to_die)
			{
				rules->someone_died = 1;
				printf("%lld %d died!\n", get_time_in_ms() - philos[i].rules->start, philos[i].id);
				pthread_mutex_unlock(&rules->death_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&rules->death_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	ft_usleep(long long time_in_ms)
{
	long long start;

	start = get_time_in_ms();
	while (get_time_in_ms() - start < time_in_ms)
		usleep(100);
}

void	destroy_mutexes(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philos)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	free(rules->forks);
}

int	init_mutexes(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos);
	if (!rules->forks)
		return (0);
	i = 0;
	while (i < rules->num_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
		{
			printf("Error: Failed to init mutex %d\n", i);
			return (0);
		}
		i++;
	}
	pthread_mutex_init(&rules->print_lock, NULL);
	pthread_mutex_init(&rules->death_lock, NULL);
	return (1);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;
	int				left;
	int				right;

	philo = (t_philosopher *)arg;
	left = philo->id - 1;
	right = (philo->id) % philo->rules->num_philos;

	while (1)
	{
		if(check_death(philo, philo->rules))
			return (NULL);
		print_action(philo, "is thinking", philo->rules);
		usleep(200);

		grab_forks(philo, philo->rules);

		pthread_mutex_unlock(&philo->rules->forks[left]);
		pthread_mutex_unlock(&philo->rules->forks[right]);

		print_action(philo, "is sleeping", philo->rules);
		ft_usleep(philo->rules->time_to_sleep);
	}
	return (NULL);
}

void	ft_philosophers(t_philosopher *philos, t_rules *rules)
{
	pthread_t	*threads;
	pthread_t	monitoring;
	int			i;

	threads = malloc(sizeof(pthread_t) * rules->num_philos);
	if (!threads)
		return ;
	i = 0;
	while (i < rules->num_philos)
	{
		if (pthread_create(&threads[i], NULL, routine, &philos[i]) != 0)
		{
			pthread_mutex_lock(&rules->death_lock);
			rules->someone_died = 1;
			pthread_mutex_unlock(&rules->death_lock);
		}
		i++;
	}
	if (pthread_create(&monitoring, NULL, monitor_routine, philos) == 0)
		pthread_join(monitoring, NULL);
	while (--i >= 0)
		pthread_join(threads[i], NULL);
	free(threads);
}

t_philosopher	*ft_init_philos(t_philosopher *philos,  t_rules *rules)
{
	int	i;
	
	philos = malloc(sizeof(t_philosopher) * rules->num_philos);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < rules->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = get_time_in_ms();
		philos[i].rules = rules;
		i++;
	}
	return (philos);
}

void	set_args(t_rules *rules, char **argv)
{
	rules->num_philos = ft_atoll(argv[1]);
	rules->time_to_die = ft_atoll(argv[2]);
	rules->time_to_eat = ft_atoll(argv[3]);
	rules->time_to_sleep = ft_atoll(argv[4]);
	rules->start = get_time_in_ms();
	if (argv[5])
		rules->meals_limit = ft_atoll(argv[5]);
	rules->someone_died = 0;
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' 
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_character(char **str)
{
	int	i;
	int	k;

	i = 1;
	while (str[i])
	{
		k = 0;
		while (str[i][k])
		{
			if (!ft_isdigit(str[i][k]))
			 	return (0);
			k++;
		}
		i++;
	}
	return (1);
}

int	validate_args(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
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
		if (!ft_isdigit(s[i]))
			return (-1);
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (result * sign);
}
int main(int argc, char **argv)
{
	t_rules			*rules;
	t_philosopher	*philos;
	
	philos = NULL;
	if (!validate_args(argc, argv))
		return (1);
	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (1);
	set_args(rules, argv);
	if (!init_mutexes(rules))
		return (1);
	philos = ft_init_philos(philos, rules);
	if (philos)
	{
		ft_philosophers(philos, rules);
		free(philos);
	}
	destroy_mutexes(rules);
	free(rules);
	return (0);
}
