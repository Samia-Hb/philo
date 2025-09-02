NAME = philo

CC = cc #-fsanitize=thread -pthread -g3
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCS = main.c philo.c simulation.c philo_utils.c parse.c ft_atoi.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all