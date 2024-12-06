# Keyboard

### Example

```c
#include "keyboard.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(int argc, char const *argv[])
{
	t_keyboard		*keyboard;
	// save thread id to stop it
	pthread_t		thread_id;
	// key will go here
	unsigned char	*buf;

	buf = (char *)malloc(1);
	// allocate
	keyboard = keyboard_init('q', buf);
	// starts reading stdin
	// result will go into 'buf'
	thread_id = start_keylistener(keyboard);
	while (keyboard->key_can_change)
	{
		if (!*buf)
			continue ;
		printf("key pressed: <%d - %c>\n", *buf, *buf);
		keyboard_reset_buf(buf, 1);
		usleep(100000);
	}
	free(buf);
	// stop thread
	keyboard_safestop(keyboard, thread_id);
	// free space
	keyboard_free(keyboard);
	return (0);
}

```

### Flags

- ECHO = terminal shows what's typed
- ICANON = waits for a \n or EOF
- ISIG = enables standard behaviour for CTRL-Z and CTRL-C
- IXON = enables CTRL-S and CTRL-Q
- IEXTEN = CTRL-V and CTRL-O
- ICRNL = CTRL-M
- OPOST = output processing