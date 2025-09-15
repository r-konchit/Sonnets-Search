#include "search_lib.h"
#include <stdio.h>
#include <regex.h>
#include <string.h>

void print_index_table(table *words) {
    if (words == NULL) {
        printf("Table is empty.\n");
        return;
    }

    for (size_t i = 0; i < words->num_buckets; i++) {
        ll_word *entry = words->buckets[i];

        while (entry != NULL) {

            ll_int *index_node = entry->indecies;
            while (index_node != NULL) {
                if (index_node->next != NULL) {
                }
                index_node = index_node->next;
            }

            entry = entry->next;
        }
    }
}
int main(void) {
        printf("Welcome to the Search of Sonnets!\n");

        ll_docs *docs = load_document("sonnets.txt");
        table *words = make_index_table(docs);
        print_index_table(words);
        char query[256];
        while(strcmp(query, "quit") != 0){
            char *mode = malloc(256 * sizeof(char));
            while(strcmp(mode, "and") != 0 && strcmp(mode, "or") != 0  && strcmp(mode, "*quit") != 0){
                printf("Select a mode ('and' or 'or') and begin your search! --or type '*quit' to quit\n");
                scanf("%s", mode);
            }
            if (strcmp(mode, "*quit") == 0) break;
            printf("Please type your search: (you can type '*quit' to quit)\n");
            scanf("%s", query);
            if (strcmp(query, "*quit") == 0) break;
            char *search = get_query(query, mode, words, docs);
            free(search);
            free(mode);
        }

        printf("Goodbye\n");
        free_docs(docs);
        free_table(words);
        return 0;
}



