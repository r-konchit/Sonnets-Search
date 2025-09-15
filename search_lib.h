#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct ll_int {
    int num;
    struct ll_int *next;
} ll_int;


typedef struct ll_word{
    char *text;
    ll_int *indecies;
    struct ll_word *next;
} ll_word;

typedef struct ll_docs {
    int index;
    char *text;
    struct ll_docs *next;
} ll_docs;

typedef struct table {
    ll_word **buckets;
    size_t num_buckets;
} table;


ll_docs *load_document(char *filename);

table *make_index_table(ll_docs *docs);

char *get_query(char *query, char *mode, table *words, ll_docs *docs);


int score_letter(char letter, char **vocabulary, size_t num_words);

int score_word(char *word, int *letter_scores);

char *get_guess(char **vocabulary, size_t num_words);

size_t filter_vocabulary_gray(char letter, char **vocabulary, size_t num_words);

size_t filter_vocabulary_yellow(char letter, int position, char **vocabulary,
                                size_t num_words);

size_t filter_vocabulary_green(char letter, int position, char **vocabulary,
                               size_t num_words);

void free_vocabulary(char **vocabulary, size_t num_words);

ll_word *lookup(char *name, table *words);

void free_docs(ll_docs *docs);

void free_table(table *words);
// Idea for the UNUSED macro from Stack Overflow user mtvec. Thanks!
// https://stackoverflow.com/a/3599170
#define UNUSED(x) (void)(x)
