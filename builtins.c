#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"

char *valid_builtins[] = {"exit", "cd", "type"};
int builtins_count = 3;

void handle_builtins(TokenList *list) {
	if (strcmp((list->tokens)[0], "exit") == 0) {
		exit(0);
	} else if (strcmp((list->tokens)[0], "cd") == 0) {
		if (list->count != 2) {
			fprintf(stderr, "Please provide an argument.\n");
			return;
		}
		if (chdir((list->tokens)[1]) < 0) {
			fprintf(stderr, "cd: %s\n", strerror(errno));
		}
	} else if (strcmp((list->tokens)[0], "type") == 0) {
		if (list->count != 2) {
			fprintf(stderr, "Please provide an argument.\n");
			return;
		}
		if (is_builtin((list->tokens)[1]) == 1) {
			printf("%s is a shell builtin\n", (list->tokens)[1]);
		} else {
			printf("%s is an external program\n", (list->tokens)[1]);
		}
	}}

int is_builtin(char *arg) {
	for (int i = 0; i < builtins_count; i++) {
		if (strcmp(arg, valid_builtins[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
