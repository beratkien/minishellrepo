/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 23:50:49 by beergin           #+#    #+#             */
/*   Updated: 2024/08/14 22:57:45 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	const char	*h;
	const char	*n;
	size_t		i;

	if (!needle[0])
		return ((char *)haystack);
	while (*haystack && len > 0)
	{
		h = haystack;
		n = needle;
		i = 0;
		while (n[i] == h[i] && n[i] && len - i > 0)
			i++;
		if (n[i] == '\0')
			return ((char *) haystack);
		len--;
		haystack++;
	}
	return (NULL);
}
