/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:43:57 by md                #+#    #+#             */
/*   Updated: 2025/07/12 20:19:30 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*create_redirection(int type, char **file)
{
	t_redirect	*new_red;

	new_red = (t_redirect *)malloc(sizeof(t_redirect));
	if (!new_red)
	{
		perror("malloc failed for redirection");
		return (NULL);
	}
	new_red->type = type;
	new_red->filename = ft_strdup(*file);
	if (!new_red->filename)
	{
		free(new_red);
		perror("malloc failed for redirection file");
		return (NULL);
	}
	new_red->next = NULL;
	return(new_red);
}

void	add_redirection(t_redirect **head, t_redirect *new_redirection)
{
	t_redirect	*current;

	if (!*head)
		*head = new_redirection;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_redirection;
	}
}

void	free_redirections(t_redirect *head)
{
	t_redirect	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
	}
}

void	free_commands(t_command *head)
{
	t_command	*cmd_tmp;
	int			i;

	while (head)
	{
		cmd_tmp = head;
		head = head->next;
		if (cmd_tmp->args)
		{
			i = 0;
			while (cmd_tmp->args[i])
				free(cmd_tmp->args[i++]);
			free(cmd_tmp->args);
		}
		if (cmd_tmp->redirects)
			free_redirections(cmd_tmp->redirects);
		free(cmd_tmp);
	}
	head = NULL;
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
