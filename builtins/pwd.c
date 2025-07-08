/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:49:19 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/02 21:38:34 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_pwd(void)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_memset(cwd, 0, sizeof(cwd));
}
int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
}

int execute_builtin(t_command *cmd, t_shell *shell)
{
	char *command_name;

	command_name = cmd->args[0];
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (builtin_pwd());
	return (0);
}
