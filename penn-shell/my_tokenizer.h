#ifndef MY_TOKENIZER_H  
#define MY_TOKENIZER_H

typedef struct tokenizer {
  char *str;  // Original string being tokenized
  char *pos;  // Current position in string
} TOKENIZER;

// Initializes a tokenizer for the given string.
TOKENIZER *init_tokenizer(char *string);

// Frees the memory allocated for the tokenizer.
void free_tokenizer(TOKENIZER *tokenizer);

// Returns the next token from the tokenizer.
char *get_next_token(TOKENIZER *tokenizer);

#endif  
