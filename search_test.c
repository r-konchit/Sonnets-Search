#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search_lib.h"


void print_docs(ll_docs *docs) {
    while (docs) {
        printf("Document #%d:\n%s\n", docs->index, docs->text);
        docs = docs->next;
    }
}

void test_load_document(void) {
    printf("\nRunning test: load_document()\n");

    ll_docs *docs = load_document("sonnets.txt");

    int count = 0;
    ll_docs *temp = docs;
    while (temp) {
        temp = temp->next;
        count++;
    }

    if (count == 0) {
        printf("ERROR: No documents were loaded.\n");
    } else {
        printf("Passed test: load_document()\n");
    }
    free(docs);
}

void test_make_index_table(void) {
    printf("\nRunning test: make_index_table()\n");

    ll_docs *docs = load_document("sonnets.txt");
    if (docs == NULL) {
        printf("ERROR: Failed to load documents.\n");
        return;
    }

    table *words = make_index_table(docs);
    if (words == NULL) {
        printf("ERROR: Failed to create index table.\n");
        return;
    }

    // see if the words are there
    ll_word *hello = lookup("hello", words);
    if (hello == NULL) {
        printf("ERROR: 'hello' not found in index table.\n");
    } else {
        printf("Found 'hello' in documents: {");
        ll_int *indices = hello->indecies;
        while (indices) {
            indices = indices->next;
        }
        printf("}\n");
    }

    ll_word *love = lookup("love", words);
    if (love == NULL) {
        printf("ERROR: 'love' not found in index table.\n");
    } else {
        printf("Found 'love' in documents: {");
        ll_int *indices = love->indecies;
        while (indices) {
            printf("%d ", indices->num);
            indices = indices->next;
        }
        printf("}\n");
    }

    printf("Passed test: make_index_table()\n");
}

void test_get_query(void) {
    printf("\nRunning test: get_query()\n");

    ll_docs *docs = load_document("sonnets.txt");
    if (docs == NULL) {
        printf("ERROR: Failed to load documents.\n");
        return;
    }

    table *words = make_index_table(docs);
    if (words == NULL) {
        printf("ERROR: Failed to create index table.\n");
        return;
    }

    // word that exists in the index
    char *result = get_query("love", "or", words, docs);
    if (result == NULL) printf("ERROR: Search for 'love' returned NULL but should have returned results.\n");
    else {
      printf("Passed test\n");
        free(result);
    }

    // does not exist
    result = get_query("xyzabc", "or", words, docs);
    if (result != NULL) {
        printf("ERROR: Search for 'xyzabc' should have returned NULL, but got:\n%s\n", result);
        free(result);
    } else {
        printf("Search for 'xyzabc' correctly returned NULL.\n");
    }

    // tesing for and
    result = get_query("love beauty", "and", words, docs);
    if (result) {
        printf("Passed search for 'love beauty' (AND mode)\n");
        free(result);
    } else {
        printf("FAILED search for 'love beauty' (AND mode) \n");
    }

    printf("passed test: get_query()\n");
}

int main(void) {
    printf("Starting test cases...\n");

    test_load_document();
    test_make_index_table();
    test_get_query();
    return 0;
}


