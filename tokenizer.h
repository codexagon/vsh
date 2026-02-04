#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct Token {
	char *content;
	int type; // 0: normal, 1: |, 2: >, 3: >>, 4: <
} Token;

typedef struct TokenList {
	Token *tokens;
	int count;
	int capacity;
} TokenList;

void init_token_list(TokenList *list);
void init_token(Token *token);
void tokenize_input(TokenList *list, char **input);

#endif
