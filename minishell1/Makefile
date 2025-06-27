NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAGS = -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include

SRCS = main.c utils.c executor.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR) all
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean
fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
re: fclean all


.PHONY: all clean fclean re
