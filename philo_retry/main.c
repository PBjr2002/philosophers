/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:25:33 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/03 20:33:35 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->table->philo_nb == 1)
	{
		pthread_mutex_lock(&philo->table->fork[0]);
		printf("%ld %i %s\n", get_ms() - philo->table->start,
			philo->seat + 1, "has taken a fork");
		pthread_mutex_unlock(&philo->table->fork[0]);
		printf("%ld %i %s\n", get_ms() - philo->table->start,
			philo->seat + 1, "died");
		return (NULL);
	}
	wait_for_turn(philo);
	eating_time(philo);
	return (NULL);
}

void	glockinator(t_table *table)
{
	long	time;
	int		n;

	n = 0;
	while (1)
	{
		pthread_mutex_lock(&table->philo[n].philo_lock);
		time = get_ms();
		if (table->full_philos == table->philo_nb)
		{
			table->go_exit = 1;
			return ;
		}
		else if (time > table->philo[n].hp)
			break ;
		pthread_mutex_unlock(&table->philo[n].philo_lock);
		n++;
		if (n == table->philo_nb)
			n = 0;
	}
	pthread_mutex_unlock(&table->philo[n].philo_lock);
	print_msg(table->philo, "died");
	pthread_mutex_lock(&table->lock);
	table->go_exit = 1;
	pthread_mutex_unlock(&table->lock);
}

void	wait_for_turn(t_philo *philo)
{
	if (philo->table->philo_nb % 2 != 0
		&& philo->seat == philo->table->philo_nb - 1)
		upgraded_sleep(philo->table, get_ms() + (philo->chewing + 5));
}

int	main(int ac, char **av)
{
	t_table	*table;
	int		n;

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
	n = 0;
	while (n < table->philo_nb)
	{
		pthread_join(table->philo[n].id, NULL);
		n++;
	}
	pthread_mutex_unlock(&table->lock);
	return (0);
}
