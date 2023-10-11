#include "philo.h"
#include <limits.h>
#include <memory.h>

bool ft_atoi(int *output, char *s)
{
    int number;
    int digit;

    if (*s < '0' || *s > '9')
        return (false);
    number = 0;
    while (*s >= '0' && *s <= '9') {
        digit = *s - '0';
        if (number > (INT_MAX - digit) / 10)
            return (false);
        number = number * 10 + digit;
        s++;
    }
    if (*s != '\0')
        return (false);
    *output = number;
    return (true);
}

bool ft_env_parse(t_env *env, int argc, char *argv[])
{
    if (argc < 5)
        return (ft_error("not enough arguments provided"));
    if (!ft_atoi(&env->len, argv[1]) || !ft_atoi(&env->time_to_die, argv[2]) || !ft_atoi(&env->time_to_eat, argv[3]) ||
        !ft_atoi(&env->time_to_sleep, argv[4])) {
        return (ft_error("cannot convert arguments to integers (format: [0-9]+)"));
    }
    if (argc < 6)
        env->meals = -1;
    else if (!ft_atoi(&env->meals, argv[5]))
        return (ft_error("cannot convert arguments to integers (format: [0-9]+)"));
    return (true);
}

bool ft_env_init(t_env *env)
{
    int i;

    env->forks = malloc(sizeof(pthread_mutex_t) * env->len);
    if (!env->forks)
        return (ft_error("cannot malloc env mutex"));
    if (pthread_mutex_init(&env->mutex, NULL) < 0) {
        free(env->forks);
        return (ft_error("cannot init env mutex"));
    }
    i = 0;
    while (i < env->len) {
        if (pthread_mutex_init(&env->forks[i], NULL) < 0) {
            while (--i >= 0) {
                pthread_mutex_destroy(&env->forks[i]);
            }
            free(env->forks);
            return (ft_error("cannot init fork mutex"));
        }
        i++;
    }
    return (true);
}

void ft_env_destroy(t_env *env)
{
    int i;

    i = 0;
    while (i < env->len) {
        pthread_mutex_destroy(&env->forks[i]);
        i++;
    }
    free(env->forks);
    pthread_mutex_destroy(&env->mutex);
}
