/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsuppan <jsuppan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:48:10 by jsuppan           #+#    #+#             */
/*   Updated: 2023/10/12 15:48:29 by jsuppan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	main(int argc, char *argv[])
{
	t_env	env;

	if (!ft_env_parse(&env, argc, argv))
		return (1);
	if (!ft_env_init(&env))
		return (ft_env_destroy(&env), 1);
	if (!ft_simulate(&env))
		return (ft_env_destroy(&env), 1);
	ft_env_destroy(&env);
	return (0);
}
