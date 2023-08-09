/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltressen <ltressen@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 10:35:57 by ltressen          #+#    #+#             */
/*   Updated: 2023/08/09 15:46:38 by jcasades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	is_dead(t_data *data)
{
	sem_wait(data->dead);
	data->all_deads = 1;
}

void	is_win(t_data *data)
{
	int	i;

	i = 0;
	if (data->win_con != 0)
	{
		while (i < data->num_of_phil)
		{
			sem_wait(data->cwin);
			i++;
		}
	data->win = 1;
	}
}

void	ft_exit(t_data *data)
{
	pthread_join(data->dead_id, NULL);
	pthread_join(data->win_id, NULL);
	sem_close(data->forks);
	sem_close(data->print);
	sem_close(data->dead);
	sem_close(data->cwin);
	return ;
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
		init_params(&data, argc, argv);
	else
	{
		printf("Usage: -number of philosophers -time_to_die");
		printf(" -time_to_eat -time_to_sleep -[number of times to eat]\n");
		return (0);
	}
	init_philos(&data);
	pthread_create(&data.dead_id, NULL, (void *)is_dead, &data);
	pthread_create(&data.win_id, NULL, (void *)is_win, &data);
	while (1)
	{
		if (data.win)
		{
			printf("🤮🤮🤮 Too much mozzarella 🤮🤮🤮\n");
			ft_exit(&data);
		}
		if (data.all_deads)
			ft_exit(&data);
	}
	return (0);
}
