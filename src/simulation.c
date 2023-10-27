/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsuppan <jsuppan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:40:47 by jsuppan           #+#    #+#             */
/*   Updated: 2023/10/12 18:08:37 by jsuppan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_philos_free(t_philo *philos, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		pthread_join(philos[i].thread, NULL);
		ft_philo_destroy(&philos[i]);
		i++;
	}
	free(philos);
}

// Watch the philosophers, returns if the simulation should continue
// It stops when a philosopher starved or when every philosophers has been fed
bool	ft_watch_philos(t_philo *philos, t_env *env)
{
	int		i;
	int		meal_elasped;
	bool	done;

	done = true;
	i = 0;
	while (i < env->len)
	{
		pthread_mutex_lock(&philos[i].mutex);
		done &= philos[i].done;
		meal_elasped = ft_timestamp() - philos[i].last_meal;
		pthread_mutex_unlock(&philos[i].mutex);
		if (meal_elasped >= env->time_to_die)
		{
			ft_philo_status(&philos[i], "died", true);
			pthread_mutex_lock(&env->mutex);
			env->ok = false;
			pthread_mutex_unlock(&env->mutex);
			return (false);
		}
		i++;
	}
	return (!done);
}

bool	ft_start_threads(t_philo *philos, t_env *env)
{
	int		start;
	int		i;
	t_philo	*ph;

	start = ft_timestamp();
	i = 0;
	while (i < env->len)
	{
		ph = &philos[i];
		ph->done = false;
		ph->last_meal = start;
		if (pthread_create(&ph->thread, NULL, (void *)ft_philo_job, ph) < 0)
		{
			ft_philos_free(philos, env->len);
			return (ft_error("cannot start threads"));
		}
		i++;
	}
	return (true);
}

bool	ft_simulate(t_env *env)
{
	t_philo	*philos;
	int		i;

	env->ok = true;
	philos = malloc(sizeof(t_philo) * env->len);
	if (!philos)
		return (ft_error("cannot malloc philosophers"));
	i = 0;
	while (i < env->len)
	{
		if (!ft_philo_init(&philos[i], i, env))
		{
			ft_philos_free(philos, i);
			return (ft_error("cannot init philosophers"));
		}
		i++;
	}
	if (!ft_start_threads(philos, env))
		return (false);
	while (ft_watch_philos(philos, env))
		;
	return (ft_philos_free(philos, env->len), true);
}
