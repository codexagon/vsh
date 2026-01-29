#include "command.h"

void init_command(Command *cmd) {
	for (int i = 0; i < MAX_ARGS; i++) {
		(cmd->args)[i] = NULL;
	}

	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->err_fd = STDERR_FILENO;
}

void redirect_input(Command *cmd, int new_fd) {
	dup2(new_fd, STDIN_FILENO);
	close(new_fd);
	cmd->input_fd = new_fd;
}

void redirect_output(Command *cmd, int new_fd) {
	dup2(new_fd, STDOUT_FILENO);
	close(new_fd);
	cmd->output_fd = new_fd;
}

void redirect_error(Command *cmd, int new_fd) {
	dup2(new_fd, STDERR_FILENO);
	close(new_fd);
	cmd->err_fd = new_fd;
}
