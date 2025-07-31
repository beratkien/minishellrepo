/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beergin <beergin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:16:16 by beergin           #+#    #+#             */
/*   Updated: 2025/08/01 02:21:03 by beergin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(char *var_name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
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

static char	*expand_string(char *str, char **envp)
{
	int		i;
	int		start;
	char	*result;
	char	*var_name;
	char	*env_value;
	char	buf[2];

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1]) || str[i
				+ 1] == '_'))
		{
			i++;
			start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			var_name = ft_substr(str, start, i - start);
			if (!var_name)
				return (free(result), NULL);
			env_value = get_env_value(var_name, envp);
			result = append_str(result, env_value);
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
	t_token *tmp = tokens;
	char *expanded;

	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && ft_strchr(tmp->value, '$'))
		{
			if (tmp->quote_type == 1)
			{
				tmp = tmp->next;
				continue;
			}
			expanded = expand_string(tmp->value, shell->envp);
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