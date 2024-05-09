/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauberna <pauberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:34:25 by pauberna          #+#    #+#             */
/*   Updated: 2024/05/09 19:15:01 by pauberna         ###   ########.fr       */
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

typedef struct s_philo
{
	int				seat;
	int				full;
	int				eat_counter;
	long			hp;
	long			sleep;
	long			think;
	long			chewing;
	struct s_table	*table;
	pthread_t		id;
	pthread_mutex_t	philo_lock;
}				t_philo;

typedef struct s_table
{
	int				philo_nb;
	int				go_exit;
	int				full_philos;
	long			start;
	long			rip_timer;
	long			eat_timer;
	long			sleep_timer;
	long			eat_counter;
	t_philo			*philo;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
	pthread_mutex_t	*fork;
}				t_table;

//main.c
void	*routine(void *p);
void	glockinator(t_table *table);
void	wait_for_turn(t_philo *philo);

//init.c
void	mutex_init(t_table *table);
void	philo_init(t_table *table);
t_table	*table_init(int ac, char **av);
void	mutex_cleaner(t_table *table);

//utils.c
long	get_ms(void);
long	ft_latoi(char *str);
void	print_msg(t_philo *philo, char *msg);
void	upgraded_sleep(t_philo *philo, long timer);

//actions.c
void	take_forks(t_philo *philo);
void	leave_forks(t_philo *philo);
void	eating_time(t_philo *philo);
void	sleeping_time(t_philo *philo);
void	thinking_time(t_philo *philo);

#endif