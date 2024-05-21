/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:56:21 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/21 16:01:56 by pauberna         ###   ########.fr       */
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
	if ((str[n] < '0' || str[n] > '9') && str[n] != '\0')
		return (-1);
	return (number * signal);
}

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->write);
	pthread_mutex_lock(&philo->philo_lock);
	if (philo->go_exit == 1)
	{
		pthread_mutex_unlock(&philo->table->write);
		pthread_mutex_unlock(&philo->philo_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->philo_lock);
	printf("%ld %i %s\n", get_ms() - philo->table->start,
		philo->seat + 1, msg);
	pthread_mutex_unlock(&philo->table->write);
}

void	upgraded_sleep(t_philo *philo, long timer)
{
	while (get_ms() < timer)
	{
		pthread_mutex_lock(&philo->philo_lock);
		if (philo->go_exit == 1)
		{
			pthread_mutex_unlock(&philo->philo_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->philo_lock);
		usleep(250);
	}
}
