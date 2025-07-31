/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: md <md@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:23:12 by md                #+#    #+#             */
/*   Updated: 2025/07/28 17:40:42 by md               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline_flag;

	i = 1;
	newline_flag = 1;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 3) == 0)
	{
		newline_flag = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
