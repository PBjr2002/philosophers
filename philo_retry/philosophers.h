/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:34:25 by pauberna          #+#    #+#             */
/*   Updated: 2024/04/26 16:57:27 by pauberna         ###   ########.fr       */
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

typedef	struct	s_philo
{
	int				seat;
	long			hp;
	long			sleep;
	long			think;
	long			chewing;
	struct s_table	*table;
	pthread_t		id;
}				t_philo;

typedef	struct	s_table
{
	int				philo_nb;
	long			start;
	long			rip_timer;
	long			eat_timer;
	long			sleep_timer;
	long			eat_counter;
	t_philo			*philo;
	pthread_mutex_t	lock;
	pthread_mutex_t	*fork;
}				t_table;

//main.c
long	get_ms(void);
void	print_msg(t_philo *philo, char *msg);
void	upgraded_sleep(long timer);

//actions.c
void	eating_time(t_philo *philo);
void	sleeping_time(t_philo *philo);
void	thinking_time(t_philo *philo);
void	take_forks(t_philo *philo);
void	leave_forks(t_philo *philo);

#endif