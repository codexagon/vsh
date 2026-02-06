#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

int token_content_capacity = 64;

void init_token_list(TokenList *list) {
	list->count = 0;
	list->capacity = 8;
	if (list->tokens != NULL) {
		free(list->tokens);
	}
	list->tokens = malloc((list->capacity) * sizeof(char *));
	for (int i = 0; i < list->capacity; i++) {
		(list->tokens)[i] = malloc(token_content_capacity * sizeof(char) + 1);
	}
}

void tokenize_input(TokenList *list, char **input) {
	int content_pos = 0;
	list->count = 1;
	for (int i = 0; (*input)[i] != '\0'; i++) {
		if ((*input)[i] == ' ') {
			content_pos = 0;
			(list->count)++;
			continue;
		}
		if (list->count >= list->capacity) {
			list->capacity *= 2;
			list->tokens = realloc(list->tokens, (list->capacity) * sizeof(char *));
			if (list->tokens == NULL) {
				fprintf(stderr, "Reallocation of token list failed.\n");
				exit(1);
			}
			for (int j = list->count; j < list->capacity; j++) {
				(list->tokens)[j] = malloc(token_content_capacity * sizeof(char) + 1);
			}
		}
		if (content_pos >= token_content_capacity) {
			token_content_capacity *= 2;
			(list->tokens)[i] = realloc((list->tokens)[i], token_content_capacity);
			if ((list->tokens)[i] == NULL) {
				fprintf(stderr, "Reallocation of token content buffer failed.\n");
				exit(1);
			}
		}
		((list->tokens)[(list->count) - 1])[content_pos++] = (*input)[i];
	}
	(list->tokens)[(list->count)] = NULL;
}
