/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfalchi <tfalchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:55:14 by lebartol          #+#    #+#             */
/*   Updated: 2024/09/20 15:49:43 by tfalchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	semaphore_write(t_data *data, char *str)
{
	sem_wait(data->write_lock);
	if (get_game(data, 0) == 1)
	{
		sem_post(data->write_lock);
		return ;
	}
	printf("%zu\t%s\n", (get_current_time() - data->timestamp), str);
	sem_post(data->write_lock);
}
void	ft_mutex_write(t_philo *p, char *str)
{
	sem_wait(p->data->write_lock);
	printf("[%zu]\t%d\t%s\n", (get_current_time() - p->birthday), p->id, str);
	sem_post(p->data->write_lock);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(1000);
	return (0);
}

void	display_table(t_data *data)
{
	t_philo	*philo_tmp;
	int		i;

	i = 1;
	printf("%p writelock\n", &data->write_lock);
	printf("%p gameover\n", &data->game_over);
	philo_tmp = data->first_philo;
	while (i <= data->number_of_philosophers)
	{
		printf("philo %d\n", philo_tmp->id);
		philo_tmp = philo_tmp->right_philo;
		i++;
	}
}
/* 
t_bool	get_gameover(t_data *data, t_bool action)
{
	t_bool	ret;

	pthread_mutex_lock(&data->game_lock);
	if (action == true)
		data->game_over = true;
	ret = data->game_over;
	pthread_mutex_unlock(&data->game_lock);
	return (ret);
}
 */