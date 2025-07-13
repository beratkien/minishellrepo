/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:50:37 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/13 23:51:29 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int builtin_exit(t_command *cmd, t_shell *shell)
{
	int exit_code;

	ft_putendl_fd("exit", 2);
	if (shell->envp)
		free_envp(shell->envp);
	if (!cmd->args[1])
	{
		free_commands(shell->command);
		free_tokens(shell->token);
		exit(shell->last_exit_code);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("exit", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_commands(shell->command);
		free_tokens(shell->token);
		exit(255);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	free_commands(shell->command);
	free_tokens(shell->token);
	exit_code = ft_atoi(cmd->args[1]);
	exit(exit_code % 256);
}
