<div align="center">

# Minishell

**A minimal Unix shell implemented in C, inspired by bash**

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Norm](https://img.shields.io/badge/norm-42-brightgreen.svg)](https://github.com/42School/norminette)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

</div>

---

## 📖 Description

**Minishell** is a minimal Unix shell written in C, inspired by **bash**. It replicates the core behaviour of a POSIX shell — interactive prompt, command execution, pipes, redirections, environment variable management and signal handling — while adhering to the [42 School norm](https://github.com/42School/norminette).

The project is a deep dive into how a real shell works under the hood: process creation with `fork`/`exec`, inter-process communication through pipes, file-descriptor manipulation for redirections, and robust signal handling.

---

## ✨ Features

| Feature | Description |
|---|---|
| **Interactive prompt** | Readline-powered prompt with command history |
| **Command execution** | Execute any binary found in `$PATH` |
| **Pipes** | Chain multiple commands with `\|` (unlimited depth) |
| **Redirections** | `>` output, `>>` append, `<` input, `<<` heredoc |
| **Environment variables** | Full `$VAR` expansion, including `$?` for last exit code |
| **Quote handling** | Single quotes (no expansion) and double quotes (partial expansion) |
| **Signal handling** | `Ctrl+C` (SIGINT), `Ctrl+D` (EOF), `Ctrl+\` (SIGQUIT) behave like bash |
| **Built-in commands** | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| **Heredoc** | `<<` delimiter support with variable expansion |
| **Error messages** | Descriptive error output consistent with bash |

---

## 🔧 Prerequisites

| Requirement | Version |
|---|---|
| **OS** | Linux / macOS |
| **Compiler** | `gcc` or `clang` |
| **GNU Make** | ≥ 3.81 |
| **readline** | Development headers required |

### Install readline

```bash
# Debian / Ubuntu
sudo apt-get install libreadline-dev

# macOS (Homebrew)
brew install readline
```

---

## 🚀 Installation

```bash
# 1. Clone the repository
git clone https://github.com/enzodobele/Minishell.git
cd Minishell

# 2. Build the project
make

# 3. Run minishell
./minishell
```

---

## 💻 Usage

Once launched, minishell presents an interactive prompt:

```
minishell$ _
```

### Basic command execution

```bash
minishell$ ls -la
minishell$ echo "Hello, World!"
minishell$ /usr/bin/cat /etc/hostname
```

### Pipes

```bash
minishell$ ls -la | grep ".c" | wc -l
minishell$ cat file.txt | sort | uniq
```

### Redirections

```bash
# Output redirection (overwrite)
minishell$ echo "hello" > output.txt

# Output redirection (append)
minishell$ echo "world" >> output.txt

# Input redirection
minishell$ cat < input.txt

# Heredoc
minishell$ cat << EOF
> line 1
> line 2
> EOF
```

### Environment variables

```bash
minishell$ echo $HOME
minishell$ echo $PATH
minishell$ echo $?          # last exit code
minishell$ export MY_VAR=42
minishell$ echo $MY_VAR
minishell$ unset MY_VAR
```

### Built-in commands

```bash
minishell$ cd /tmp          # change directory
minishell$ pwd              # print working directory
minishell$ env              # list environment variables
minishell$ export FOO=bar   # set / export a variable
minishell$ unset FOO        # remove a variable
minishell$ echo -n "hello"  # echo without newline
minishell$ exit 0           # exit with status code
```

### Signals

| Signal | Shortcut | Behaviour |
|---|---|---|
| SIGINT | `Ctrl+C` | Interrupt current command, display new prompt |
| EOF | `Ctrl+D` | Exit the shell |
| SIGQUIT | `Ctrl+\` | Ignored at the prompt; terminates child processes |

---

## 🗂️ Project Structure

```
Minishell/
├── Makefile
├── README.md
├── includes/
│   └── minishell.h          # Global header — structs, enums, prototypes
└── src/
    ├── main.c               # Entry point — main loop
    ├── builtins/            # Built-in command implementations
    │   ├── builtin.c
    │   ├── builtin_cd.c
    │   ├── builtin_cd_utils.c
    │   ├── builtin_env.c
    │   ├── builtin_export.c
    │   ├── builtin_export_utils.c
    │   └── echo.c
    ├── exec/                # Process execution layer
    │   ├── command_validation.c
    │   ├── entry_point.c
    │   ├── exec.c
    │   ├── fork.c
    │   ├── heredoc.c
    │   ├── heredoc_child.c
    │   ├── pipeline.c
    │   └── redirect.c
    ├── parsing/             # Input parsing & variable expansion
    │   ├── expand_var.c
    │   ├── expand_var_utils.c
    │   ├── process_pre_parsing.c
    │   ├── process_pre_parsing_bis.c
    │   └── process_var.c
    ├── tokenizer/           # Lexer / tokenizer
    │   ├── create_command.c
    │   ├── handler_token.c
    │   ├── handler_token_2.c
    │   ├── token_check_syntaxe.c
    │   ├── tokenizer.c
    │   └── type_token.c
    ├── utils/               # General utilities
    │   ├── add_history.c
    │   ├── clean.c
    │   ├── debug.c
    │   ├── env.c
    │   ├── env_utils.c
    │   ├── error_handler.c
    │   ├── expand.c
    │   ├── free_command.c
    │   ├── ft_lst_add_back_command.c
    │   ├── handle_exit_var.c
    │   ├── is_char_token.c
    │   ├── print_syntaxe_error.c
    │   └── token_utils.c
    └── usefull/             # Custom libft subset
        ├── ft_isalnum.c
        ├── ft_isalpha.c
        ├── ft_isdigit.c
        ├── ft_itoa.c
        ├── ft_split.c
        ├── ft_strcmp.c
        ├── ft_strdup.c
        ├── ft_strjoin.c
        ├── ft_strlen.c
        └── ...
```

---

## 🏗️ Architecture

Minishell processes each input line through four sequential stages:

```
User input
    │
    ▼
┌──────────────┐
│  Tokenizer   │  Lexical analysis — splits the line into tokens
│  (tokenizer/)│  (words, operators: |  >  >>  <  <<)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│   Parser     │  Semantic analysis — builds a command list,
│  (parsing/)  │  expands variables, handles quotes
└──────┬───────┘
       │
       ▼
┌──────────────┐
│  Executor    │  Forks child processes, sets up pipes and
│   (exec/)    │  redirections, resolves PATH, runs execve()
└──────┬───────┘
       │
       ▼
┌──────────────┐
│  Built-ins   │  Executed in the parent process (no fork)
│ (builtins/)  │  for commands that must affect shell state
└──────────────┘
```

### Key design decisions

- **Fork-based execution** — every external command runs in a child process created with `fork()`. The parent waits with `waitpid()` and stores the exit status.
- **Pipeline** — `n` commands connected by `|` create `n-1` anonymous pipes. Each child inherits the appropriate read/write ends; unused ends are closed before `execve()`.
- **Redirections** — file descriptors are duplicated with `dup2()` in the child process before the executable is loaded.
- **Heredoc** — implemented via a temporary pipe: the shell reads lines until the delimiter and writes them to the pipe's write end; the command reads from the pipe's read end.
- **Signal handling** — `SIGINT` and `SIGQUIT` dispositions are set to `SIG_IGN` in the parent and restored to `SIG_DFL` in each child before `execve()`.

---

## ⚙️ Building & Compilation

```bash
# Build (default target)
make

# Clean object files
make clean

# Clean objects + executable
make fclean

# Full rebuild
make re
```

Compiler flags used: `-Wall -Wextra -Werror`

Linked library: `-lreadline`

---

## 🧪 Testing

Minishell can be tested manually against bash to verify POSIX-compliant behaviour:

```bash
# Start minishell
./minishell

# Compare output with bash for a given command
bash -c "ls | grep .c | wc -l"
./minishell   # then type the same command interactively
```

### Suggested test cases

```bash
# Pipes
ls | cat | wc -l
cat /dev/urandom | head -c 100 | wc -c

# Redirections
echo hello > /tmp/test.txt && cat /tmp/test.txt
echo world >> /tmp/test.txt && cat /tmp/test.txt

# Heredoc
cat << END
line1
line2
END

# Variable expansion
export TEST=42; echo $TEST; unset TEST; echo $TEST

# Exit codes
ls /nonexistent; echo $?
true; echo $?; false; echo $?

# Quotes
echo "Hello $HOME"
echo 'Hello $HOME'

# Edge cases
echo
```
---

## 👤 Author

**enzodobele**

- GitHub: [@enzodobele](https://github.com/enzodobele)

---
