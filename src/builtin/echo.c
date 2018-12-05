/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 18:40:28 by mpetruno          #+#    #+#             */
/*   Updated: 2018/11/28 18:40:29 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **av)
{
	while (*(++av))
	{
		if (*(av + 1))
			ft_printf("%s ", *av);
		else
			ft_printf("%s\n", *av);
	}
	return (1);
}
