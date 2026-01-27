#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

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

		// show prompt again if no input given
		if (args[0] == NULL) {
			continue;
		}

		// handle 'exit' command
		if (strcmp(args[0], "exit") == 0) {
			exit(0);
		} else if (strcmp(args[0], "cd") == 0) {
			if (args[1] != NULL) {
				int rc = chdir(args[1]);
				if (rc == -1) {
					fprintf(stderr, "cd: %s: %s\n", args[1], strerror(errno));
				}
			}
			continue;
		}

		// handle general commands
		int pid = fork();
		if (pid < 0) {
			perror("fork failed\n");
		} else if (pid == 0) {
			if (execvp(args[0], args) < 0) {
				if (errno == ENOENT) {
					fprintf(stderr, "%s: command not found\n", args[0]);
				}
			}
			exit(1);
		} else {
			wait(NULL);
		}
	}
	return 0;
}
