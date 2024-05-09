/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:29:52 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/09 19:49:01 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mutex_init(t_table *table)
{
	int	n;

	pthread_mutex_init(&table->lock, NULL);
	pthread_mutex_init(&table->write, NULL);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	n = 0;
	while (n < table->philo_nb)
	{
		pthread_mutex_init(&table->philo[n].philo_lock, NULL);
		pthread_mutex_init(&table->fork[n], NULL);
		n++;
	}
}

void	philo_init(t_table *table)
{
	int	n;

	table->philo = malloc(sizeof(t_philo) * table->philo_nb);
	mutex_init(table);
	n = 0;
	while (n < table->philo_nb)
	{
		table->philo[n].seat = n;
		table->philo[n].full = 0;
		table->philo[n].eat_counter = table->eat_counter;
		table->philo[n].hp = table->start + table->rip_timer;
		table->philo[n].sleep = table->sleep_timer;
		table->philo[n].think = 0;
		table->philo[n].chewing = table->eat_timer;
		table->philo[n].table = table;
		n++;
	}
}

t_table	*table_init(int ac, char **av)
{
	struct timeval	time;
	t_table			*table;

	if (ac < 5 || ac > 6)
		return (NULL);
	table = malloc(sizeof(t_table));
	gettimeofday(&time, NULL);
	table->start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	table->philo_nb = ft_latoi(av[1]);
	table->rip_timer = ft_latoi(av[2]);
	table->eat_timer = ft_latoi(av[3]);
	table->sleep_timer = ft_latoi(av[4]);
	if (table->philo_nb < 1 || table->rip_timer < 1
		|| table->eat_timer < 1 || table->sleep_timer < 1)
		return (NULL);
	if (ac == 6)
		table->eat_counter = ft_latoi(av[5]);
	else
		table->eat_counter = -1;
	if (table->eat_counter < -1 || table->eat_counter == 0)
		return (NULL);
	table->go_exit = 0;
	table->full_philos = 0;
	philo_init(table);
	return (table);
}

void	mutex_cleaner(t_table *table)
{
	int	n;

	n = 0;
	while (n < table->philo_nb)
	{
		if (pthread_mutex_lock(&table->philo[n].philo_lock) == -1)
			pthread_mutex_unlock(&table->philo[n].philo_lock);
		n++;
	}
	if (pthread_mutex_lock(&table->lock) == -1)
		pthread_mutex_unlock(&table->lock);
	if (pthread_mutex_lock(&table->write) == -1)
		pthread_mutex_unlock(&table->write);
}

/* void	cleaner(t_table *table)
{
	int	n;

	
} */
