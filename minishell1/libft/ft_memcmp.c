/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 23:48:31 by beergin           #+#    #+#             */
/*   Updated: 2024/10/13 18:57:58 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*s1_;
	const unsigned char	*s2_;
	size_t				index;

	index = 0;
	s1_ = (const unsigned char *)s1;
	s2_ = (const unsigned char *)s2;
	while (index < n)
	{
		if (s1_[index] != s2_[index])
			return (s1_[index] - s2_[index]);
		index++;
	}
	return (0);
}
