#CC = clang
#CFLAGS = -Wall -Wpedantic -Werror -Wextra
#
#
#all: search search_lib
#
#search: search.o solver.o
#	$(CC) search_lib.o solver.o -o solver
#
#
#search_test: search_lib.o search_test.o
#	$(CC) search_lib.o search_lib_test.o -o search_test
#
#search_test.o: search_test.c search_lib.h
#	$(CC) $(CFLAGS) -c search_test.c
#
#search_lib.o: search_lib.c search_lib.h
#	$(CC) $(CFLAGS) -c search_lib.c
#
#search.o: search.c search_lib.h
#	$(CC) $(CFLAGS) -c search.c
#
#clean:
#	rm -f search_lib.o solver.o search_lib_test.o solver search_lib_test
#
#format:
#	clang-format -i -style=file *.[ch]


CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: search search_test

# Build the main search program
search: search.o search_lib.o
	$(CC) search.o search_lib.o -o search

# Build the test executable
search_test: search_test.o search_lib.o
	$(CC) search_test.o search_lib.o -o search_test

# Compile search_test.c
search_test.o: search_test.c search_lib.h
	$(CC) $(CFLAGS) -c search_test.c

# Compile search_lib.c
search_lib.o: search_lib.c search_lib.h
	$(CC) $(CFLAGS) -c search_lib.c

# Compile search.c
search.o: search.c search_lib.h
	$(CC) $(CFLAGS) -c search.c

# Clean build files
clean:
	rm -f search.o search_lib.o search_test.o search search_test

# Format code using clang-format
format:
	clang-format -i -style=file *.[ch]
