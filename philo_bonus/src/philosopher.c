/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfalchi <tfalchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:05:01 by lebartol          #+#    #+#             */
/*   Updated: 2024/09/19 16:45:03 by tfalchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

t_bool	take_forks(t_philo *philo);

void	philo_sleep(t_philo *p)
{
	ft_mutex_write(p, "is sleeping.");
	ft_usleep(p->data->time_to_sleep);
}

void	philo_eat(t_philo *p)
{
	if (p->data->number_of_philosophers == 1)
		alone(p);
	if (get_game(p->data, 0) == 1)
	{
		sem_post(p->data->game_stop);
		return ;
	}
	if(check_meals(p) == 1)
	{
		while (get_game(p->data, 0) != 1)
			p->last_meal = get_current_time();
		return ;
	}
	sem_wait(p->data->forks);
	if (get_game(p->data, 0) == 1)
	{
		sem_post(p->data->game_stop);
		sem_post(p->data->forks);
		return ;
	}
	ft_mutex_write(p, "took fork.");
	sem_wait(p->data->forks);
	if (get_game(p->data, 0) == 1)
	{
		sem_post(p->data->game_stop);
		sem_post(p->data->forks);
		sem_post(p->data->forks);
		return ;
	}
	ft_mutex_write(p, "took fork.");
	ft_mutex_write(p, "is eating.");
	p->last_meal = get_current_time();
	ft_usleep(p->data->time_to_eat);
	sem_post(p->data->forks);
	sem_post(p->data->forks);
	if (get_game(p->data, 0) == 1)
	{
		sem_post(p->data->game_stop);
		return ;
	}
	p->meals_eaten++;
	philo_sleep(p);
	if (get_game(p->data, 0) == 1)
	{
		sem_post(p->data->game_stop);
		return ;
	}
	ft_mutex_write(p, "is thinking.");
}

void	alone(t_philo *p)
{
	while (1)
	{
		ft_mutex_write(p, "is eating.");
		p->last_meal = get_current_time();
		if(check_meals(p) == 1)
		{
			get_game(p->data, 1);
			return ;
		}
		ft_usleep(p->data->time_to_eat);
		philo_sleep(p);
		ft_mutex_write(p, "is thinking.");
	}
}
	// if (p->data->number_of_philosophers == 1)
	// 	return ;
	// if (p->data->number_of_philosophers == p->id)
	// {
	// 	pthread_mutex_lock(&p->r_fork->fork);
	// 	pthread_mutex_lock(&p->l_fork->fork);
	// }
	// else
	// {
	// 	pthread_mutex_lock(&p->l_fork->fork);
	// 	pthread_mutex_lock(&p->r_fork->fork);
	// }
	// ft_mutex_write(p, "took right fork.");
	// ft_mutex_write(p, "took left fork.");
	// ft_mutex_write(p, "is eating.");
	// pthread_mutex_lock(&p->philo_lock);
	// p->last_meal = get_current_time();
	// p->meals_eaten++;
	// pthread_mutex_unlock(&p->philo_lock);
	// ft_usleep(p->data->time_to_eat);
	// pthread_mutex_unlock(&p->l_fork->fork);
	// pthread_mutex_unlock(&p->r_fork->fork);
	// philo_sleep(p);
	// ft_mutex_write(p, "is thinking.");

/* t_bool	take_forks(t_philo *philo)
{
	int	f_r;
	int	f_l;

	if (philo->r_fork->is_avaible && philo->l_fork->is_avaible)
	{
		f_r = pthread_mutex_lock(&philo->r_fork->fork);
		f_l = pthread_mutex_lock(&philo->l_fork->fork);
		if (f_r != 0 || f_l != 0)
		{
			if (f_r != 0)
				pthread_mutex_unlock(&philo->r_fork->fork);
			if (f_l != 0)
				pthread_mutex_unlock(&philo->l_fork->fork);
			return (false);
		}
		philo->r_fork->is_avaible = true;
		philo->l_fork->is_avaible = true;
		return (true);
	}
	return (false);
} */
