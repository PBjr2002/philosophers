/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:56:21 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/03 20:32:38 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_ms(void)
{
	struct timeval	time;
	long			true_time;

	gettimeofday(&time, NULL);
	true_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (true_time);
}

long	ft_latoi(char *str)
{
	long	number;
	int		n;
	int		signal;

	n = 0;
	signal = 1;
	number = 0;
	while (str[n] == 32 || (str[n] >= 9 && str[n] <= 13))
		n++;
	if (str[n] == '-' || str[n] == '+')
	{
		if (str[n] == '-')
			signal = -1;
		n++;
	}
	while (str[n] >= '0' && str[n] <= '9')
	{
		number = number * 10 + (str[n] - 48);
		n++;
	}
	return (number * signal);
}

void	print_msg(t_philo *philo, char *msg)
{
	long	time;

	pthread_mutex_lock(&philo->table->write);
	time = get_ms() - philo->table->start;
	pthread_mutex_lock(&philo->table->lock);
	if (philo->table->go_exit == 1)
	{
		pthread_mutex_unlock(&philo->table->lock);
		pthread_mutex_unlock(&philo->table->write);
		return ;
	}
	pthread_mutex_unlock(&philo->table->lock);
	printf("%ld %i %s\n", time, philo->seat + 1, msg);
	pthread_mutex_unlock(&philo->table->write);
}

void	upgraded_sleep(t_table *table, long timer)
{
	while (get_ms() < timer)
	{
		pthread_mutex_lock(&table->lock);
		if (table->go_exit == 1)
		{
			pthread_mutex_unlock(&table->lock);
			pthread_mutex_unlock(&table->philo->philo_lock);
			return ;
		}
		pthread_mutex_unlock(&table->lock);
		usleep(250);
	}
}
