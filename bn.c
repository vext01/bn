#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

int
main(void)
{
	char			*line = 0;

	while (line = readline("> ") != NULL) {
		if (*line) {
			//stuff
		}
		free(line);
	}

	return (EXIT_SUCCESS);

}
