.SILENT:

NAME := pipex

COMPILER := cc

FLAGS := -Wall -Wextra -Werror

SRCS := pipex.c

OBJS := $(SRCS:%.c=%.o)

all: $(NAME)

%.o: %.c
	$(COMPILER) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) pipex.h
	@cd Libft && $(MAKE) --no-print-directory
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJS) ./Libft/libft.a

clean:
	@cd Libft/ && $(MAKE) --no-print-directory fclean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
