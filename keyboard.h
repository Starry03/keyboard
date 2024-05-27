/*
	Desc: thread that handles key input
	Integration: #include <keyboard.h>,
		allocate a keyboard and call 'start_keylistener'
*/

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include <stdbool.h>

typedef struct s_keyboard
{
	char	*buf;
	char	exit_char;
	bool	running;
}			t_keyboard;

t_keyboard	*keyboard_init(char exit_char, char *buf);
void		keyboard_free(t_keyboard *keyboard);
void		start_keylistener(t_keyboard *keyboard);
/* static void		*listen(void *args); */
void		enable_raw_mode(void);
void		disable_raw_mode(void);

void		keyboard_bruteforce_exit(t_keyboard *keyboard);

#endif