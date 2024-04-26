/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:34:10 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/22 15:24:31 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time(t_info *info)
{
	gettimeofday((struct timeval *)info->clock, NULL);
	return ((info->clock->mil_sec * 1000 + \
	info->clock->sec - info->start) / 1000000);
}

/* unsigned long	get_time_fresh(void)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((time.tv_sec * (long)1000 + \
	time.tv_usec) / (long)1000 );
}

unsigned long	get_time(t_info *info)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((time.tv_sec * (long)1000 + \
	time.tv_usec) / (long)1000 - info->start);
}
unsigned long	get_time_philo(t_philo *philo)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((time.tv_sec * (long)1000 + \
	time.tv_usec) / (long)1000 - philo->start);
} */

int		ft_atoi(char *str)
{
	int	number;
	int	n;
	int	signal;

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

void	upgraded_sleep(t_info *info, long timer)
{
	while (get_time(info) < timer)
		usleep(250);
}

void	check_even_philo(t_info *info)
{
	int		n;
	long	time;

	n = 0;
	while (n < info->philo_nb)
	{
		time = get_time(info);
		info->philo[n].hp = time + info->time_to_rip;
		if (info->philo[n].seat % 2 == 0)
		{
			if (n != info->philo_nb - 1)
			{
				pthread_mutex_lock(&info->philo->info->fork[n + 1]);
				printf("%lu %d has taken a fork\n", time, info->philo[n].seat + 1);
				pthread_mutex_lock(&info->philo->info->fork[n]);
				printf("%lu %d has taken a fork\n", time, info->philo[n].seat + 1);
			}
		}
		n++;
	}
}

void	wait_for_turn(t_philo *philo)
{
	if (philo->philo_nb % 2 != 0 && philo->seat == philo->philo_nb - 1)
		upgraded_sleep(philo->info, get_time(philo->info) + (philo->chewing + 20));
}

void	*skirmish(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	wait_for_turn(philo);
	eating_time(philo);
	return (NULL);
}

t_info	*philo_init(char **av)
{
	t_info	*info;
	int		n;

	info = malloc(sizeof(t_info));
	info->philo_nb = ft_atoi(av[1]);
	info->time_to_rip = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_zzz = ft_atoi(av[4]);
	if (av[5])
		info->eat_nb = ft_atoi(av[5]);
	info->clock = malloc(sizeof(t_timeval));
	info->start = gettimeofday((struct timeval *)info->clock, NULL);
	pthread_mutex_init(&info->lock, NULL);
	info->fork = malloc(sizeof(pthread_mutex_t) * info->philo_nb);
	info->philo = malloc(sizeof(t_philo) * info->philo_nb);
	n = 0;
	while (n < info->philo_nb)
	{
		pthread_mutex_init(&info->fork[n], NULL);
		n++;
	}
	n = 0;
	while (n < info->philo_nb)
	{
		info->philo[n].info = info;
		info->philo[n].start = info->start;
		info->philo[n].philo_nb = info->philo_nb;
		info->philo[n].seat = n;
		info->philo[n].clock = info->clock;
		info->philo[n].hp = info->time_to_rip;
		info->philo[n].chewing = info->time_to_eat;
		info->philo[n].sleep_t = info->time_to_zzz;
		pthread_mutex_init(&info->philo[n].lock, NULL);
		n++;
	}
	return (info);
}

int	glock_time(t_philo *philo)
{
	long	time;
	int		n;

	n = 0;
	while (1)
	{
		pthread_mutex_lock(&philo->info->lock);
		time = get_time(philo->info);
		//if (time > philo[n].hp)
		//{
		//	pthread_mutex_unlock(&philo[n].lock);
		//	break;
		//}
		pthread_mutex_unlock(&philo->info->lock);
		n++;
		if (n == philo->philo_nb)
			n = 0;
	}
	pthread_mutex_lock(&philo->info->lock);
	print_msg(philo, "philo has died");
	pthread_mutex_unlock(&philo->info->lock);
	pthread_closer(philo->info);
	return (0);
}

int	main(int ac, char **av)
{
	t_info		*info;
	int				n;

	info = philo_init(av);
	//check_even_philo(info);
	n = 0;
	while (n < info->philo_nb)
	{
		pthread_create(&info->philo[n].id, NULL, skirmish, &info->philo[n]);
		n++;
	}
	if(glock_time(info->philo) == 0)
		return (0);
	//pthread_mutex_destroy(&info->lock);
	//pthread_exit(NULL);
	(void)ac;
	return (0);
}

/* 410 100 100

	0    100    200    300    400   500
1	\	  \.............\      \.......
2   ......\      \.............\      \
3   \     \.............\      \.......
4   ......\      \..............\      \
5   .............\      \..............\ */