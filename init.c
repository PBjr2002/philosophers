/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:29:52 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/23 15:52:04 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	latoi_init(t_table *table, char **av)
{
	table->philo_nb = ft_latoi(av[1]);
	table->rip_timer = ft_latoi(av[2]);
	table->eat_timer = ft_latoi(av[3]);
	table->sleep_timer = ft_latoi(av[4]);
	if (table->philo_nb < 1 || table->rip_timer < 1
		|| table->eat_timer < 1 || table->sleep_timer < 1
		|| table->philo_nb > 200)
		return (-1);
	return (0);
}

void	cleaner(t_table *table, int n)
{
	int	i;

	i = 0;
	while (i < table->philo_nb && n >= 0)
	{
		pthread_join(table->philo[n].id, NULL);
		i++;
		n--;
	}
	i = 0;
	while (i < table->philo_nb)
	{
		if (pthread_mutex_destroy(&table->fork[i]) != 0)
			return ;
		if (pthread_mutex_destroy(&table->philo[i].philo_lock) != 0)
			return ;
		i++;
	}
	if (pthread_mutex_destroy(&table->lock) != 0)
		return ;
	if (pthread_mutex_destroy(&table->write) != 0)
		return ;
	free(table->philo);
	free(table->fork);
	free(table);
}

int	mutex_init(t_table *table)
{
	int	n;

	if (pthread_mutex_init(&table->lock, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&table->write, NULL) != 0)
		return (-1);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	n = 0;
	while (n < table->philo_nb)
	{
		if (pthread_mutex_init(&table->philo[n].philo_lock, NULL) != 0)
			return (-1);
		if (pthread_mutex_init(&table->fork[n], NULL) != 0)
			return (-1);
		n++;
	}
	return (0);
}

int	philo_init(t_table *table)
{
	int	n;

	table->philo = malloc(sizeof(t_philo) * table->philo_nb);
	if (!table->philo)
		return (-1);
	if (mutex_init(table) == -1)
		return (-1);
	n = 0;
	while (n < table->philo_nb)
	{
		table->philo[n].seat = n;
		table->philo[n].full = 0;
		table->philo[n].go_exit = 0;
		table->philo[n].eat_counter = table->eat_counter;
		table->philo[n].hp = get_ms() + table->rip_timer;
		table->philo[n].sleep = table->sleep_timer;
		table->philo[n].think = 0;
		table->philo[n].chewing = table->eat_timer;
		table->philo[n].rip_timer = table->rip_timer;
		table->philo[n].table = table;
		n++;
	}
	return (0);
}

t_table	*table_init(int ac, char **av)
{
	t_table			*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	if (latoi_init(table, av) == -1)
		return (free(table), NULL);
	if (ac == 6)
	{
		table->eat_counter = ft_latoi(av[5]);
		if (table->eat_counter <= 0)
			return (free(table), NULL);
	}
	else
		table->eat_counter = -1;
	table->full_philo = 0;
	if (philo_init(table) == -1)
		return (free(table->philo), free(table), NULL);
	return (table);
}
