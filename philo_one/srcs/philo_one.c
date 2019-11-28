/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo-one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:26:33 by roduquen          #+#    #+#             */
/*   Updated: 2019/11/28 15:57:14 by roduquen         ###   ########.fr       */
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

static int	check_each_philos(t_philo *philo, t_data *data
	, pthread_mutex_t *mutex)
{
	int		i;
	long	change;

	i = 0;
	change = time_elapsed(data->start_time);
	while (i < data->nbr_philo)
	{
		if (philo[i].state != EATING)
			philo[i].time_to_die -= change;
		else
			philo[i].time_to_die = data->time_to_die;
		if (philo[i].time_to_die <= 0)
		{
			pthread_mutex_lock(&mutex[data->nbr_philo]);
			printf("%ld %d died\n", (long)500 / 1000, i + 1);
			pthread_mutex_destroy(&mutex[0]);
			exit(0);
		}
		i++;
	}
	return (SUCCESS);
}

static void	check_if_alive(t_philo *philo, pthread_mutex_t *mutex, t_data *data)
{
	while (1)
		check_each_philos(philo, data, mutex);
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
	check_if_alive(philo, mutex, &data);
	return (0);
}
