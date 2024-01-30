/*
	Desc: thread that handles key input
	Integration: #include <keyboard.h>, allocate a keyboard and call 'start_keylistener'
*/

#ifndef KEYBOARD_H
# define KEYBOARD_H

#include <stdbool.h>

typedef struct s_keyboard {
	char	exit_char;
}	t_keyboard;

typedef struct s_listen_args {
	t_keyboard	*keyboard;
	char		*buf;
}	t_listen_args;

t_keyboard			*keyboard_init(char exit_char);
/* static void		keyboard_free(t_keyboard *keyboard); */
void				start_keylistener(t_keyboard *keyboard, char *buf);
/* static void		*listen(void *args); */
void				enable_raw_mode();
void				disable_raw_mode();

#endif