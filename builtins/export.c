/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: md <md@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:54:12 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/31 18:15:57 by md               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Environment değişkenlerini listele
static void	print_exported_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		ft_putstr_fd(envp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}

// Geçerli değişken ismi kontrolü (VAR=value formatında)
static int	is_valid_export_arg(char *arg)
{
	int	i;

	if (!arg || !arg[0])
		return (0);
	// İlk karakter alfabe veya _ olmalı
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	// = işaretine kadar kontrol et
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Environment'ta değişkeni bul (index döndür, yoksa -1)
static int	find_env_var(char **envp, char *var_name, int var_len)
{
	int	i;

	if (!envp || !var_name)
		return (-1);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_len) == 0 && envp[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

// Environment array'inin boyutunu hesapla
static int	get_env_size(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

// Yeni environment array oluştur (eski + 1 yeni)
static char	**create_new_env(char **old_envp, int size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_envp[i] = old_envp[i];
		i++;
	}
	return (new_envp);
}

// Environment'a değişken ekle veya güncelle
static int	add_to_env(char ***envp, char *arg)
{
	int		var_len;
	int		env_index;
	int		env_size;
	char	**new_envp;

	var_len = 0;
	while (arg[var_len] && arg[var_len] != '=')
		var_len++;
	env_index = find_env_var(*envp, arg, var_len);
	if (env_index >= 0)
	{
		free((*envp)[env_index]);
		(*envp)[env_index] = ft_strdup(arg);
		if ((*envp)[env_index])
			return (0);
		return (1);
	}
	env_size = get_env_size(*envp);
	new_envp = create_new_env(*envp, env_size);
	if (!new_envp)
		return (1);
	new_envp[env_size] = ft_strdup(arg);
	new_envp[env_size + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	if (new_envp[env_size])
		return (0);
	return (1);
}

int	builtin_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	ret_status;

	// Eğer argüman yoksa (sadece "export"), tüm değişkenleri listele
	if (!cmd->args[1])
	{
		print_exported_vars(shell->envp);
		return (0);
	}

	// Argümanları işle
	ret_status = 0;
	i = 1;
	while (cmd->args[i])
	{
		// Geçerli format kontrolü
		if (!is_valid_export_arg(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret_status = 1;
		}
		else
		{
			// Environment'a ekle/güncelle
			if (add_to_env(&shell->envp, cmd->args[i]) != 0)
				ret_status = 1;
		}
		i++;
	}
	return (ret_status);
}
