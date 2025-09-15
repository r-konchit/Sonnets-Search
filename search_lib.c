#include "search_lib.h"
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ll_docs *makeNewNode(char *contents, int index){
    ll_docs *list = (ll_docs*)malloc(sizeof(ll_docs));
    list->index = index;
    list->text = contents;
    return list;
}


void free_table(table *words) {

    for (size_t i = 0; i < words->num_buckets; i++) {
        ll_word *entry = words->buckets[i];
        while (entry) {
            ll_word *word_temp = entry;
            entry = entry->next;

            ll_int *index_node = word_temp->indecies;
            while (index_node) {
                ll_int *index_temp = index_node;
                index_node = index_node->next;
                free(index_temp);
            }

            free(word_temp->text);
            free(word_temp);
        }
    }
    free(words->buckets);
    free(words);
}


void free_docs(ll_docs *docs) {
    while (docs) {
        ll_docs *temp = docs;
        docs = docs->next;
        free(temp->text);
        free(temp);
    }
}

// returns a linked list of the documents, contains the document and its index
ll_docs *load_document(char *filename) {
    ll_docs *head = NULL, *tail = NULL;
    FILE *file = fopen(filename, "r");


    regex_t regex;
    regmatch_t matches[1];
    char line[1024];
    const char *pattern = "^[IVXLC]+$";
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        fclose(file);
        return NULL;
    }

    int index = 0;
    // stores the docs text
    char *buffer = NULL;
    size_t buffer_size = 0;
    bool in_document = false;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (regexec(&regex, line, 1, matches, 0) == 0) {
            if (in_document) {
                // store the previous document before starting a new one
                ll_docs *new_doc = malloc(sizeof(ll_docs));
                new_doc->index = index++;
                new_doc->text = buffer;
                new_doc->next = NULL;

                if (!head) {
                    head = new_doc;
                } else {
                    tail->next = new_doc;
                }
                tail = new_doc;
                // reset the buffer for the next doc
                buffer = NULL;
                buffer_size = 0;
            }
            in_document = true;
            continue;
        }

        if (in_document) {
            // make buffer bigger
            size_t line_length = strlen(line);

            // ADD 2 FOR NEW LINE AND NULL TERMINOSTOR
            buffer = realloc(buffer, buffer_size + line_length + 2);
            strcpy(buffer + buffer_size, line);
            buffer_size += line_length;

            // should have a new line and null terminotr
            buffer[buffer_size++] = '\n';
            buffer[buffer_size] = '\0';  // Ensure null termination
        }
    }

    // DONT FORGET ABOUT THE LAST DOC
    if (in_document && buffer) {
        ll_docs *new_doc = malloc(sizeof(ll_docs));
        new_doc->index = index++;
        new_doc->text = buffer;
        new_doc->next = NULL;

        if (!head) {
            head = new_doc;
        } else {
            tail->next = new_doc;
        }
    }

    regfree(&regex);
    fclose(file);
    return head;
}

// hashed the string
unsigned long djb_hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while (*str != '\0') {
        c = *str;
        hash = ((hash << 5) + hash) + (unsigned char)c; /* hash * 33 + c */
        str++;
    }
    return hash;
}

// makes a table for the words, this will look like this:
// 'word' : {link lists of indecies} (document number)
table *make_table(size_t num_buckets) {
    table *out = calloc(1, sizeof(table));

    out->num_buckets = num_buckets;
    out->buckets = calloc(num_buckets, sizeof(ll_word*));

    return out;
}

// returns the ll_word which contains the word and its indecies
ll_word *lookup(char *name, table *words) {
    unsigned long k = djb_hash(name);
    unsigned long bucket = k % words->num_buckets;

    for (ll_word *here = words->buckets[bucket]; here; here = here->next) {
        if (!strcmp(name, here->text)) {
            return here;
        }
    }

    return NULL;
}


// adds a word and its new index to the hashtable of words
// if the word in already in the hashtable, just
// append the index to the linked list of indecies of that specific word

void add_word(char *word, int index, table *words) {
    unsigned long k = djb_hash(word);
    unsigned long bucket = k % words->num_buckets;

    ll_word *existing = lookup(word, words);

    if (existing != NULL) {
        // Append index to existing word
        ll_int *new = malloc(sizeof(ll_int));
        new->num = index;
        new->next = existing->indecies;
        existing->indecies = new;
    } else {
        // Create new word entry
        ll_word *other = malloc(sizeof(ll_word));
        other->text = strdup(word);
        other->indecies = malloc(sizeof(ll_int));
        other->indecies->num = index;
        other->indecies->next = NULL;
        other->next = words->buckets[bucket];
        words->buckets[bucket] = other;
    }
}

