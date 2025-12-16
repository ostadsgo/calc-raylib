# Variables
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lraylib -lm -lpthread -ldl

# Default target
all: app

# Build rule
app: app.c
	$(CC) $(CFLAGS) app.c -o app $(LIBS)

# Run
run: app
	@./app

# Clean
clean:
	rm -f app
