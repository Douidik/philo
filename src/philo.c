#include "philo.h"
#include <stdio.h>
#include <unistd.h>

bool ft_philo_init(t_philo *philo, int index, t_env *env)
{
    pthread_mutex_t *swap_fork;

    if (index < env->len)
        philo->fork[0] = &env->forks[index];
    if (env->len > 1)
        philo->fork[1] = &env->forks[(index + 1) % env->len];
    if (index & 1 && env->len > 1) {
        swap_fork = philo->fork[0];
        philo->fork[0] = philo->fork[1];
        philo->fork[1] = swap_fork;
    }
    philo->id = index + 1;
    philo->meals = 0;
    philo->env = env;
    return (pthread_mutex_init(&philo->mutex, NULL) == 0);
}

void ft_philo_destroy(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex);
    if (philo->job == JOB_FORK_R || philo->job == JOB_EAT)
	pthread_mutex_unlock(philo->fork[0]);
    if (philo->job == JOB_EAT)
	pthread_mutex_unlock(philo->fork[1]);
    pthread_mutex_unlock(&philo->mutex);
    pthread_mutex_destroy(&philo->mutex);
}

bool ft_philo_done(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex);
    pthread_mutex_lock(&philo->env->mutex);
    /* philo->done = !philo->env->ok || (philo->meals != -1 && philo->meals >= philo->env->meals); */
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

void ft_job_done(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex);
    philo->job = (philo->job + 1) % JOB_END;
    pthread_mutex_unlock(&philo->mutex);
}

void ft_philo_wait(t_philo *philo, int ms)
{
    int wake_up;

    wake_up = ft_timestamp() + ms;
    while (ft_timestamp() < wake_up && !ft_philo_done(philo)) {
        usleep(50);
    }
}

void ft_philo_think(t_philo *philo)
{
    ft_philo_status(philo, "is thinking", false);
    ft_job_done(philo);
}

void ft_philo_take_fork(t_philo *philo, int n)
{
    pthread_mutex_t *fork;

    fork = philo->fork[n];
    if (fork != NULL) {
        pthread_mutex_lock(fork);
        ft_philo_status(philo, "has taken a fork", false);
        ft_job_done(philo);
    }
}

void ft_philo_eat(t_philo *philo, t_env *env)
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

void ft_philo_sleep(t_philo *philo, t_env *env)
{
    ft_philo_status(philo, "is sleeping", false);
    ft_philo_wait(philo, env->time_to_sleep);
    ft_job_done(philo);
}

void ft_philo_routine(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex);
    philo->job = JOB_FORK_L;
    pthread_mutex_unlock(&philo->mutex);
    if (philo->id & 1 && philo->env->len > 1) {
        ft_philo_wait(philo, philo->env->time_to_eat);
    }
    while (!ft_philo_done(philo)) {
        if (philo->job == JOB_FORK_L)
            ft_philo_take_fork(philo, 0);
        else if (philo->job == JOB_FORK_R)
            ft_philo_take_fork(philo, 1);
        else if (philo->job == JOB_EAT)
            ft_philo_eat(philo, philo->env);
        else if (philo->job == JOB_SLEEP)
            ft_philo_sleep(philo, philo->env);
        else if (philo->job == JOB_THINK)
            ft_philo_think(philo);
        else
            ft_philo_status(philo, "invalid state", true);
    }
    ft_philo_destroy(philo);
}

void ft_philo_status(t_philo *philo, const char *status, bool death)
{
    pthread_mutex_lock(&philo->mutex);
    pthread_mutex_lock(&philo->env->mutex);
    if (philo->env->ok || death) {
        printf("%d %d %s\n", ft_timestamp(), philo->id, status);
    }
    pthread_mutex_unlock(&philo->env->mutex);
    pthread_mutex_unlock(&philo->mutex);
}
