CC = gcc
CFLAGS = -Wall -Wextra -I./src
SRC = src/server.c src/http.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = html09-server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean