/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsuppan <jsuppan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:39:27 by jsuppan           #+#    #+#             */
/*   Updated: 2023/10/12 15:40:42 by jsuppan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>

int	ft_strlen(const char *s)
{
	int	size;

	size = 0;
	while (s[size] != 0)
		size++;
	return (size);
}

uint64_t	gettimeofday_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Get the timestamp since the first timestamp call
// With 32 bits integers, the max timestamp is about 3 hours
int	ft_timestamp(void)
{
	static uint64_t	start = -1;

	if (start == (uint64_t)-1)
		start = gettimeofday_ms();
	return ((int)(gettimeofday_ms() - start));
}

bool	ft_error(const char *message)
{
	write(2, "error: ", ft_strlen("error: "));
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	return (false);
}
