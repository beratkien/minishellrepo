/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: md <md@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:16:16 by beergin           #+#    #+#             */
/*   Updated: 2025/08/03 08:19:13 by md               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(char *var_name, t_shell *shell)
{
	int		i;
	size_t	len;
	char	*exit_code_str;

	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_code_str = ft_itoa(shell->last_exit_code);
		return (exit_code_str);
	}
	i = 0;
	len = ft_strlen(var_name);
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var_name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return ("");
}

static char	*append_str(char *dest, const char *src)
{
	size_t	len_dest;
	size_t	len_src;
	char	*new_str;

	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	new_str = malloc(len_dest + len_src + 1);
	if (!new_str)
	{
		free(dest);
		return (NULL);
	}
	ft_memcpy(new_str, dest, len_dest);
	ft_memcpy(new_str + len_dest, src, len_src);
	new_str[len_dest + len_src] = '\0';
	free(dest);
	return (new_str);
}

static char	*expand_string(char *str, t_shell *shell)
{
	int		i;
	int		start;
	char	*result;
	char	*var_name;
	char	*env_value;
	char	buf[2];
	int		is_special_var;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		is_special_var = 0;
		if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
		{
			i++;
			start = i;
			if (str[i] == '?')
			{
				i++;
				is_special_var = 1;
			}
			else
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
			var_name = ft_substr(str, start, i - start);
			if (!var_name)
				return (free(result), NULL);
			env_value = get_env_value(var_name, shell);
			result = append_str(result, env_value);
			if (is_special_var)
				free(env_value);
			free(var_name);
			if (!result)
				return (NULL);
		}
		else
		{
			buf[0] = str[i];
			buf[1] = '\0';
			result = append_str(result, buf);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}

void	expand_variables(t_token *tokens, t_shell *shell)
{
	t_token	*tmp;
	char	*expanded;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && ft_strchr(tmp->value, '$'))
		{
			if (tmp->quote_type == 1)
			{
				tmp = tmp->next;
				continue ;
			}
			expanded = expand_string(tmp->value, shell);
			if (!expanded)
			{
				perror("expand_variables malloc error");
				exit(1);
			}
			free(tmp->value);
			tmp->value = expanded;
		}
		tmp = tmp->next;
	}
}
