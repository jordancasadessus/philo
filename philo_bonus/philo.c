/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltressen <ltressen@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 14:59:50 by ltressen          #+#    #+#             */
/*   Updated: 2023/08/09 15:57:04 by jcasades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	rip_timer(t_philo *philo)
{
	int	i;

	i = 0;
	while (1)
	{	
		if (get_time() - philo->time_since_eat > philo->info->time_to_die)
		{
			philo->is_dead = 1;
			status_message(philo, "is kill 💀");
			while (i < philo->info->num_of_phil)
			{
				sem_post(philo->info->dead);
				i++;
			}
			break;
		}
		if (philo->is_dead)
			break;
	}
}

void	check_rip(t_philo *philo)
{
	sem_wait(philo->info->dead);
	philo->is_dead = 1;
}

void	mangiare(t_philo *philo)
{
	printf("ici\n");
	printf("%p\n", philo->info->forks);
	sem_post(philo->info->forks);
	sem_wait(philo->info->forks);
	printf("kekw");
	status_message(philo, " has taken a forquetta 🍴");
	sem_wait(philo->info->forks);
	status_message(philo, " has taken a forquetta 🍴");
	status_message(philo, " is mangiaring 🍝");
	philo->eat_count++;
	philo->time_since_eat = get_time();
	if (philo->eat_count == philo->info->win_con)
		sem_post(philo->info->cwin);
	ft_usleep(philo->info->time_to_eat);
	philo->time_since_eat = get_time();
	status_message(philo, " is dodoing 💤");
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
	ft_usleep(philo->info->time_to_sleep);
	status_message(philo, " is pensing 🤔");
}

void	*loop(t_data *data, int i)
{
	t_philo	*philo;
	
	philo = &data->phil[i];
	if (i % 2 == 0)
		ft_usleep(data->time_to_eat / 10);
	pthread_create(&philo->rip, NULL, (void *)rip_timer, &philo);
	pthread_create(&philo->rips, NULL, (void *)check_rip, &philo);
	while (1)
	{
		printf("here\n");
		if (!philo->is_dead)
		{
			printf("la\n");
			mangiare(philo);
		}
		else
			break ;
	}
	pthread_join(philo->rip, NULL);
	pthread_join(philo->rips, NULL);
	exit(0);
}
