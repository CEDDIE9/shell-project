#include "my_tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

TOKENIZER *init_tokenizer(char *string) {
  assert(string != NULL);
  
  TOKENIZER *tokenizer = (TOKENIZER *)malloc(sizeof(TOKENIZER));
  if (tokenizer == NULL) {
    perror("Failed to allocate tokenizer");
    return NULL;
  }

  int len = strlen(string) + 1;
  tokenizer->str = (char *)malloc(len);
  if (tokenizer->str == NULL) {
    perror("Failed to allocate tokenizer string");
    free(tokenizer);
    return NULL;
  }
  
  memcpy(tokenizer->str, string, len);
  tokenizer->pos = tokenizer->str;
  return tokenizer;
}

void free_tokenizer(TOKENIZER *tokenizer) {
  assert(tokenizer != NULL);
  free(tokenizer->str);
  free(tokenizer);
}

char *get_next_token(TOKENIZER *tokenizer) {
  assert(tokenizer != NULL);

  while (isspace(*tokenizer->pos)) tokenizer->pos++;  // Skip leading spaces

  if (*tokenizer->pos == '\0') return NULL;  // End of string

  char *startptr = tokenizer->pos;

  // Single-character tokens
  if (strchr("|&<>", *startptr) != NULL) {
    char *tok = (char *)malloc(2);  // Allocate space for char and null terminator
    if (tok == NULL) {
      perror("Failed to allocate token");
      return NULL;
    }
    tok[0] = *startptr++;
    tok[1] = '\0';
    tokenizer->pos = startptr;
    return tok;
  }

  // Other tokens
  char *endptr = startptr;
  while (*endptr && !isspace(*endptr) && strchr("|&<>", *endptr) == NULL) {
    endptr++;
  }

  int toklen = endptr - startptr;
  char *tok = (char *)malloc(toklen + 1);
  if (tok == NULL) {
    perror("Failed to allocate token");
    return NULL;
  }
  memcpy(tok, startptr, toklen);
  tok[toklen] = '\0';
  tokenizer->pos = endptr;

  return tok;
}
