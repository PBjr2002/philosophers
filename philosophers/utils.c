/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:42:22 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/19 16:41:35 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_msg(t_philo *philo, char *msg)
{
	long	time;
	
	pthread_mutex_lock(&philo->info->lock);
	time = get_time(philo->info);
	printf("%ld %i %s\n", time, philo->seat + 1, msg);
	pthread_mutex_unlock(&philo->info->lock);
}

void	pthread_closer(t_info *info)
{
	int	n;

	n = 0;
	while (n < info->philo_nb)
	{
		pthread_mutex_unlock(&info->fork[n]);
		pthread_mutex_destroy(&info->fork[n]);
		pthread_mutex_destroy(&info->philo[n].lock);
		n++;
	}
	pthread_mutex_destroy(&info->lock);
	n = 0;
	while (n < info->philo_nb)
	{
		pthread_join(info->philo[n].id, NULL);
		n++;
	}
}
