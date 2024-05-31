#include "keyboard.h"
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios	default_termios;

t_keyboard	*keyboard_init(uint32_t exit_char, unsigned char *buf)
{
	t_keyboard	*keyboard;

	keyboard = (t_keyboard *)malloc(sizeof(t_keyboard));
	keyboard->exit_char = exit_char;
	keyboard->running = true;
	keyboard->buf = buf;
	return (keyboard);
}

inline void	keyboard_free(t_keyboard *keyboard)
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
	// new_termios.c_oflag &= ~(OPOST);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}

static void	*listen(void *arg)
{
	t_keyboard		*args;
	unsigned char	local_buffer;
	size_t			char_size;

	char_size = sizeof(unsigned char);
	args = (t_keyboard *)arg;
	enable_raw_mode();
	while (args->running && read(STDIN_FILENO, &local_buffer, char_size) > 0)
	{
		*(args->buf) = local_buffer;
		if (local_buffer == args->exit_char)
			break ;
	}
	return (NULL);
}

/**
 * @brief Start the key listener thread
 * @return thread id
 */
unsigned long	start_keylistener(t_keyboard *keyboard)
{
	pthread_t	thread;

	*keyboard->buf = 0;
	pthread_create(&thread, NULL, listen, (void *)keyboard);
	return (thread);
}

void	keyboard_bruteforce_exit(t_keyboard *keyboard)
{
	// set running to false -> exit thread
	keyboard->running = false;
}

void	keyboard_safestop(t_keyboard *keyboard, unsigned long thread_id)
{
	keyboard_bruteforce_exit(keyboard);
	pthread_join(thread_id, NULL);
}