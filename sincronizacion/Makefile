# Variables
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXEC = simulador

# Regla por defecto
all: $(EXEC)

# Compilación de objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Enlazado del ejecutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Limpiar archivos objeto y ejecutable
clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
