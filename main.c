#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

void get_input(char **str);

size_t input_capacity = 256;

int main() {
	char *input = malloc(input_capacity);
	TokenList list;

	while (1) {
		printf("\033[1;34mvsh\033[0m $ ");
		get_input(&input);
		printf("%s\n", input);

		// Tokenize
		init_token_list(&list);
		tokenize_input(&list, &input);
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
