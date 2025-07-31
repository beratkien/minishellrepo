/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: md <md@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:51:00 by md                #+#    #+#             */
/*   Updated: 2025/07/28 20:51:15 by md               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var_inplace(char **envp, char *var_name)
{
	int		i;
	int		j;
	int		var_len;

	if (!envp || !var_name)
		return ;
	var_len = ft_strlen(var_name);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_len) == 0 && envp[i][var_len] == '=')
			free(envp[i]);
		else
		{
			if (i != j)
				envp[j] = envp[i];
			j++;
		}
		i++;
	}
	envp[j] = NULL;
}

int	builtin_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	ret_status;

	if (!cmd->args[1])
		return (0);
	ret_status = 0;
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret_status = 1;
		}
		else
			remove_env_var_inplace(shell->envp, cmd->args[i]);
		i++;
	}
	return (ret_status);
}
