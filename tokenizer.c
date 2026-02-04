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
	list->tokens = malloc((list->capacity) * sizeof(Token));
	for (int i = 0; i < list->capacity; i++) {
		init_token(&((list->tokens)[i]));
	}
}

void init_token(Token *token) {
	token->content = malloc(token_content_capacity);
	token->type = 0;
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
		if (content_pos >= token_content_capacity) {
			token_content_capacity *= 2;
			((list->tokens)[i]).content = realloc(((list->tokens)[i]).content, token_content_capacity);
			if (((list->tokens)[i]).content == NULL) {
				fprintf(stderr, "Reallocation of token content buffer failed.\n");
				exit(1);
			}
		}
		(((list->tokens)[(list->count) - 1]).content)[content_pos++] = (*input)[i];
	}
}
