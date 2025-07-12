/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:55:09 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/11 23:16:44 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int execute_builtin(t_command *cmd, t_shell *shell)
{
	(void)shell;
	char *command_name;

	command_name = cmd->args[0];
	if (ft_strncmp(command_name, "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(command_name, "env", 4) == 0)
		return (builtin_env(shell));
	if (ft_strncmp(command_name, "cd", 3) == 0)
		return (builtin_cd(cmd, shell));
	if (ft_strncmp(command_name, "exit", 5) == 0)
		return (builtin_exit(cmd, shell));
	return (0);
}
