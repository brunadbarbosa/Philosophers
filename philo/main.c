/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:44:37 by brmaria-          #+#    #+#             */
/*   Updated: 2025/10/29 17:18:25 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_in_ms(void)
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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
				printf("Philosopher %d died\n", philos[i].id);
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
	return (1);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;
	int	left;
	int	right;

	philo = (t_philosopher *)arg;
	left = philo->id - 1;
	right = (philo->id) % philo->rules->num_philos;


	while (1)
	{
		pthread_mutex_lock(&philo->rules->death_lock);
		if(philo->rules->someone_died)
		{
			pthread_mutex_unlock(&philo->rules->death_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->death_lock);
		printf("Philosopher %d is thinking\n", philo->id);
		// usleep(200);

		pthread_mutex_lock(&philo->rules->forks[left]);
		printf("Philosopher %d picked up left fork \n", philo->id);
		pthread_mutex_lock(&philo->rules->forks[right]);
		printf("Philosopher %d picked up right fork \n", philo->id);

		printf("%lld Philosopher %d is eating!\n", get_time_in_ms() - philo->rules->start, philo->id);
		ft_usleep(philo->rules->time_to_eat);

		pthread_mutex_unlock(&philo->rules->forks[left]);
		pthread_mutex_unlock(&philo->rules->forks[right]);
		printf("Philosopher %d released forks!\n", philo->id);

		printf("Philosopher %d is sleeping!\n", philo->id);
		ft_usleep(philo->rules->time_to_sleep);
	}
	return (NULL);
}

void	ft_philosophers(t_philosopher *philos, t_rules *rules)
{
	pthread_t	*threads;
	// pthread_t	monitoring;
	int	i;

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
	// if (pthread_create(&monitoring, NULL, monitor_routine, philos) == 0)
	// 	pthread_join(monitoring, NULL);
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
	else
		rules->meals_limit = -1;
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
