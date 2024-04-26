/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:22:46 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/26 17:10:28 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	thinking_time(t_philo *philo)
{
	print_msg(philo, "is thinking");
	if (philo->table->philo_nb % 2 == 0)
		philo->think = philo->chewing - philo->sleep;
	else
		philo->think = philo->chewing - philo->sleep * 2;
	upgraded_sleep(get_ms() + philo->think);
	eating_time(philo);
}

void	sleeping_time(t_philo *philo)
{
	print_msg(philo, "is sleeping");
	upgraded_sleep(get_ms() + philo->sleep);
	thinking_time(philo);
}

void	eating_time(t_philo *philo)
{
	long	time;

	take_forks(philo);
	time = get_ms();
	philo->hp = time + philo->hp;
	print_msg(philo, "is eating");
	upgraded_sleep(get_ms() + philo->chewing);
	leave_forks(philo);
	sleeping_time(philo);
}

void	take_forks(t_philo *philo)
{
	if (philo->seat == philo->table->philo_nb - 1)
	{
		if (philo->table->philo_nb % 2 == 0)
		{
			pthread_mutex_lock(&philo->table->fork[0]);
			print_msg(philo, "has taken a fork");
			pthread_mutex_lock(&philo->table->fork[philo->seat]);
			print_msg(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(&philo->table->fork[philo->seat]);
			print_msg(philo, "has taken a fork");
			pthread_mutex_lock(&philo->table->fork[0]);
			print_msg(philo, "has taken a fork");
		}
	}
	else if (philo->seat % 2 == 0)
	{
		pthread_mutex_lock(&philo->table->fork[philo->seat + 1]);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
	}
	else if (philo->seat % 2 != 0)
	{
		pthread_mutex_lock(&philo->table->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->fork[philo->seat + 1]);
		print_msg(philo, "has taken a fork");
	}
}

void	leave_forks(t_philo *philo)
{
	if (philo->seat == philo->table->philo_nb - 1)
	{
		if (philo->table->philo_nb % 2 == 0)
		{
			pthread_mutex_unlock(&philo->table->fork[0]);
			pthread_mutex_unlock(&philo->table->fork[philo->seat]);
		}
		else
		{
			pthread_mutex_unlock(&philo->table->fork[philo->seat]);
			pthread_mutex_unlock(&philo->table->fork[0]);
		}
	}
	else if (philo->seat % 2 == 0)
	{
		pthread_mutex_unlock(&philo->table->fork[philo->seat + 1]);
		pthread_mutex_unlock(&philo->table->fork[philo->seat]);
	}
	else if (philo->seat % 2 != 0)
	{
		pthread_mutex_unlock(&philo->table->fork[philo->seat]);
		pthread_mutex_unlock(&philo->table->fork[philo->seat + 1]);
	}
}
