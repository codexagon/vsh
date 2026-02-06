#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct TokenList {
	char **tokens;
	int count;
	int capacity;
} TokenList;

void init_token_list(TokenList *list);
void tokenize_input(TokenList *list, char **input);

#endif
