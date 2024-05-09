/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:22:46 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/09 19:15:51 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *philo)
{
	if (philo->seat % 2 == 0)
	{
		if (philo->seat == philo->table->philo_nb - 1)
			pthread_mutex_lock(&philo->table->fork[0]);
		else
			pthread_mutex_lock(&philo->table->fork[philo->seat + 1]);
		print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
	}
	else if (philo->seat % 2 != 0)
	{
		pthread_mutex_lock(&philo->table->fork[philo->seat]);
		print_msg(philo, "has taken a fork");
		if (philo->seat == philo->table->philo_nb - 1)
			pthread_mutex_lock(&philo->table->fork[0]);
		else
			pthread_mutex_lock(&philo->table->fork[philo->seat + 1]);
		print_msg(philo, "has taken a fork");
	}
}

void	leave_forks(t_philo *philo)
{
	if (philo->seat % 2 == 0)
	{
		if (philo->seat == philo->table->philo_nb - 1)
			pthread_mutex_unlock(&philo->table->fork[0]);
		else
			pthread_mutex_unlock(&philo->table->fork[philo->seat + 1]);
		pthread_mutex_unlock(&philo->table->fork[philo->seat]);
	}
	else if (philo->seat % 2 != 0)
	{
		pthread_mutex_unlock(&philo->table->fork[philo->seat]);
		if (philo->seat == philo->table->philo_nb - 1)
			pthread_mutex_unlock(&philo->table->fork[0]);
		else
			pthread_mutex_unlock(&philo->table->fork[philo->seat + 1]);
	}
}

void	eating_time(t_philo *philo)
{
	long	time;

	pthread_mutex_lock(&philo->table->lock);
	if (philo->table->go_exit == 1)
	{
		pthread_mutex_unlock(&philo->table->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->table->lock);
	if (philo->eat_counter == 0 && philo->full == 0)
	{
		philo->full = 1;
		philo->table->full_philos++;
	}
	take_forks(philo);
	pthread_mutex_lock(&philo->philo_lock);
	time = get_ms();
	philo->hp = time + philo->table->rip_timer;
	if (philo->table->go_exit == 1)
	{
		pthread_mutex_unlock(&philo->philo_lock);
		leave_forks(philo);
		return ;
	}
	pthread_mutex_unlock(&philo->philo_lock);
	print_msg(philo, "is eating");
	if (philo->full == 0)
		philo->eat_counter--;
	pthread_mutex_lock(&philo->philo_lock);
	upgraded_sleep(philo, get_ms() + philo->chewing);
	pthread_mutex_unlock(&philo->philo_lock);
	leave_forks(philo);
	sleeping_time(philo);
}

void	sleeping_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->lock);
	if (philo->table->go_exit == 1)
	{
		pthread_mutex_unlock(&philo->table->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->table->lock);
	print_msg(philo, "is sleeping");
	pthread_mutex_lock(&philo->philo_lock);
	upgraded_sleep(philo, get_ms() + philo->sleep);
	pthread_mutex_unlock(&philo->philo_lock);
	thinking_time(philo);
}

void	thinking_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->lock);
	if (philo->table->go_exit == 1)
	{
		pthread_mutex_unlock(&philo->table->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->table->lock);
	print_msg(philo, "is thinking");
	if (philo->table->philo_nb % 2 == 0)
		philo->think = philo->chewing - philo->sleep;
	else
		philo->think = ((philo->chewing * 2) - philo->sleep) * 0.42;
	pthread_mutex_lock(&philo->philo_lock);
	upgraded_sleep(philo, get_ms() + philo->think);
	pthread_mutex_unlock(&philo->philo_lock);
	eating_time(philo);
}
