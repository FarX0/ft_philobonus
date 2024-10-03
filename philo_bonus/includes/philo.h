/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfalchi <tfalchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:01:24 by lebartol          #+#    #+#             */
/*   Updated: 2024/09/16 18:04:04 by tfalchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <fcntl.h>
# include "utils.h"

typedef struct s_fork
{
	pthread_mutex_t	fork;
	t_bool			is_avaible;
}	t_fork;

// Is managed as a "chain" connecting first and last "nodes"
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	size_t			last_meal;
	size_t			birthday;
	struct s_philo	*right_philo;
	struct s_philo	*left_philo;
	struct s_data	*data;
	pthread_t		monitor;
	pthread_t		philo_stop;
	pid_t			pid;
	// t_fork			*r_fork;
	// t_fork			*l_fork;
	// pthread_mutex_t	philo_lock;
}	t_philo;

typedef struct s_data
{
	int				number_of_philosophers;	
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_count;
	t_bool			game_over;
	size_t			timestamp;
	t_philo			*first_philo;
	// pthread_mutex_t	write_lock;
	// pthread_mutex_t	game_lock;
	// pthread_mutex_t	p_mutex;
	sem_t			*write_lock;
	sem_t			*game_stop;
	sem_t			*forks;
	sem_t			*food;
	sem_t			*bool_n;
}	t_data;

void 	free_philo(t_philo *p);

t_data	*parse_arguments(int argc, char *argv[]);

t_data	*initialize_table(t_data *data);

void	display_table(t_data *data);

void	*free_all(t_data *data, char *error);

void	ft_mutex_write(t_philo *p, char *str);

void	philo_sleep(t_philo *p);

void	philo_eat(t_philo *p);

void	alone(t_philo *p);

void	*monitor(void *p);

t_philo	*check_philo(t_data *data);

int		check_meals(t_philo *p);

int		check_death(t_philo *p);

int		check_meal(t_philo *p);

void	game_over(t_philo *p);

int		get_game(t_data *data, int stop);

void	semaphore_init(t_data *data);

#endif
