#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "tokenizer.h"

void get_input(char **str);
void convert_to_array(TokenList *list, char ***args);
void execute(TokenList *list);

int input_capacity = 256;

int main() {
	char *input = malloc(input_capacity);
	TokenList list;

	while (1) {
		printf("\033[1;34mvsh\033[0m $ ");
		get_input(&input);

		// Tokenize
		init_token_list(&list);
		tokenize_input(&list, &input);

		execute(&list);
	}

	return 0;
}

void get_input(char **str) {
	int i = 0, c = 0;
	while ((c = getchar()) != '\n' && c != EOF) {
		if (i >= input_capacity) {
			input_capacity *= 2;
			*str = realloc(*str, input_capacity);
			if (*str == NULL) {
				fprintf(stderr, "Reallocation of input buffer failed.\n");
				exit(1);
			}
		}
		(*str)[i++] = c;
	}
	(*str)[i] = '\0';
}

void execute(TokenList *list) {
	if (is_builtin((list->tokens)[0]) == 1) {
		handle_builtins(list);
		return;
	}

	switch (fork()) {
	case -1:
		fprintf(stderr, "Fork failed.\n");
		exit(2);
		break;
	case 0:
		if (execvp((list->tokens)[0], list->tokens) < 0) {
			if (errno == ENOENT) {
				fprintf(stderr, "%s: command not found\n", (list->tokens)[0]);
			} else {
				fprintf(stderr, "%i: %s\n", errno, strerror(errno));
			}
		}
		exit(1); // calls only if execvp failed
		break;
	default:
		wait(NULL);
		break;
	}
}
