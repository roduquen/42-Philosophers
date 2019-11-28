/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 22:29:25 by roduquen          #+#    #+#             */
/*   Updated: 2019/11/28 16:00:23 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define ERROR			1
# define SUCCESS		0

# define THINKING		0
# define SLEEPING		1
# define EATING			2
# define DEAD			3

typedef struct timeval	t_time;

typedef struct			s_data
{
	int					nbr_philo;
	long				time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nbr_of_eating;
	long				start_time;
	int					running;
}						t_data;

typedef struct			s_philo
{
	pthread_t			thread;
	pthread_mutex_t		*mutex1;
	pthread_mutex_t		*mutex2;
	pthread_mutex_t		*exit;
	long				time_to_die;
	int					nbr_of_eating;
	int					nbr;
	int					state;
	t_data				*data;
}						t_philo;

int						init(t_data *data, char **av, t_philo **philo
	, pthread_mutex_t **mutex);
void					*philo_table(void *ptr);

#endif
