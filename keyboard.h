/*
	Desc: thread that handles key input
	Integration: #include <keyboard.h>,
		allocate a keyboard and call 'start_keylistener'
*/

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>

# define KEYBOARD_MUTEX_NAME "/keyboard_mutex"

typedef struct s_keyboard
{
	unsigned char	*buf;
	unsigned char	exit_char;
	bool			running;
	bool			key_can_change;
}					t_keyboard;

t_keyboard			*keyboard_init(unsigned char exit_char, unsigned char *buf);
void				keyboard_free(t_keyboard *keyboard);
pthread_t			start_keylistener(t_keyboard *keyboard);
void				set_key_can_change(t_keyboard *keyboard, bool value);
/* static void		*listen(void *args); */
void				enable_raw_mode(void);
void				disable_raw_mode(void);

void				keyboard_reset_buf(char *buf, size_t size);
void				keyboard_set_buf(char *buf, char c, size_t size);
void				keyboard_bruteforce_exit(t_keyboard *keyboard);
void				keyboard_safestop(t_keyboard *keyboard,
						pthread_t thread_id);

# define LINE_FEED 10
# define CARRIAGE_RETURN 13
# define ESC 27
# define DELETE 127
# define ARROW_UP 129
# define ARROW_DOWN 141
# define ARROW_RIGHT 143
# define ARROW_LEFT 144
#endif