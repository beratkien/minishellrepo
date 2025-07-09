/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdonmeze <mdonmeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:56:45 by mdonmeze          #+#    #+#             */
/*   Updated: 2025/07/09 10:41:47 by mdonmeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TOKEN_WORD 1
# define TOKEN_PIPE 2
# define TOKEN_REDIRECT_IN 3
# define TOKEN_REDIRECT_OUT 4
# define TOKEN_REDIRECT_APPEND 5
# define TOKEN_HERE_DOC 6

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}					t_token;

typedef enum e_redir_type
{
	REDIR_IN,		// <
	REDIR_OUT,		// >
	REDIR_APPEND,	// >>
	REDIR_HEREDOC	// <<
}	t_redir_type;

typedef struct s_redirect
{
	char				*filename;
	t_redir_type		type;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
}	t_command;
//yeni shellin genel durumunu tutan ana yapı!!!!.
typedef struct s_shell
{
	char	**envp;
	int		last_exit_code;
}	t_shell;

extern int g_last_exit_status;
t_token				*lexer(char *line);
int					is_whitespace(char c);
int					is_metachar(char c);
t_token				*create_token(char *value, int type);
void				add_token(t_token **head, t_token *new_token);
void				free_tokens(t_token *head);
void				execute_pipeline(t_command *pipeline, t_shell *shell);
t_redirect				*create_redirection(int type, char **file);
void					add_redirection(t_redirect **head,
							t_redirect *new_redirection);
void					free_redirections(t_redirect *head);
void					free_commands(t_command *head);
int						count_args(t_token *tokens);
char			*get_command_path(char *cmd, t_shell *shell);
t_command	*parser(t_token *tokens);
int builtin_pwd(void);
int is_builtin(char *cmd);
int execute_builtin(t_command *cmd, t_shell *shell);


#endif
