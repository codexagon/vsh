#ifndef COMMAND_H
#define COMMAND_H

#include <unistd.h>

#define MAX_ARGS 64

typedef struct {
	char *args[MAX_ARGS];

	int input_fd;
	int output_fd;
	int err_fd;
} Command;

void init_command(Command *cmd);
void redirect_input(Command *cmd, int new_fd);
void redirect_output(Command *cmd, int new_fd);
void redirect_error(Command *cmd, int new_fd);

#endif
