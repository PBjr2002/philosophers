/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:25:33 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/23 16:15:02 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	pthread_mutex_lock(&philo->philo_lock);
	pthread_mutex_lock(&philo->table->lock);
	philo->hp = get_ms() + philo->table->rip_timer;
	pthread_mutex_unlock(&philo->table->lock);
	pthread_mutex_unlock(&philo->philo_lock);
	if (philo->table->philo_nb == 1)
	{
		pthread_mutex_lock(&philo->philo_lock);
		pthread_mutex_lock(&philo->table->fork[0]);
		printf("%ld %i %s\n", get_ms() - philo->table->start,
			philo->seat + 1, "has taken a fork");
		pthread_mutex_unlock(&philo->table->fork[0]);
		pthread_mutex_unlock(&philo->philo_lock);
		return (NULL);
	}
	wait_for_turn(philo);
	eating_time(philo);
	return (NULL);
}

void	glockinator(t_table *table)
{
	int		n;

	n = 0;
	while (1)
	{
		pthread_mutex_lock(&table->lock);
		if (table->full_philo == table->philo_nb)
		{
			pthread_mutex_unlock(&table->lock);
			return ;
		}
		pthread_mutex_unlock(&table->lock);
		pthread_mutex_lock(&table->philo[n].philo_lock);
		if (get_ms() > table->philo[n].hp && table->philo[n].full == 0)
			break ;
		pthread_mutex_unlock(&table->philo[n].philo_lock);
		n++;
		if (n == table->philo_nb)
			n = 0;
		usleep(50);
	}
	pthread_mutex_unlock(&table->philo[n].philo_lock);
	self_destruct(table, n);
}

void	self_destruct(t_table *table, int n)
{
	pthread_mutex_lock(&table->write);
	printf("%ld %i %s\n", get_ms() - table->start, n + 1, "died");
	n = 0;
	while (n < table->philo_nb)
	{
		pthread_mutex_lock(&table->philo[n].philo_lock);
		table->philo[n].go_exit = 1;
		pthread_mutex_unlock(&table->philo[n].philo_lock);
		n++;
	}
	pthread_mutex_unlock(&table->write);
}

void	wait_for_turn(t_philo *philo)
{
	if (philo->seat % 2 != 0)
		upgraded_sleep(philo, get_ms() + (philo->chewing - 60));
	else if (philo->seat == philo->table->philo_nb - 1
		&& philo->table->philo_nb % 2 != 0)
		upgraded_sleep(philo, get_ms() + (philo->chewing + 5));
}

int	main(int ac, char **av)
{
	struct timeval	time;
	t_table			*table;
	int				n;

	if (ac < 5 || ac > 6)
		return (0);
	table = table_init(ac, av);
	if (!table)
		return (printf("Invalid arguments\n"), 0);
	n = 0;
	pthread_mutex_lock(&table->lock);
	while (n < table->philo_nb)
	{
		if (pthread_create(&table->philo[n].id, NULL, \
			routine, &table->philo[n]) != 0)
			return (pthread_mutex_unlock(&table->lock), \
			cleaner(table, n), printf("Failed to creat a thread\n"), 0);
		n++;
	}
	gettimeofday(&time, NULL);
	table->start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	pthread_mutex_unlock(&table->lock);
	glockinator(table);
	cleaner(table, n - 1);
	return (0);
}
