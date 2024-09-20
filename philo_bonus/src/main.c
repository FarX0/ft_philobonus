/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfalchi <tfalchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:45:02 by lebartol          #+#    #+#             */
/*   Updated: 2024/09/20 16:50:47 by tfalchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	get_game(t_data *data, int stop)
{
	int	i;

	sem_wait(data->bool_n);
	if(stop == 1)
	{
		data->game_over = 0;
		i = data->game_over;
		sem_post(data->bool_n);
		return (i);
	}
	else
	{
		i = data->game_over;
		sem_post(data->bool_n);
	}
	return (i);
}

void	*philo_stop(void *arg)
{
	t_data	*data;
	
	data = (t_data *)arg;
	sem_wait(data->game_stop);
	get_game(data, 1);
	sem_post(data->game_stop);
	return (NULL);
}

void	*philo_eaten(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	i = 0;
	while(i < data->number_of_philosophers)
	{
		sem_wait(data->food);
		i++;
	}
	if (data->meals_count == -1)
		return (NULL);
	sem_post(data->game_stop);
	ft_mutex_write(data->first_philo, "each philosopher is satisfied");
	return (NULL);
}

void	philo_routine(t_data *data, t_philo *p)
{
	pthread_create(&p->game_stop, NULL, philo_stop, (void *)data);
	pthread_create(&p->game_stop, NULL, monitor, (void *)p);
	if (p->id % 2)
		philo_sleep(p);
	while (get_game(p->data, 0) == 1)
	{
		philo_eat(p);
	}
	return ;
}

int	create_philo(t_data *data)
{
	t_philo	*p;
	int		i;

	i = 0;
	p = data->first_philo;
	while (i < data->number_of_philosophers)
	{
		p->pid = fork();
		if (p->pid == -1)
			return (1);
		if (p->pid == 0)
		{
			philo_routine(data, p);
			exit(0);
		}
		p = p->right_philo;
		i++;
	}
	return (0);
}

void	wait_fork(t_data *data)
{
	t_philo	*p;
	int		i;

	i = 0;
	p = data->first_philo;
	while (i < data->number_of_philosophers)
	{
		waitpid(p->pid, NULL, 0);
		p = p->right_philo;
		i++;
	}
}

void	main_monitor(t_data *data)
{
	pthread_t	monitor;
	pthread_t	eaten;
	
	pthread_create(&monitor, NULL, philo_stop, (void *)data);
	pthread_create(&eaten, NULL, philo_eaten, (void *)data);
	pthread_join(monitor, NULL);
	pthread_join(eaten, NULL);
	wait_fork(data);
}


int	main(int argc, char **argv)
{
	t_data	*data;

	data = parse_arguments(argc, argv);
	if (!data)
		return (1);
	data = initialize_table(data);
	semaphore_init(data);
	printf("main\n");
	if (!data)
		return (1);
	if (create_philo(data))
		free_all(data, "Error\n fork failed");
	main_monitor(data);
	printf("end\n");
	free_all(data, NULL);
}
