NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
BUILTINS_DIR = $(SRC_DIR)/builtins
UTILS_DIR = $(SRC_DIR)/utils
EXEC_DIR = $(SRC_DIR)/exec
PARSING_DIR = $(SRC_DIR)/parsing
TOKEN_DIR = $(SRC_DIR)/tokenizer
LIBFT_DIR = $(SRC_DIR)/usefull

OBJ_DIR = obj
INCLUDES_DIR = includes

LIBS = -lreadline

LIBFT = \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_itoa.c \
	ft_lstadd_back.c \
	ft_lstclear.c \
	ft_putstr_fd.c \
	ft_split.c \
	ft_strchr.c \
	ft_strcmp.c \
	ft_strcpy.c \
	ft_strdup_2.c \
	ft_strdup.c \
	ft_strjoin.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strncmp.c \
	ft_strncpy.c \
	ft_substr.c \

SRC_BUILTINS = \
	builtin_cd_utils.c \
	builtin_cd.c \
	builtin_env.c \
	builtin_export_utils.c \
	builtin_export.c \
	builtin.c \
	echo.c \

SRC_UTILS = \
	add_history.c \
	clean.c \
	debug.c \
	env_utils.c \
	env.c \
	error_handler.c \
	expand.c \
	free_command.c \
	ft_lst_add_back_command.c \
	handle_exit_var.c \
	is_char_token.c \
	print_syntaxe_error.c \
	token_utils.c \

SRC_EXEC = \
	command_validation.c \
	entry_point.c \
	exec.c \
	fork.c \
	heredoc_child.c \
	heredoc.c \
	pipeline.c \
	redirect.c \

SRC_PARSING = \
	expand_var_utils.c \
	expand_var.c \
	process_pre_parsing_bis.c \
	process_pre_parsing.c \
	process_var.c \

SRC_TOKENIZER = \
	create_command.c \
	handler_token_2.c \
	handler_token.c \
	token_check_syntaxe.c \
	tokenizer.c \
	type_token.c \

SRC = \
	main.c \

SRC_ALL = \
	$(addprefix $(SRC_DIR)/, $(SRC)) \
	$(addprefix $(BUILTINS_DIR)/, $(SRC_BUILTINS)) \
	$(addprefix $(UTILS_DIR)/, $(SRC_UTILS)) \
	$(addprefix $(EXEC_DIR)/, $(SRC_EXEC)) \
	$(addprefix $(PARSING_DIR)/, $(SRC_PARSING)) \
	$(addprefix $(TOKEN_DIR)/, $(SRC_TOKENIZER)) \
	$(addprefix $(LIBFT_DIR)/, $(LIBFT)) \

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o, $(SRC_ALL))

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME) 

$(NAME): $(OBJS) $(INCLUDES_DIR)/minishell.h
	@echo "$(YELLOW)🔧 Compilation de $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(GREEN)✅ Compilé avec succès !$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)🧹 Fichiers objets supprimés.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🧼 Exécutable supprimé.$(RESET)"

re: fclean all

.PHONY: all clean fclean re