#ifndef BUILTINS_H
#define BUILTINS_H

#include "tokenizer.h"

void handle_builtins(TokenList *list);
int is_builtin(char *arg);

#endif
