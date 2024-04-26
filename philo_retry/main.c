/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:25:33 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/26 17:27:59 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	return (number * signal);
}

long	get_ms(void)
{
	struct timeval	time;
	long			true_time;

	gettimeofday(&time, NULL);
	true_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (true_time);
}

void	print_msg(t_philo *philo, char *msg)
{
	long	time;
	
	pthread_mutex_lock(&philo->table->lock);
	time = get_ms() - philo->table->start;
	printf("%ld %i %s\n", time, philo->seat + 1, msg);
	pthread_mutex_unlock(&philo->table->lock);
}

void	upgraded_sleep(long timer)
{
	while (get_ms() < timer)
		usleep(250);
}

t_table	*table_init(int ac, char **av)
{
	struct timeval	time;
	t_table			*table;
	int				n;

	table = malloc(sizeof(t_table));
	table->philo_nb = ft_latoi(av[1]);
	table->rip_timer = ft_latoi(av[2]);
	table->eat_timer = ft_latoi(av[3]);
	table->sleep_timer = ft_latoi(av[4]);
	if (ac - 1 == 5)
		table->eat_counter = ft_latoi(av[5]);
	else
		table->eat_counter = -1;
	pthread_mutex_init(&table->lock, NULL);
	gettimeofday(&time, NULL);
	table->start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->philo_nb);
	n = 0;
	while (n < table->philo_nb)
	{
		pthread_mutex_init(&table->fork[n], NULL);
		n++;
	}
	table->philo = malloc(sizeof(t_philo) * table->philo_nb);
	n = 0;
	while (n < table->philo_nb)
	{
		table->philo[n].table = table;
		table->philo[n].seat = n;
		table->philo[n].hp = table->rip_timer;
		table->philo[n].sleep = table->sleep_timer;
		table->philo[n].think = 0;
		table->philo[n].chewing = table->eat_timer;
		n++;
	}
	return (table);
}

void	wait_for_turn(t_philo *philo)
{
	if (philo->table->philo_nb % 2 != 0 && philo->seat == philo->table->philo_nb - 1)
		upgraded_sleep(philo->chewing + 20);
}

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	wait_for_turn(philo);
	eating_time(philo);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_table	*table;
	int		n;

	table = table_init(ac, av);
	n = 0;
	while (n < table->philo_nb)
	{
		pthread_create(&table->philo[n].id, NULL, routine, &table->philo[n]);
		//pthread_join(table->philo[n].id, NULL);
		n++;
	}
	while (1)
	{
	}
	return (0);
}
