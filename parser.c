/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 02:03:45 by beergin           #+#    #+#             */
/*   Updated: 2025/08/05 13:47:50 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parser(t_token *tokens)
{
	t_command *cmd_head;
	t_command *current_cmd;
	t_token *token_iter;
	t_token *tmp_iter;
	t_token *cmd_start_token;
	t_command *new_cmd;
    t_redirect *new_red;
    int redirect_type;
	int arg_count;
	int i;

	cmd_head = NULL;
	current_cmd = NULL;
	token_iter = tokens;
	if (!tokens)
		return (NULL);
	while (token_iter)
	{
		new_cmd = (t_command *)malloc(sizeof(t_command));
		if (!new_cmd)
		{
			perror("malloc failed for command");
			free_commands(cmd_head);
			return (NULL);
		}
		new_cmd->redirects = NULL;
		new_cmd->next = NULL;
		new_cmd->heredoc_delimiter = NULL;
		new_cmd->heredoc_file = NULL;
		arg_count = 0;
		tmp_iter = token_iter;
		cmd_start_token = token_iter;
		arg_count = count_args(cmd_start_token);
		new_cmd->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
		if (!new_cmd->args)
		{
			perror("malloc failed for args");
			free_commands(cmd_head);
			free(new_cmd);
			return (NULL);
		}
		i = 0;
		while (token_iter && token_iter->type != TOKEN_PIPE)
		{
			if (token_iter->type == TOKEN_WORD)
			{
				new_cmd->args[i] = ft_strdup(token_iter->value);
				if (!new_cmd->args[i])
				{
					free_commands(cmd_head);
					free(new_cmd->args);
					free(new_cmd);
					return (NULL);
				}
				i++;
			}
			else if (token_iter->type == TOKEN_REDIRECT_IN
				|| token_iter->type == TOKEN_REDIRECT_OUT
				|| token_iter->type == TOKEN_REDIRECT_APPEND)
				{
					redirect_type = token_iter->type;
					token_iter = token_iter->next;
					if(!token_iter || token_iter->type != TOKEN_WORD)
					{
						if(token_iter)
						printf("minishell: syntax error near unexpected token  %s\n", token_iter->value);
						else
						printf("minishell: syntax error near unexpected token  %s\n", "newline");
						free(new_cmd->args);
						free(new_cmd);
						return (NULL);
					}
					new_red = create_redirection(redirect_type, &token_iter->value);
					if(!new_red)
					{
						free_commands(cmd_head);
						free(new_cmd->args);
						free(new_cmd);
						return (NULL);
					}
					add_redirection(&(new_cmd->redirects), new_red);
				}
			else if (token_iter->type == TOKEN_HERE_DOC)
			{
				// Heredoc için sadece token'ı atla, parse_heredoc handle edecek
				token_iter = token_iter->next;
				if(!token_iter || token_iter->type != TOKEN_WORD)
				{
					if(token_iter)
						printf("minishell: syntax error near unexpected token  %s\n", token_iter->value);
					else
						printf("minishell: syntax error near unexpected token  %s\n", "newline");
					free(new_cmd->args);
					free(new_cmd);
					return (NULL);
				}
				}
				token_iter = token_iter->next;
			}
			new_cmd->args[i] = NULL;
			parse_heredoc(new_cmd, cmd_start_token);
		if(!cmd_head)
			cmd_head = new_cmd;
        else
			current_cmd->next = new_cmd;
        current_cmd = new_cmd;
        if(token_iter && token_iter->type == TOKEN_PIPE)
        {
            token_iter = token_iter->next;
            if(!token_iter && cmd_head)
            {
                printf("minishell: syntax error near unexpected token `|'\n");
                free_commands(cmd_head);
                return (NULL);
            }
        }
	}
	(void)tmp_iter;
    return (cmd_head);
}
