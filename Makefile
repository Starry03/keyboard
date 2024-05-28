CC = gcc
FLAGS = -Werror -Wall -Wextra
NAME = keyboard.a

keyboard.o:
	$(CC) $(FLAGS) -I. -c keyboard.c -o keyboard.o

all: keyboard.o
	ar rcs $(NAME) keyboard.o
	ranlib $(NAME)
	@rm keyboard.o

clean:
	@rm keyboard.o

fclean: clean
	@rm $(NAME)

re: fclean all

.PHONY: all
	