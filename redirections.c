/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: md <md@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:32:44 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/08/01 00:25:33 by md               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redirections(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;

	if (!cmd->redirects)
		return(0);
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_HEREDOC)
		{
			// Heredoc için şimdilik basit implementasyon
			// TODO: Gerçek heredoc implementasyonu eklenecek
			redir = redir->next;
			continue;
		}
		else
		{
			redir = redir->next;
			continue;
		}
		if (fd == -1)
		{
			perror("minishell");
			return (-1);
		}
		if (redir->type == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
