NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
BUILTINS_DIR = $(SRC_DIR)/builtins
UTILS_DIR = $(SRC_DIR)/utils
EXEC_DIR = $(SRC_DIR)/exec

LIBFT_DIR = usefull
OBJ_DIR = obj
INCLUDES_DIR = includes

LIBS = -lreadline

LIBFT = \
	ft_lstadd_back.c \
	ft_strdup.c \
	ft_itoa.c \
	ft_strlen.c \
	ft_lstclear.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_strncpy.c \
	ft_isdigit.c \
	ft_strjoin.c \
	is_char_token.c \
	ft_strcmp.c \
	ft_split.c \
	ft_strcpy.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_strchr.c \
	ft_strncmp.c \
	ft_putstr_fd.c \
	ft_strdup_2.c \
	ft_substr.c \
	print_syntaxe_error.c \
	hangle_exit_var.c \

SRC_BUILTINS = \
	m_builtin_cd_utils.c \
	m_builtin_cd.c \
	m_builtin_env.c \
	m_builtin_export_utils.c \
	m_builtin_export.c \
	m_builtin.c \
	m_echo.c \

SRC_UTILS = \
	m_clean.c \
	m_debug.c \
	m_env_utils.c \
	m_env.c \
	m_expand.c \
	m_error_handler.c \

SRC_EXEC = \
	m_command_validation.c \
	m_exec.c \
	m_fork.c \
	m_heredoc.c \
	m_pipeline.c \
	m_redirect.c \


SRC = \
	main.c \
	tokenizer.c \
	type_token.c \
	token_check_syntaxe.c \
	test_parsing.c \
	create_command.c \
	free_command.c \
	ft_lst_add_back_command.c \
	handler_token.c \
	handler_token_2.c \
	process_pre_parsing.c \
	process_pre_parsing_bis.c \
	expand_var.c \
	process_var.c \
	expand_var_utils.c \
	add_history.c \


SRC_ALL = \
	$(addprefix $(SRC_DIR)/, $(SRC)) \
	$(addprefix $(BUILTINS_DIR)/, $(SRC_BUILTINS)) \
	$(addprefix $(UTILS_DIR)/, $(SRC_UTILS)) \
	$(addprefix $(EXEC_DIR)/, $(SRC_EXEC)) \
	$(addprefix $(LIBFT_DIR)/, $(LIBFT)) \

OBJ_ALL = \
	$(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC_ALL)))

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ_ALL)
	@echo "$(YELLOW)🔧 Compilation de $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ_ALL) -o $(NAME) $(LIBS)
	@echo "$(GREEN)✅ Compilé avec succès !$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(BUILTINS_DIR)/%.c
	@$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	@$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(EXEC_DIR)/%.c
	@$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIBFT_DIR)/%.c
	@$(CC) $(CFLAGS) -I $(INCLUDES_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)🧹 Fichiers objets supprimés.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🧼 Exécutable supprimé.$(RESET)"

re: fclean all

.PHONY: all clean fclean re