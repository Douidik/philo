/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsuppan <jsuppan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:51:35 by jsuppan           #+#    #+#             */
/*   Updated: 2023/10/12 19:17:49 by jsuppan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

bool	ft_philo_init(t_philo *philo, int index, t_env *env)
{
	if (index < env->len)
		philo->fork[0] = &env->forks[index];
	if (env->len > 1)
		philo->fork[1] = &env->forks[(index + 1) % env->len];
	philo->id = index + 1;
	philo->meals = 0;
	philo->env = env;
	return (pthread_mutex_init(&philo->mutex, NULL) == 0);
}

void	ft_philo_destroy(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_destroy(&philo->mutex);
}

bool	ft_philo_done(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	pthread_mutex_lock(&philo->env->mutex);
	if (!philo->env->ok)
		philo->done = true;
	else if (philo->env->meals == -1)
		philo->done = false;
	else
		philo->done = philo->meals >= philo->env->meals;
	pthread_mutex_unlock(&philo->env->mutex);
	pthread_mutex_unlock(&philo->mutex);
	return (philo->done);
}

void	ft_philo_status(t_philo *philo, const char *status, bool death)
{
	pthread_mutex_lock(&philo->mutex);
	pthread_mutex_lock(&philo->env->mutex);
	if (philo->env->ok || death)
		printf("%d %d %s\n", ft_timestamp(), philo->id, status);
	pthread_mutex_unlock(&philo->env->mutex);
	pthread_mutex_unlock(&philo->mutex);
}

void	ft_philo_wait(t_philo *philo, int ms)
{
	int	wake_up;

	wake_up = ft_timestamp() + ms;
	while (ft_timestamp() < wake_up && !ft_philo_done(philo))
		usleep(50);
}