// this goes thru the linked list of documents and returns a hash table
// of the word and in the documents they occur.
table *make_index_table(ll_docs *docs){
    // string : linked list of where that word appears
    // "hello" : {11, 3, 14}
    // for each doc in the linked list:
        // for each word in the doc:
            // hash that word, append the index of the doc to the linked list for that word
            // CHECK IF THAT INDEX IS ALREADY IN THE LINKED LIST, do not add if its there

    table *words = make_table(100);
    ll_docs *current = docs;
    // goes thru each document:
    while(current->next != NULL){
       // for word in the current doc: (make a token for each)
       char *copy = strdup(current->text);
       char *token = strtok(copy, " ,.!?;:\n\r’'");

       while (token != NULL){
           // HASH THE TOKEN TO THE WORDS HASHTABLE
           // WORD IS THE SAME THING AS TOKEN!!!!!!!
           add_word(token, current->index, words);
           token = strtok(NULL, " ,.!?;:\n\r’'");
       }
       free(copy);
       current = current->next;
    }

    return words;
}


char *to_lower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
    return str;
}

bool isIn(ll_int *indecies, int index){
     while (indecies != NULL){
       if (indecies->num == index) return true;
       indecies = indecies->next;
     }
     return false;
}

// this will find each document in the
char *printAndConcat(ll_int *indecies, ll_docs *docs){
    // goes thru each document and gets its id
    // runs the function: isIn(indecies, id)
    size_t totalLength = 1;  // Start with space for '\0'
    char *returnCase = malloc(totalLength);
    returnCase[0] = '\0';  // Ensure string is empty initially

    while (docs != NULL) {
        if (isIn(indecies, docs->index)) {
            size_t textLen = strlen(docs->text);
            totalLength += textLen + 3;

            returnCase = realloc(returnCase, totalLength);


            strcat(returnCase, docs->text);
            strcat(returnCase, "\n\n");
        }
        docs = docs->next;
    }

    // TODO REMEBER TO FREE THE RC!!!!!!
    printf("%s", returnCase);
    return returnCase;
}

ll_int *copy_list(ll_int *list) {
    if (!list) return NULL;
    ll_int *new_list = malloc(sizeof(ll_int));
    new_list->num = list->num;
    new_list->next = copy_list(list->next);
    return new_list;
}

ll_int *intersect_lists(ll_int *list, ll_int *other) {
    ll_int *result = NULL, **tail = &result;
    while (list) {
        ll_int *temp = other;
        while (temp) {
            if (list->num == temp->num) {
                *tail = malloc(sizeof(ll_int));
                (*tail)->num = list->num;
                (*tail)->next = NULL;
                tail = &((*tail)->next);
                break;
            }
            temp = temp->next;
        }
        list = list->next;
    }
    return result;
}

bool lookup_number(int num, ll_int *list) {
    while (list) {
        if (list->num == num) return true;
        list = list->next;
    }
    return false;
}

ll_int *merge_lists(ll_int *list, ll_int *other) {
    ll_int *result = copy_list(list);  // Copy list first
    ll_int *tail = result;

    while (tail && tail->next) tail = tail->next;  // Move to last node

    while (other) {
        if (!lookup_number(other->num, result)) {  // Add only if not present
            ll_int *new_node = malloc(sizeof(ll_int));
            new_node->num = other->num;
            new_node->next = NULL;
            if (!tail) {
                result = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
        }
        other = other->next;
    }
    return result;
}


char *get_query(char *query, char *mode, table *words, ll_docs *docs) {
    // filter thru the given query (words)
    // lookup the word

    // this a string of all the docs that'll be returned
    char *query_copy = strdup(query);
    char *word = strtok(query_copy, " ,.!?;:\n\r’'");
    ll_int *indecies = NULL;

    while (word != NULL) {
        ll_word *entry = lookup(word, words);

        if (entry != NULL) {
            if (strcmp(mode, "and") == 0) {
                if (indecies == NULL) {
                    indecies = copy_list(entry->indecies);
                } else {
                    indecies = intersect_lists(indecies, entry->indecies);
                }
            } else if (strcmp(mode, "or") == 0) {
                indecies = merge_lists(indecies, entry->indecies);
            }
        }
        word = strtok(NULL, " ,.!?;:\n\r’'");
    }

    free(query_copy);  // Free duplicated query

    if (indecies == NULL) {
        printf("Seems like no document contains your search\n");
        return NULL;
    }

    // TODO: remember to free this
    char *result = printAndConcat(indecies, docs);
    return result;
}

