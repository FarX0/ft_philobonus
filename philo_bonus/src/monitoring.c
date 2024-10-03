/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfalchi <tfalchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:43:14 by lebartol          #+#    #+#             */
/*   Updated: 2024/09/20 16:49:15 by tfalchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*int	check_meal(t_philo *p)
{
	int	i;

	pthread_mutex_lock(p->philo_lock);
	i = (p->meals_eaten < p->data->meals_count);
	pthread_mutex_unlock(p->philo_lock);
	return (i);
}*/
// if (data->meals_count == -1)
// 	return (0);
// p = data->first_philo;
// if (check_meal(p))
// 	return (0);
// if (p->right_philo)
// 	p = p->right_philo;
// while (p != data->first_philo)
// {
// 	if (check_meal(p))
// 		return (0);
// 	if (p->right_philo)
// 		p = p->right_philo;
// }

int check_meals(t_philo *p)
{
	if (p->data->meals_count == -1)
		return (0);
	if (p->meals_eaten < p->data->meals_count)
		return (0);
	sem_post(p->data->food);
	return (1);
}

int check_death(t_philo *p)
{
	int i;

	i = ((int)(get_current_time() - p->last_meal) >= p->data->time_to_die + 5);
	return (i);
}

/* int	check_death(t_philo *p)
{
	int			i;
	static int	y;

	if (p->id == 1)
		y = 0;
	pthread_mutex_lock(&p->philo_lock);
	i = ((int)(get_current_time() - p->last_meal) >= p->data->time_to_die + 5);
	y += (p->meals_eaten > p->data->meals_count && p->data->meals_count != -1);
	pthread_mutex_unlock(&p->philo_lock);
	if (y == p->data->number_of_philosophers)
		return (1);
	if (p->id == p->data->number_of_phicheck_deathlosophers)
		y = 0;
	return (i);
} */

void *monitor(void *data)
{
	t_philo *p;
	int i;

	p = (t_philo *)data;
	while (1)
	{
		if (get_game(p->data, 0) == 1)
		{
			sem_post(p->data->game_stop);
			return (NULL);
		}
		if (check_death(p))
		{
			sem_post(p->data->game_stop);
			sem_wait(p->data->bool_n);
			if (p->data->game_over == 0)
			{			
				sem_wait(p->data->write_lock);
				printf("[%zu] %d died\n", (get_current_time() - p->birthday), p->id);
				sem_post(p->data->write_lock);
			}
			sem_post(p->data->bool_n);
			get_game(p->data, 1);
			i = 0;
			while (i < p->data->meals_count)
				sem_post(p->data->food);
			return (NULL);
		}
	}
	return (NULL);
}
