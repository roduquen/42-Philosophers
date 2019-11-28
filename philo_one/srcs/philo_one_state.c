/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/28 09:37:04 by roduquen          #+#    #+#             */
/*   Updated: 2019/11/28 16:03:22 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	state_eating(t_philo *philo)
{
	t_time		time_val;
	long		time;

	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->mutex1);
	pthread_mutex_unlock(philo->mutex2);
	++philo->nbr_of_eating;
	philo->state = SLEEPING;
	gettimeofday(&time_val, NULL);
	time = time_val.tv_sec * 1000000 + time_val.tv_usec
		- philo->data->start_time;
	pthread_mutex_lock(philo->exit);
	pthread_mutex_unlock(philo->exit);
	printf("%ld %d is sleeping\n", time / 1000, philo->nbr + 1);
}

static void	state_sleeping(t_philo *philo)
{
	t_time		time_val;
	long		time;

	usleep(philo->data->time_to_sleep * 1000);
	gettimeofday(&time_val, NULL);
	time = time_val.tv_sec * 1000000 + time_val.tv_usec
		- philo->data->start_time;
	pthread_mutex_lock(philo->exit);
	pthread_mutex_unlock(philo->exit);
	printf("%ld %d is thinking\n", time / 1000, philo->nbr + 1);
	philo->state = THINKING;
}

static void	state_thinking(t_philo *philo)
{
	t_time		time_val;
	long		time;

	pthread_mutex_lock(philo->mutex1);
	pthread_mutex_lock(philo->mutex2);
	gettimeofday(&time_val, NULL);
	time = time_val.tv_sec * 1000000 + time_val.tv_usec
		- philo->data->start_time;
	pthread_mutex_lock(philo->exit);
	pthread_mutex_unlock(philo->exit);
	printf("%ld %d is eating\n", time / 1000, philo->nbr + 1);
	philo->state = EATING;
}

void		*philo_table(void *ptr)
{
	t_philo		*philo;

	philo = (t_philo*)ptr;
	while (1)
	{
		if (philo->state == EATING)
			state_eating(philo);
		else if (philo->state == SLEEPING)
			state_sleeping(philo);
		else if (philo->state == THINKING)
			state_thinking(philo);
	}
	pthread_exit(0);
}
