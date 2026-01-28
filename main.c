#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TOKEN_DELIM " \t\n\r\a"

int main(int argc, char *argv[]) {
	while (1) {
		char command[1024];

		// print prompt
		printf("vsh $ ");

		// get command input
		fgets(command, sizeof(command), stdin);

		// tokenize the command by whitespace
		char *args[64] = {NULL};
		int i = 0;
		char *token = strtok(command, TOKEN_DELIM);
		while (token != NULL) {
			args[i++] = token;
			if (i == 63) {
				break;
			}
			token = strtok(NULL, TOKEN_DELIM);
		}

		// split argument list based on pipe character
		char *list[2][64] = {{NULL}, {NULL}};
		i = 0;
		int secondArg = 0, currentArg = 0, commandsCount = 1;

		while (args[i] != NULL) {
			if (strcmp(args[i], "|") == 0) {
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

			list[secondArg][currentArg] = args[i];
			i++;
			currentArg++;
		}

		int fd[2];
		if (commandsCount > 1) {
			pipe(fd);
		}

		for (int c = 0; c < commandsCount; c++) {
			// re-prompt if no command or invalid pipe structure entered
			if (list[c][0] == NULL) {
				if (commandsCount > 1) {
					fprintf(stderr, "unexpected token near |\n");
				}
				continue;
			}

			// handle 'exit' command
			if (strcmp(list[c][0], "exit") == 0) {
				exit(0);
			} else if (strcmp(list[c][0], "cd") == 0) {
				if (list[c][1] != NULL) {
					int rc = chdir(list[c][1]);
					if (rc == -1) {
						fprintf(stderr, "cd: %s: %s\n", list[c][1], strerror(errno));
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
						dup2(fd[1], STDOUT_FILENO);
					}

					close(fd[1]);
					close(fd[0]);
				} else if (c == 1) {
					if (commandsCount > 1) {
						dup2(fd[0], STDIN_FILENO);
					}

					close(fd[0]);
					close(fd[1]);
				}

				if (execvp(list[c][0], list[c]) < 0) {
					if (errno == ENOENT) {
						fprintf(stderr, "%s: command not found\n", list[c][0]);
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
