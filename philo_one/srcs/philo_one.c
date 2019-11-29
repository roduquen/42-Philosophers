/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:26:33 by roduquen          #+#    #+#             */
/*   Updated: 2019/11/29 09:02:10 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	time_elapsed(long start_time)
{
	t_time		val;
	static long	prev = 0;
	long		time;
	long		change;

	gettimeofday(&val, NULL);
	if (prev == 0)
		prev = val.tv_sec * 1000000 + val.tv_usec - start_time;
	time = val.tv_sec * 1000000 + val.tv_usec - start_time;
	change = time - prev;
	prev = time;
	return (change);
}

static int	leave_philos(t_philo *philo, t_data *data
		, pthread_mutex_t *mutex, long total)
{
	int		i;

	pthread_mutex_lock(&mutex[data->nbr_philo]);
	printf("%ld %d died\n", total, i + 1);
	i = 0;
	while (i <= data->nbr_philo)
	{
		pthread_mutex_destroy(&mutex[i]);
		i++;
	}
	free(mutex);
	free(philo);
	return (SUCCESS);
}

static int	check_each_philos(t_philo *philo, t_data *data
		, pthread_mutex_t *mutex)
{
	int		i;
	long	change;
	long	total;

	total = 0;
	while (1)
	{
		i = 0;
		change = time_elapsed(data->start_time);
		total += change;
		while (i < data->nbr_philo)
		{
			if (philo[i].state != EATING)
			{
				philo[i].time_to_die -= change;
				if (philo[i].time_to_die <= 0)
					return (leave_philos(philo, data, mutex, total / 1000));
			}
			else
				philo[i].time_to_die = data->time_to_die;
			i++;
		}
	}
	return (SUCCESS);
}

int			main(int ac, char **av)
{
	t_data			data;
	t_philo			*philo;
	pthread_mutex_t	*mutex;

	if (ac != 6)
		return (ERROR);
	if (init(&data, av, &philo, &mutex) == ERROR)
		return (ERROR);
	return (check_each_philos(philo, &data, mutex));
}
