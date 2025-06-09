CC = gcc
#CC = clang
CFLAGS = -Iinclude
#CFLAGS = -g -Wall -Wextra -fsanitize=memory -Iinclude
#CFLAGS = -g -Wall -Wextra -fsanitize=address -Iinclude
SRC = src/main.c src/operations.c src/parser.c src/psf.c
TARGET = main

$(TARGET): $(SRC)
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f $(TARGET)
