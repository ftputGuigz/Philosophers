/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpetit <gpetit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 15:29:34 by gpetit            #+#    #+#             */
/*   Updated: 2021/09/02 17:31:25 by gpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitoring(void *elem)
{
	int		ret;
	t_datas	*data;

	data = (t_datas *)elem;
	ret = monitor_everyone(data);
	if (ret == ERROR)
	{
		pthread_mutex_lock(&data->error_mutex);
		data->error = ERROR;
		pthread_mutex_unlock(&data->error_mutex);
	}
	return ((void *) &data->error);
}

void	*routine(void *elem)
{
	int		ret;
	int		ret_1;
	t_philo	*philo;

	philo = (t_philo *)elem;
	ret_1 = SUCCESS;
	if (philo->nb % 2 == 0)
		ret_1 = my_usleep(10, philo->data);
	ret = tamagochi_philo(philo->nb, philo->data);
	while (!ret)
		ret = tamagochi_philo(philo->nb, philo->data);
	if (ret == ERROR || ret_1 == ERROR)
	{
		pthread_mutex_lock(&philo->data->error_mutex);
		philo->data->error = ERROR;
		pthread_mutex_unlock(&philo->data->error_mutex);
	}
	return ((void *) &philo->data->error);
}

pthread_t	*create_thread_tab_init_mutex(t_datas *data)
{
	pthread_t	*thread;

	thread = (pthread_t *)malloc(sizeof(pthread_t) * data->nb);
	if (pthread_mutex_init(&data->death_mutex, NULL))
	{
		free(thread);
		thread = NULL;
	}
	if (pthread_mutex_init(&data->error_mutex, NULL))
	{
		free(thread);
		thread = NULL;
	}
	return (thread);
}

int	launch_threads(t_datas *data)
{
	int			i;
	static int	ret_status = SUCCESS;
	pthread_t	*thread;
	pthread_t	monitoring_thread;
	int			*ret;

	i = -1;
	thread = create_thread_tab_init_mutex(data);
	if (!thread)
		return (ERROR);
	while (!ret_status && ++i < data->nb)
		if (pthread_create(thread + i, NULL, &routine, data->philo + i))
			ret_status = ERROR;
	if (pthread_create(&monitoring_thread, NULL, &monitoring, data))
		ret_status = ERROR;
	while (--i >= 0)
		if (pthread_join(thread[i], (void **) &ret) || *ret == ERROR)
			ret_status = ERROR;
	if (pthread_join(monitoring_thread, NULL))
		ret_status = ERROR;
	if (thread)
		free(thread);
	return (ret_status);
}
