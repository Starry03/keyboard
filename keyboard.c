#include "keyboard.h"
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios	default_termios;

t_keyboard	*keyboard_init(char exit_char)
{
	t_keyboard	*keyboard;

	keyboard = (t_keyboard *)malloc(sizeof(t_keyboard));
	keyboard->exit_char = exit_char;
	keyboard->running = true;
	return (keyboard);
}

static void	keyboard_free(t_keyboard *keyboard)
{
	free(keyboard);
}

void	disable_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &default_termios);
}

void	enable_raw_mode(void)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, &default_termios);
	atexit(disable_raw_mode);
	new_termios = default_termios;
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	new_termios.c_iflag &= ~(IXON);
	new_termios.c_oflag &= ~(OPOST);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}

static void	*listen(void *arg)
{
	t_listen_args	*args;
	char			local_buffer;

	args = (t_listen_args *)arg;
	enable_raw_mode();
	while (args->keyboard->running && read(STDIN_FILENO, &local_buffer, 1) == 1
		&& local_buffer != args->keyboard->exit_char)
		*(args->buf) = local_buffer;
	keyboard_free(args->keyboard);
	free(args);
	return (NULL);
}

void	start_keylistener(t_keyboard *keyboard, char *buf)
{
	t_listen_args	*args;
	pthread_t		thread;

	args = malloc(sizeof(t_listen_args));
	args->keyboard = keyboard;
	args->buf = buf;
	*buf = 0;
	pthread_create(&thread, NULL, listen, (void *)args);
}

void	keyboard_bruteforce_exit(t_keyboard *keyboard)
{
	// set running to false -> exit thread
	keyboard->running = false;
}