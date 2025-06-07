CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

OBJ = obj/main.o obj/action.o obj/eventlog.o obj/process.o obj/resource.o \
      obj/simulation.o obj/sync.o obj/timeline.o obj/gui.o

programa: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf obj programa
