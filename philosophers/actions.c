/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:01:14 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/23 21:30:18 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	thinking_time(t_philo *philo)
{
	long	time;

	print_msg(philo, "is thinking");
	time = get_time(philo->info);
	if (philo->philo_nb % 2 == 0)
		upgraded_sleep(philo->info, time + (philo->chewing - philo->sleep_t));
	else
		upgraded_sleep(philo->info, time + (philo->chewing - philo->sleep_t) * 2);
	eating_time(philo);
}

void	sleeping_time(t_philo *philo)
{
	long	time;
	
	print_msg(philo, "is sleeping");
	time = get_time(philo->info);
	upgraded_sleep(philo->info, time + philo->sleep_t);
	thinking_time(philo);
}

void	eating_time(t_philo *philo)
{
	long	time;

	take_forks(philo);
	time = get_time(philo->info);
	//printf("hp = %ld\n", philo->hp);
	//printf("time = %ld\n", time);
	philo->hp = time + philo->hp;
	print_msg(philo, "is eating");
	time = get_time(philo->info);
	upgraded_sleep(philo->info, time + philo->chewing);
	leave_forks(philo);
	sleeping_time(philo);
}

void	take_forks(t_philo *philo)
{
	if (philo->seat == philo->philo_nb - 1)
	{	
		pthread_mutex_lock(&philo->info->fork[0]);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->info->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
	}
	else if (philo->seat % 2 == 0)
	{
		pthread_mutex_lock(&philo->info->fork[philo->seat + 1]);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->info->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
	}
	else if (philo->seat % 2 != 0)
	{
		pthread_mutex_lock(&philo->info->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->info->fork[philo->seat + 1]);
		print_msg(philo, "has taken a fork");
	}
}

void	leave_forks(t_philo *philo)
{
	if (philo->seat == philo->philo_nb - 1)
	{
		pthread_mutex_unlock(&philo->info->fork[0]);
		//print_msg(philo, "has dropped a fork");
		pthread_mutex_unlock(&philo->info->fork[philo->seat]);
		//(philo, "has dropped a fork");
	}
	else if (philo->seat % 2 == 0)
	{
		pthread_mutex_unlock(&philo->info->fork[philo->seat + 1]);
		//print_msg(philo, "has dropped a fork");
		pthread_mutex_unlock(&philo->info->fork[philo->seat]);
		//print_msg(philo, "has dropped a fork");
	}
	else if (philo->seat % 2 != 0)
	{
		pthread_mutex_unlock(&philo->info->fork[philo->seat]);
		//print_msg(philo, "has dropped a fork");
		pthread_mutex_unlock(&philo->info->fork[philo->seat + 1]);
		//print_msg(philo, "has dropped a fork");
	}
}
