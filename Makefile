CC = gcc
FLAGS = -Werror -Wall -Wextra
NAME = keyboard.a

all: keyboard.o
	ar rcs $(NAME) keyboard.o
	ranlib $(NAME)

keyboard.o:
	$(CC) $(FLAGS) -I. -c keyboard.c -o keyboard.o

clean:
	@rm keyboard.o

fclean: clean
	@rm $(NAME)

re: fclean all

.PHONY: all
	