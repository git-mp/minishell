/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 17:22:28 by mpetruno          #+#    #+#             */
/*   Updated: 2018/11/05 18:22:49 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// modify to use char *ptr instead index and avoid type cast at each cycle

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(char *)(dst + i) = *(char *)(src + i);
		i++;
	}
	return (dst);
}