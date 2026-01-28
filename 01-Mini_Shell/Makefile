CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = minishell

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)