/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:25:33 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/21 17:20:19 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
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
		pthread_mutex_lock(&table->philo[n].philo_lock);
		if (get_ms() > table->philo[n].hp && table->philo[n].full == 0)
			break ;
		pthread_mutex_unlock(&table->philo[n].philo_lock);
		n++;
		if (n == table->philo_nb)
			n = 0;
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
			upgraded_sleep(philo, get_ms() + philo->chewing);
	else if (philo->seat == philo->table->philo_nb - 1
			&& philo->table->philo_nb % 2 != 0)
			upgraded_sleep(philo, get_ms() + (philo->chewing + 5));
}

int	main(int ac, char **av)
{
	t_table	*table;
	int		n;

	if (ac < 5 || ac > 6)
		return (0);
	table = table_init(ac, av);
	if (!table)
	{
		printf("Invalid arguments\n");
		return (0);
	}
	n = 0;
	while (n < table->philo_nb)
	{
		pthread_create(&table->philo[n].id, NULL, routine, &table->philo[n]);
		n++;
	}
	glockinator(table);
	cleaner(table);
	return (0);
}
