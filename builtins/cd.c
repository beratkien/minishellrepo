/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:04:17 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/13 23:20:04 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_env_value(t_shell *shell, const char *key)
{
     int	i;
    int	key_len;

    if (!shell->envp || !key)
        return (NULL);
    key_len = ft_strlen(key);
    i = 0;
    while (shell->envp[i])
    {
        if (ft_strncmp(shell->envp[i], key, key_len) == 0
            && shell->envp[i][key_len] == '=')
            return (shell->envp[i] + key_len + 1);
        i++;
    }
    return (NULL);
}

static int	update_env_var(t_shell *shell, const char *key, const char *value)
{
    char	*new_var;
    char	*temp;
    int		i;
    int		key_len;

    if (!key || !value)
        return (1);
    key_len = ft_strlen(key);
    i = 0;
    while (shell->envp[i])
    {
        if (ft_strncmp(shell->envp[i], key, key_len) == 0
            && shell->envp[i][key_len] == '=')
        {
            temp = ft_strjoin(key, "=");
            if (!temp)
                return (1);
            new_var = ft_strjoin(temp, value);
            free(temp);
            if (!new_var)
                return (1);
            free(shell->envp[i]);
            shell->envp[i] = new_var;
            return (0);
        }
        i++;
    }
    return (1);
}

int builtin_cd(t_command *cmd, t_shell *shell)
{
    char	*path;
    char	*old_pwd;
    char	*new_pwd;

    if (cmd->args[1] && cmd->args[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        return (1);
    }

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return (1);

    if (!cmd->args[1])
    {
        path = get_env_value(shell, "HOME");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            free(old_pwd);
            return (1);
        }
    }
    // else if (ft_strncmp(cmd->args[1], "~", 2) == 0)
    // {
    //     path = get_env_value(shell, "HOME");
    //      if (!path)
    //     {
    //         ft_putstr_fd("minishell: cd: HOME not set\n", 2);
    //         free(old_pwd);
    //         return (1);
    //     }
    // }
    else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
    {
        path = get_env_value(shell, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            free(old_pwd);
            return (1);
        }
        ft_putstr_fd(path, 1);
        ft_putstr_fd("\n", 1);
    }
    else
        path = cmd->args[1];

    if (chdir(path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        perror(path);
        free(old_pwd);
        return (1);
    }

    new_pwd = getcwd(NULL, 0);
    if (new_pwd)
    {
        update_env_var(shell, "OLDPWD", old_pwd);
        update_env_var(shell, "PWD", new_pwd);
        free(new_pwd);
    }
    free(old_pwd);
    return (0);
}
