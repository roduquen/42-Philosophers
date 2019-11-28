/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/28 12:53:26 by roduquen          #+#    #+#             */
/*   Updated: 2019/11/28 16:00:54 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutex(t_data *data, pthread_mutex_t **mutex)
{
	int		i;

	*mutex = malloc(sizeof(pthread_mutex_t) * (data->nbr_philo + 1));
	if (*mutex == NULL)
		return (ERROR);
	i = 0;
	while (i <= data->nbr_philo)
	{
		if (pthread_mutex_init(&mutex[0][i], NULL))
		{
			while (--i >= 0)
			{
				if (pthread_mutex_destroy(&mutex[0][i]))
					return (ERROR);
			}
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

static int	init_info(t_data *data, char **av)
{
	t_time	time_val;
	char	*checker;

	gettimeofday(&time_val, NULL);
	data->start_time = time_val.tv_sec * 1000000 + time_val.tv_usec;
	data->nbr_philo = strtol(av[1], &checker, 10);
	if (data->nbr_philo <= 0 || checker[0])
		return (ERROR);
	data->time_to_die = strtol(av[2], &checker, 10) * 1000;
	if (data->time_to_die <= 0 || checker[0])
		return (ERROR);
	data->time_to_eat = strtol(av[3], &checker, 10);
	if (data->time_to_eat <= 0 || checker[0])
		return (ERROR);
	data->time_to_sleep = strtol(av[4], &checker, 10);
	if (data->time_to_sleep <= 0 || checker[0])
		return (ERROR);
	data->nbr_of_eating = strtol(av[5], &checker, 10);
	if (data->nbr_of_eating <= 0 || checker[0])
		return (ERROR);
	return (SUCCESS);
}

static int	init_philos(t_data *data, t_philo **philo, pthread_mutex_t **mutex)
{
	int		i;

	*philo = (t_philo*)malloc(sizeof(t_philo) * data->nbr_philo);
	if (*philo == NULL)
		return (ERROR);
	i = 0;
	while (i < data->nbr_philo)
	{
		philo[0][i].nbr = i;
		philo[0][i].time_to_die = data->time_to_die;
		philo[0][i].data = data;
		philo[0][i].nbr_of_eating = 0;
		philo[0][i].state = THINKING;
		philo[0][i].exit = &mutex[0][data->nbr_philo];
		if (i == 0)
			philo[0][i].mutex1 = &mutex[0][data->nbr_philo - 1];
		else
			philo[0][i].mutex1 = &mutex[0][i - 1];
		philo[0][i].mutex2 = &mutex[0][i];
		i++;
	}
	return (SUCCESS);
}

static int	start_to_eat(t_data *data, t_philo **philo)
{
	int		i;
	int		ret;

	i = 0;
	while (i < data->nbr_philo)
	{
		ret = pthread_create(&philo[0][i].thread, NULL, &philo_table
			, &philo[0][i]);
		if (ret)
			return (ERROR);
		ret = pthread_detach(philo[0][i].thread);
		if (ret)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int			init(t_data *data, char **av, t_philo **philo
	, pthread_mutex_t **mutex)
{
	if (init_info(data, av) == ERROR)
		return (ERROR);
	if (init_mutex(data, mutex) == ERROR)
		return (ERROR);
	if (init_philos(data, philo, mutex) == ERROR)
		return (ERROR);
	if (start_to_eat(data, philo) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
