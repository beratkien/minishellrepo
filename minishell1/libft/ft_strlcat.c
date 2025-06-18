/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 23:46:10 by beergin           #+#    #+#             */
/*   Updated: 2024/08/14 22:20:06 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	leng;
	size_t	i;

	i = 0;
	leng = ft_strlen(dest);
	if (size - 1 > leng && size > 0)
	{
		while (src[i] && size - 1 > leng + i)
		{
			dest[i + leng] = src[i];
			i++;
		}
		dest[leng + i] = 0;
	}
	if (leng >= size)
		leng = size;
	return (leng + ft_strlen(src));
}
