/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:34:25 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/19 16:34:03 by pauberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct	s_timeval
{
	time_t			sec;
	suseconds_t		mil_sec;
}				t_timeval;

typedef	struct	s_philo
{
	int				seat;
	int				philo_nb;
	long			start;
	long			hp;
	long			chewing;
	long			sleep_t;
	struct	s_info	*info;
	t_timeval		*clock;
	pthread_t		id;
	pthread_mutex_t	lock;
}				t_philo;

typedef	struct	s_info
{
	int				eat_nb;
	int				philo_nb;
	long			start;
	long			time_to_rip;
	long			time_to_eat;
	long			time_to_zzz;
	t_philo			*philo;
	t_timeval		*clock;
	pthread_mutex_t	lock;
	pthread_mutex_t	*fork;
}				t_info;

long	get_time(t_info *info);
void	upgraded_sleep(t_info *info, long timer);
void	thinking_time(t_philo *philo);
void	sleeping_time(t_philo *philo);
void	eating_time(t_philo *philo);
void	take_forks(t_philo *philo);
void	leave_forks(t_philo *philo);
//unsigned long	get_time_philo(t_philo *philo);
void	print_msg(t_philo *philo, char *msg);
void	pthread_closer(t_info *info);

#endif