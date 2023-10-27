/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsuppan <jsuppan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:53:00 by jsuppan           #+#    #+#             */
/*   Updated: 2023/10/12 15:55:53 by jsuppan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>

typedef enum e_job
{
	JOB_THINK,
	JOB_FORK_L,
	JOB_FORK_R,
	JOB_EAT,
	JOB_SLEEP,
	JOB_END
}	t_job;

typedef struct s_philo
{
	int				id;
	t_job			job;
	bool			done;
	int				meals;
	int				last_meal;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*fork[2];
	struct s_env	*env;
}	t_philo;

typedef struct s_env
{
	pthread_mutex_t	mutex;
	pthread_mutex_t	*forks;
	bool			ok;
	int				len;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals;
}	t_env;

bool	ft_philo_init(t_philo *philo, int id, t_env *env);
void	ft_philo_destroy(t_philo *philo);
void	ft_philo_job(t_philo *philo);
void	ft_philo_status(t_philo *philo, const char *status, bool death);
bool	ft_philo_done(t_philo *philo);
void	ft_philo_wait(t_philo *philo, int ms);

bool	ft_env_init(t_env *env);
bool	ft_env_parse(t_env *env, int argc, char *argv[]);
void	ft_env_destroy(t_env *env);
bool	ft_simulate(t_env *env);

bool	ft_error(const char *message);
int		ft_timestamp(void);
int		ft_strlen(const char *s);

#endif
