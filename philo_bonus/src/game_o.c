/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_o.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfalchi <tfalchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:18:27 by tfalchi           #+#    #+#             */
/*   Updated: 2024/09/16 17:26:03 by tfalchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	set_game_over(t_philo *p)
{
	sem_close(p->data->write_lock);
	sem_close(p->data->game_stop);
	sem_close(p->data->forks);
	sem_close(p->data->food);
	sem_close(p->data->bool_n);
	sem_unlink("write_lock");
	sem_unlink("game_stop");
	sem_unlink("forks");
	sem_unlink("food");
	sem_unlink("bool_n");
}

void	game_over(t_philo *p)
{
	set_game_over(p);
	if (p->right_philo)
		p = p->right_philo;
	while (p != p->data->first_philo)
	{
		set_game_over(p);
		if (p->right_philo)
			p = p->right_philo;
	}
}

void free_philo(t_philo *p)
{
	free(p->right_philo);
	free(p->left_philo);
	free(p->data);
	free(p);
}
