/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo-one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:26:33 by roduquen          #+#    #+#             */
/*   Updated: 2019/11/28 01:46:35 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline int	init(t_data *data, char **av, t_philo **philo
	, pthread_mutex_t **mutex)
{
	t_time	time_val;

	gettimeofday(&time_val, NULL);
	data->start_time = time_val.tv_sec * 1000000 + time_val.tv_usec;
	data->nbr_philo = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	data->nbr_of_eating = atoi(av[5]);
	data->running = 1;
	*philo = (t_philo*)malloc(sizeof(t_philo) * data->nbr_philo);
	memset(*philo, 0, sizeof(t_philo) * data->nbr_philo);
	*mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	return (0);
}

void				*philo_table(void *ptr)
{
	t_philo		*philo;
	t_time		time_val;
	int			time;

	philo = (t_philo*)ptr;
	while (philo->state != DEAD)
	{
		if (philo->state == EATING)
		{
			usleep(philo->data->time_to_eat * 1000);
			pthread_mutex_unlock(philo->mutex1);
			pthread_mutex_unlock(philo->mutex2);
			philo->time_to_die = time;
			philo->state = SLEEPING;
			gettimeofday(&time_val, NULL);
			time = time_val.tv_sec * 1000000 + time_val.tv_usec - philo->data->start_time;
			printf("%d %d is sleeping\n", time / 1000 - (time / 1000) % 100, philo->nbr);
		}
		else if (philo->state == SLEEPING)
		{
			usleep(philo->data->time_to_sleep * 1000);
			gettimeofday(&time_val, NULL);
			time = time_val.tv_sec * 1000000 + time_val.tv_usec - philo->data->start_time;
			printf("%d %d is thinking\n", time / 1000 - (time / 1000) % 100, philo->nbr);
			philo->state = THINKING;
		}
		else if (philo->state == THINKING)
		{
			gettimeofday(&time_val, NULL);
			time = time_val.tv_sec * 1000000 + time_val.tv_usec - philo->data->start_time;
		//	usleep(time / 1000 - philo->time_to_die * 1000 - philo->data->time_to_eat * 1000);
			pthread_mutex_lock(philo->mutex1);
			pthread_mutex_lock(philo->mutex2);
			gettimeofday(&time_val, NULL);
			time = time_val.tv_sec * 1000000 + time_val.tv_usec - philo->data->start_time;
			if (time - philo->time_to_die < philo->data->time_to_die)
			{
				printf("%d %d is eating\n", time / 1000 - (time / 1000) % 100, philo->nbr);
				philo->state = EATING;
			}
			else
			{
				printf("%d %d is dead\n", time / 1000 - (time / 1000) % 100, philo->nbr);
				philo->state = DEAD;
				philo->data->running = 0;
				pthread_mutex_unlock(philo->mutex1);
				pthread_mutex_unlock(philo->mutex2);
			}
		}
	}
	pthread_exit(0);
}

int					main(int ac, char **av)
{
	t_data			data;
	t_philo			*philo;
	pthread_mutex_t	*mutex;

	init(&data, av, &philo, &mutex);
	ac = 0;
	while (ac < data.nbr_philo)
		pthread_mutex_init(&mutex[ac++], NULL);
	ac = 0;
	while (ac < data.nbr_philo)
	{
		philo[ac].nbr = ac;
		philo[ac].time_to_die = data.start_time;
		philo[ac].data = &data;
		if (ac == 0)
			philo[ac].mutex1 = &mutex[data.nbr_philo - 1];
		else
			philo[ac].mutex1 = &mutex[ac - 1];
		philo[ac].mutex2 = &mutex[ac];
		pthread_create(&philo[ac].thread, NULL, &philo_table, &philo[ac]);
		ac++;
	}
	ac = 0;
	while (1)
	{
		if (!data.running)
			break ;
	}
	return (0);
}
