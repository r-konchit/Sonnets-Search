# Shakespeare's Sonnets Search

* author: *Ron Konchitsky*

## introduction

This is a mini search engine for a collection of Shakespeare's Sonnets.

### `search_lib.c`:

Implements these functions:

#### `*ll_text load_document(char *filename)`

This returns a linked list for each of the Sonnets in the text file. It will iteratively go through each of them and creates a struct holding the title and its corresponding text. The document id number will be the number equivalent of the roman numeral associated with the given document (just going to be incrementing a counter for each iteration)

#### `*table make_index_table(ll_docs *docs)`

This iteratively goes through the texts given in the list of documents and will index each word such that the name of the word is the key and a linked list of the document number in which that word occurs.

#### `**char get_query(char *prompt, char *mode)`

Returns and prints documents that either contain all of the words in the given prompt, or just one of the words--depending on the mode (either `AND` or `OR`). This will partition the prompt using regex and will then use each word to find their corresponding documents using the table that's returned from the `search_index()` function. This will also implement the helper function `print_doc()` which will print the document given its index.


### `search.c:`

The main interaction loop. This will call the functions in `search_lib.c` and then prompt the user with some commands with the `prompt_user()` function.

#### `void prompt_user()`

Will set the mode to either `AND` or `OR`. Will ask the user for their search query, or will quit the program and free everything.
