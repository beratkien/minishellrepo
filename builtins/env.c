/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:54:11 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/09 19:49:15 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_env(t_shell *shell)
{
	int	i;

	if (!shell || !shell->envp)
		return (1);

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}
