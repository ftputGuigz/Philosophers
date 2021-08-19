/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpetit <gpetit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 13:37:02 by gpetit            #+#    #+#             */
/*   Updated: 2021/08/19 11:59:47 by gpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_fork_to_usable(t_philo *philo, int elems)
{
	int	i;

	i = 0;
	while (i < elems)
	{
		philo[i].nb = i + 1;
		philo[i].fork = 1;
		if (pthread_mutex_init(&philo[i].mutex, NULL))
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

t_philo	*create_philo_give_them_forks(t_datas *data)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo) * data->nb);
	if (!philo)
		return (NULL);
	memset(philo, 0, sizeof(philo) * data->nb);
	if (set_fork_to_usable(philo, data->nb))
	{
		free(philo);
		philo = NULL;
	}
	return (philo);
}

int	start_vandevilling(t_datas *data)
{
	int	ret_status;

	ret_status = SUCCESS;
	data->philo = create_philo_give_them_forks(data);
	if (!data->philo)
		return (ERROR);
	if (gettimeofday(&(data->time), NULL))
	{
		free(data->philo);
		return (ERROR);
	}
	ret_status = launch_threads(data);
	free(data->philo);
	return (ret_status);
}