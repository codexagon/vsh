#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"

#define TOKEN_DELIM " \t\n\r\a"

int main(int argc, char *argv[]) {
	while (1) {
		char input[1024];

		printf("vsh $ ");

		fgets(input, sizeof(input), stdin);

		// tokenize the input by whitespace
		char *tokens[64] = {NULL};
		int i = 0;
		char *token = strtok(input, TOKEN_DELIM);
		while (token != NULL) {
			tokens[i++] = token;
			if (i == 63) {
				break;
			}
			token = strtok(NULL, TOKEN_DELIM);
		}

		// split argument list based on pipe character
		Command list[64] = {NULL};
		i = 0;
		int secondArg = 0, currentArg = 0, commandsCount = 1;

		for (int j = 0; j < 64; j++) {
			init_command(&(list[j]));
		}

		while (tokens[i] != NULL) {
			if (strcmp(tokens[i], "|") == 0) {
				secondArg++;
				i++;
				commandsCount++;
				currentArg = 0;
				continue;
			}
			if (commandsCount > 2) {
				fprintf(stderr, "too many pipes\n");
				break;
			}

			(list[secondArg].args)[currentArg] = tokens[i];
			i++;
			currentArg++;
		}

		int fd[2];
		if (commandsCount > 1) {
			pipe(fd);
		}

		for (int c = 0; c < commandsCount; c++) {
			// re-prompt if no command or invalid pipe structure entered
			if ((list[c].args)[0] == NULL) {
				if (commandsCount > 1) {
					fprintf(stderr, "unexpected token near |\n");
				}
				continue;
			}

			// handle shell builtins
			if (strcmp((list[c].args)[0], "exit") == 0) {
				exit(0);
			} else if (strcmp((list[c]).args[0], "cd") == 0) {
				if ((list[c].args)[1] != NULL) {
					int rc = chdir((list[c].args)[1]);
					if (rc == -1) {
						fprintf(stderr, "cd: %s: %s\n", (list[c].args)[1], strerror(errno));
					}
				}
				continue;
			}

			// handle general commands
			int pid = fork();
			if (pid < 0) {
				perror("fork failed\n");
			} else if (pid == 0) {
				if (c == 0) {
					if (commandsCount > 1) {
						redirect_output(&(list[c]), fd[1]);
					}

					// close(fd[1]);
					close(fd[0]);
				} else if (c == 1) {
					if (commandsCount > 1) {
						redirect_input(&(list[c]), fd[0]);
					}

					close(fd[1]);
				}

				fprintf(stderr, "cmd: %s, in: %i, out: %i\n", (list[c].args)[0], list[c].input_fd, list[c].output_fd);

				if (execvp((list[c].args)[0], list[c].args) < 0) {
					if (errno == ENOENT) {
						fprintf(stderr, "%s: command not found\n", (list[c].args)[0]);
					}
				}
				exit(1);
			}
		}
		if (commandsCount > 1) {
			close(fd[0]);
			close(fd[1]);
		}

		for (int c = 0; c < commandsCount; c++) {
			wait(NULL);
		}
	}
	return 0;
}
