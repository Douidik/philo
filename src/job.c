/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsuppan <jsuppan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:48:33 by jsuppan           #+#    #+#             */
/*   Updated: 2023/10/13 15:16:34 by jsuppan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_job_done(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	philo->job = (philo->job + 1) % JOB_END;
	pthread_mutex_unlock(&philo->mutex);
}

void	ft_philo_take_fork(t_philo *philo, int n)
{
	pthread_mutex_t	*fork;

	fork = philo->fork[n];
	if (fork != NULL)
	{
		pthread_mutex_lock(fork);
		ft_philo_status(philo, "has taken a fork", false);
		ft_job_done(philo);
	}
}

void	ft_philo_eat(t_philo *philo, t_env *env)
{
	ft_philo_status(philo, "is eating", false);
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal = ft_timestamp();
	pthread_mutex_unlock(&philo->mutex);
	ft_philo_wait(philo, env->time_to_eat);
	pthread_mutex_lock(&philo->mutex);
	philo->meals++;
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(philo->fork[0]);
	pthread_mutex_unlock(philo->fork[1]);
	ft_job_done(philo);
}

void	ft_philo_sleep(t_philo *philo, t_env *env)
{
	ft_philo_status(philo, "is sleeping", false);
	ft_philo_wait(philo, env->time_to_sleep);
	ft_job_done(philo);
}

void	ft_philo_job(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	philo->job = JOB_FORK_L;
	pthread_mutex_unlock(&philo->mutex);
	if (philo->id & 1 && philo->env->len > 1)
		ft_philo_wait(philo, philo->env->time_to_eat / 2);
	while (!ft_philo_done(philo))
	{
		if (philo->job == JOB_FORK_L)
			ft_philo_take_fork(philo, 0);
		else if (philo->job == JOB_FORK_R)
			ft_philo_take_fork(philo, 1);
		else if (philo->job == JOB_EAT)
			ft_philo_eat(philo, philo->env);
		else if (philo->job == JOB_SLEEP)
			ft_philo_sleep(philo, philo->env);
		else if (philo->job == JOB_THINK)
		{
			ft_philo_status(philo, "is thinking", false);
			ft_job_done(philo);
		}
	}
	if ((philo->job == JOB_FORK_R || philo->job == JOB_EAT) && (philo->fork[0]))
		pthread_mutex_unlock(philo->fork[0]);
	if ((philo->job == JOB_EAT && philo->fork[1]))
		pthread_mutex_unlock(philo->fork[1]);
}
