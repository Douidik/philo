#include "philo.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    t_env env;

    if (!ft_env_parse(&env, argc, argv))
        return (1);
    if (!ft_env_init(&env))
        return (ft_env_destroy(&env), 1);
    if (!ft_simulate(&env))
        return (ft_env_destroy(&env), 1);
    ft_env_destroy(&env);
    return (0);
}
