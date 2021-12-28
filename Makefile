CFLAGS := -Wall -ansi -pedantic

dcg: dcg.c
	$(CC) $(CFLAGS) $< -o $@

