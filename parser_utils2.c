/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:43:57 by md                #+#    #+#             */
/*   Updated: 2025/08/05 02:11:44 by mdonmeze         ###   ########.fr       */
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
	// Token tipini redirect tipine çevir
	if (type == TOKEN_REDIRECT_IN)
		new_red->type = REDIR_IN;
	else if (type == TOKEN_REDIRECT_OUT)
		new_red->type = REDIR_OUT;
	else if (type == TOKEN_REDIRECT_APPEND)
		new_red->type = REDIR_APPEND;
	else if (type == TOKEN_HERE_DOC)
		new_red->type = REDIR_HEREDOC;
	else
		new_red->type = REDIR_IN; // varsayılan

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
		if (cmd_tmp->heredoc_delimiter)
			free(cmd_tmp->heredoc_delimiter);
		if (cmd_tmp->heredoc_file)
			free(cmd_tmp->heredoc_file);
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
